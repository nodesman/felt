#include "editor.h"
#include "themeregistry.h"
#include <QFontDatabase>
#include <QScrollBar>
#include <QResizeEvent>
#include <QKeyEvent>
#include <QTextBlock>

FeltEditor::FeltEditor(QWidget *parent)
    : QPlainTextEdit(parent)
    , m_measure(DocumentMeasure::kDefaultColumnWidth)
{
    setFrameStyle(QFrame::NoFrame);
    setLineWrapMode(QPlainTextEdit::WidgetWidth);

    setupTypography();

    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setCenterOnScroll(m_typewriterMode);
    setCursorWidth(2);

    m_highlighter = new FeltHighlighter(document());
    applyTheme(ThemeRegistry::obsidianVoid());

    connect(this, &QPlainTextEdit::cursorPositionChanged,
            this, &FeltEditor::onCursorPositionChanged);
    connect(this, &QPlainTextEdit::textChanged,
            this, &FeltEditor::onTextChanged);
}

QFont FeltEditor::resolveMonospaceFont(const QStringList &installedFamilies,
                                       const QStringList &priorityOrder,
                                       int pointSize) {
    QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    for (const QString &candidate : priorityOrder) {
        if (installedFamilies.contains(candidate)) {
            font.setFamily(candidate);
            break;
        }
    }
    font.setPointSize(pointSize);
    font.setLetterSpacing(QFont::PercentageSpacing, 102);
    return font;
}

void FeltEditor::setupTypography() {
    const QStringList priorityOrder = {
        QStringLiteral("Menlo"),
        QStringLiteral("Monaco"),
        QStringLiteral("SF Mono"),
        QStringLiteral("Courier New")
    };
    setFont(resolveMonospaceFont(QFontDatabase::families(), priorityOrder, 16));
}

void FeltEditor::applyTheme(const ThemePalette &palette) {
    if (m_highlighter) {
        m_highlighter->setTheme(palette);
    }
    QPalette pal = this->palette();
    pal.setColor(QPalette::Base, palette.bgOuter());
    pal.setColor(QPalette::Text, palette.textMain());
    pal.setColor(QPalette::Highlight, palette.selectionBg());
    pal.setColor(QPalette::HighlightedText, palette.selectionText());
    setPalette(pal);

    setStyleSheet(QStringLiteral(
        "QPlainTextEdit { "
        "  background-color: %1; "
        "  color: %2; "
        "  selection-background-color: %3; "
        "  selection-color: %4; "
        "  qproperty-cursorWidth: 2; "
        "  border: none; "
        "}"
    ).arg(palette.bgOuter().name(),
          palette.textMain().name(),
          palette.selectionBg().name(),
          palette.selectionText().name()));
}

ThemePalette FeltEditor::currentTheme() const {
    if (m_highlighter) {
        return m_highlighter->theme();
    }
    return ThemeRegistry::obsidianVoid();
}

void FeltEditor::setColumnWidth(int width) {
    m_measure = DocumentMeasure(width);
    updateMargins();
}

void FeltEditor::setFocusMode(bool enabled) {
    if (m_highlighter && m_highlighter->isFocusMode() != enabled) {
        m_highlighter->setFocusMode(enabled);
        emit focusModeToggled(enabled);
    }
}

bool FeltEditor::isFocusMode() const {
    return m_highlighter ? m_highlighter->isFocusMode() : false;
}

void FeltEditor::toggleFocusMode() {
    setFocusMode(!isFocusMode());
}

void FeltEditor::setTypewriterMode(bool enabled) {
    if (m_typewriterMode == enabled) {
        return;
    }
    m_typewriterMode = enabled;
    setCenterOnScroll(m_typewriterMode);
    if (m_typewriterMode) {
        centerCursor();
    }
    emit typewriterModeToggled(enabled);
}

bool FeltEditor::isTypewriterMode() const {
    return m_typewriterMode;
}

void FeltEditor::toggleTypewriterMode() {
    setTypewriterMode(!m_typewriterMode);
}

void FeltEditor::resizeEvent(QResizeEvent *event) {
    QPlainTextEdit::resizeEvent(event);
    updateMargins();
    if (m_typewriterMode) {
        centerCursor();
    }
}

void FeltEditor::updateMargins() {
    const ViewportMargins margins = m_measure.computeMargins(width());
    setViewportMargins(margins.left, margins.top, margins.right, margins.bottom);
}

void FeltEditor::onCursorPositionChanged() {
    if (m_highlighter) {
        m_highlighter->setCurrentBlockNumber(textCursor().blockNumber());
    }
    emit statsChanged();
}

void FeltEditor::onTextChanged() {
    emit statsChanged();
}

void FeltEditor::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Tab) {
        insertPlainText(QStringLiteral("    "));
        return;
    }
    QPlainTextEdit::keyPressEvent(event);
}
