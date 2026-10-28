#include "editor.h"
#include "themeregistry.h"
#include <QFontDatabase>
#include <QScrollBar>
#include <QResizeEvent>
#include <QKeyEvent>
#include <QRegularExpression>
#include <QTextBlock>
#include <algorithm>

namespace {
struct ScopedEditBlock {
    QTextCursor &cursor;
    explicit ScopedEditBlock(QTextCursor &c) : cursor(c) { cursor.beginEditBlock(); }
    ~ScopedEditBlock() { cursor.endEditBlock(); }
};
} // namespace

FeltEditor::FeltEditor(QWidget *parent)
    : QPlainTextEdit(parent), m_measure(DocumentMeasure::kDefaultColumnWidth)
{
    setFrameStyle(QFrame::NoFrame);
    setLineWrapMode(QPlainTextEdit::WidgetWidth);
    setTypography(QStringLiteral("Menlo"), 16);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setCenterOnScroll(m_typewriterMode);
    setCursorWidth(2);
    m_highlighter = new FeltHighlighter(document());
    applyTheme(ThemeRegistry::obsidianVoid());
    connect(this, &QPlainTextEdit::cursorPositionChanged, this, &FeltEditor::onCursorPositionChanged);
    connect(this, &QPlainTextEdit::textChanged, this, &FeltEditor::onTextChanged);
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

void FeltEditor::setTypography(const QString &family, int pointSize) {
    const QStringList priority = {family, QStringLiteral("Menlo"), QStringLiteral("Monaco"),
                                  QStringLiteral("SF Mono"), QStringLiteral("Courier New")};
    setFont(resolveMonospaceFont(QFontDatabase::families(), priority, pointSize));
}

void FeltEditor::setCaretWidth(int pixels) { setCursorWidth(std::clamp(pixels, 1, 3)); }

void FeltEditor::applyTheme(const ThemePalette &palette) {
    if (m_highlighter) m_highlighter->setTheme(palette);
    QPalette pal = this->palette();
    pal.setColor(QPalette::Base, palette.bgOuter());
    pal.setColor(QPalette::Window, palette.bgOuter());
    pal.setColor(QPalette::Text, palette.textMain());
    pal.setColor(QPalette::Highlight, palette.selectionBg());
    pal.setColor(QPalette::HighlightedText, palette.selectionText());
    setPalette(pal);
    setStyleSheet(QStringLiteral(
        "QPlainTextEdit { background-color: %1; color: %2; selection-background-color: %3; selection-color: %4; border: none; }"
    ).arg(palette.bgOuter().name(), palette.textMain().name(), palette.selectionBg().name(), palette.selectionText().name()));
    viewport()->update();
}

ThemePalette FeltEditor::currentTheme() const {
    return m_highlighter ? m_highlighter->theme() : ThemeRegistry::obsidianVoid();
}

void FeltEditor::applyLineSpacingToAllBlocks() {
    if (m_applyingLineSpacing || !document()) return;
    m_applyingLineSpacing = true;
    const bool wasModified = document()->isModified();
    const bool hasUndo = document()->isUndoAvailable();
    if (!hasUndo) document()->setUndoRedoEnabled(false);
    for (QTextBlock b = document()->begin(); b.isValid(); b = b.next()) {
        if (qFuzzyCompare(b.blockFormat().lineHeight(), 160.0)) continue;
        QTextCursor cur(b);
        if (hasUndo) cur.joinPreviousEditBlock();
        QTextBlockFormat fmt = b.blockFormat();
        fmt.setLineHeight(160.0, QTextBlockFormat::ProportionalHeight);
        cur.setBlockFormat(fmt);
        if (hasUndo) cur.endEditBlock();
    }
    if (!hasUndo) document()->setUndoRedoEnabled(true);
    if (!wasModified) document()->setModified(false);
    m_applyingLineSpacing = false;
}

void FeltEditor::setColumnWidth(int width) { m_measure = DocumentMeasure(width); updateMargins(); }

void FeltEditor::setFocusMode(bool enabled) {
    if (m_highlighter && m_highlighter->isFocusMode() != enabled) {
        m_highlighter->setFocusMode(enabled);
        emit focusModeToggled(enabled);
    }
}

bool FeltEditor::isFocusMode() const { return m_highlighter ? m_highlighter->isFocusMode() : false; }
void FeltEditor::toggleFocusMode() { setFocusMode(!isFocusMode()); }

void FeltEditor::setTypewriterMode(bool enabled) {
    if (m_typewriterMode == enabled) return;
    m_typewriterMode = enabled;
    setCenterOnScroll(m_typewriterMode);
    if (m_typewriterMode) centerCursor();
    emit typewriterModeToggled(enabled);
}

bool FeltEditor::isTypewriterMode() const { return m_typewriterMode; }
void FeltEditor::toggleTypewriterMode() { setTypewriterMode(!m_typewriterMode); }

void FeltEditor::resizeEvent(QResizeEvent *event) {
    QPlainTextEdit::resizeEvent(event);
    updateMargins();
    if (m_typewriterMode) centerCursor();
}

void FeltEditor::updateMargins() {
    const ViewportMargins m = m_measure.computeMargins(width());
    setViewportMargins(m.left, m.top, m.right, m.bottom);
}

void FeltEditor::onCursorPositionChanged() {
    if (m_highlighter) m_highlighter->setCurrentBlockNumber(textCursor().blockNumber());
    emit statsChanged();
}

void FeltEditor::onTextChanged() { applyLineSpacingToAllBlocks(); emit statsChanged(); }

bool FeltEditor::handleListReturnKey() {
    QTextCursor cur = textCursor();
    const QString line = cur.block().text();
    static const QRegularExpression listRe(QStringLiteral("^(\\s*)([-*+]|\\d+\\.)\\s(.*)$"));
    const QRegularExpressionMatch match = listRe.match(line);
    if (!match.hasMatch()) return false;
    const QString indent = match.captured(1);
    const QString marker = match.captured(2);
    const QString body = match.captured(3);
    ScopedEditBlock editGuard(cur);
    if (body.trimmed().isEmpty()) {
        cur.movePosition(QTextCursor::StartOfBlock);
        cur.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
        cur.removeSelectedText();
        setTextCursor(cur);
        return true;
    }
    QString nextMarker = marker;
    if (marker.endsWith(QLatin1Char('.'))) {
        nextMarker = QString::number(marker.left(marker.size() - 1).toInt() + 1) + QStringLiteral(".");
    }
    cur.insertText(QStringLiteral("\n") + indent + nextMarker + QStringLiteral(" "));
    setTextCursor(cur);
    return true;
}

bool FeltEditor::handleTabKey(bool unindent) {
    QTextCursor cur = textCursor();
    ScopedEditBlock editGuard(cur);
    if (!unindent) {
        cur.movePosition(QTextCursor::StartOfBlock);
        cur.insertText(QStringLiteral("    "));
        return true;
    }
    const QString line = cur.block().text();
    int spaces = 0;
    while (spaces < 4 && spaces < line.size() && line.at(spaces) == QLatin1Char(' ')) ++spaces;
    cur.movePosition(QTextCursor::StartOfBlock);
    cur.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, spaces);
    cur.removeSelectedText();
    return true;
}

void FeltEditor::keyPressEvent(QKeyEvent *event) {
    if ((event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) && handleListReturnKey()) return;
    if (event->key() == Qt::Key_Tab && handleTabKey(false)) return;
    if (event->key() == Qt::Key_Backtab && handleTabKey(true)) return;
    QPlainTextEdit::keyPressEvent(event);
}
