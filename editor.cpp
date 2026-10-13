#include "editor.h"
#include <QFontDatabase>
#include <QScrollBar>
#include <QResizeEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QTextBlock>
#include <QCoreApplication>
#include <algorithm>

FeltEditor::FeltEditor(QWidget *parent)
    : QPlainTextEdit(parent)
{
    setFrameStyle(QFrame::NoFrame);
    setLineWrapMode(QPlainTextEdit::WidgetWidth);

    setupTypography();

    // Hide scrollbars for an undisturbed aesthetic
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Native typewriter centering
    setCenterOnScroll(m_typewriterMode);

    // Color palette & electric cyan-blue caret
    setCursorWidth(2);
    QPalette p = palette();
    p.setColor(QPalette::Base, QColor(0x08, 0x08, 0x08));
    p.setColor(QPalette::Text, QColor(0xf0, 0xf0, 0xf0));
    p.setColor(QPalette::Highlight, QColor(0x1e, 0x3a, 0x8a));
    p.setColor(QPalette::HighlightedText, QColor(0xff, 0xff, 0xff));
    setPalette(p);

    setStyleSheet(QStringLiteral(
        "QPlainTextEdit { "
        "  background-color: #080808; "
        "  color: #f0f0f0; "
        "  selection-background-color: #1e3a8a; "
        "  selection-color: #ffffff; "
        "  qproperty-cursorWidth: 2; "
        "  border: none; "
        "}"
    ));

    // Attach in-situ syntax highlighter
    m_highlighter = new FeltHighlighter(document());

    connect(this, &QPlainTextEdit::cursorPositionChanged,
            this, &FeltEditor::onCursorPositionChanged);
    connect(this, &QPlainTextEdit::textChanged,
            this, &FeltEditor::onTextChanged);
}

void FeltEditor::setupTypography() {
    QFont font("Menlo", 16);
    const QStringList families = QFontDatabase::families();
    if (families.contains("Menlo")) {
        font.setFamily("Menlo");
    } else if (families.contains("Monaco")) {
        font.setFamily("Monaco");
    } else if (families.contains("SF Mono")) {
        font.setFamily("SF Mono");
    } else if (families.contains("Courier New")) {
        font.setFamily("Courier New");
    } else {
        font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    }
    font.setPointSize(16);
    font.setLetterSpacing(QFont::PercentageSpacing, 102);
    setFont(font);
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
    if (m_typewriterMode != enabled) {
        m_typewriterMode = enabled;
        setCenterOnScroll(m_typewriterMode);
        if (m_typewriterMode) {
            centerCursor();
        }
        emit typewriterModeToggled(enabled);
    }
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
    const int currentWidth = width();
    int sideMargin = 40;
    if (currentWidth > m_maxColumnWidth + 80) {
        sideMargin = (currentWidth - m_maxColumnWidth) / 2;
    }
    // Generous top and bottom breathing room (80px)
    setViewportMargins(sideMargin, 80, sideMargin, 80);
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
    // Tab inserts 4 spaces instead of moving focus
    if (event->key() == Qt::Key_Tab) {
        insertPlainText("    ");
        return;
    }
    QPlainTextEdit::keyPressEvent(event);
}

void FeltEditor::forwardMouseEventToViewport(QMouseEvent *event) {
    if (!viewport()) return;

    QPointF localPos = event->position();
    int clampedX = std::clamp(static_cast<int>(localPos.x() - viewport()->x()), 0, viewport()->width() - 1);
    int clampedY = std::clamp(static_cast<int>(localPos.y() - viewport()->y()), 0, viewport()->height() - 1);

    QMouseEvent mappedEvent(
        event->type(),
        QPointF(clampedX, clampedY),
        event->globalPosition(),
        event->button(),
        event->buttons(),
        event->modifiers()
    );
    QCoreApplication::sendEvent(viewport(), &mappedEvent);
}

void FeltEditor::mousePressEvent(QMouseEvent *event) {
    if (viewport() && !viewport()->geometry().contains(event->pos())) {
        forwardMouseEventToViewport(event);
        return;
    }
    QPlainTextEdit::mousePressEvent(event);
}

void FeltEditor::mouseMoveEvent(QMouseEvent *event) {
    if (viewport() && !viewport()->geometry().contains(event->pos())) {
        forwardMouseEventToViewport(event);
        return;
    }
    QPlainTextEdit::mouseMoveEvent(event);
}

void FeltEditor::mouseReleaseEvent(QMouseEvent *event) {
    if (viewport() && !viewport()->geometry().contains(event->pos())) {
        forwardMouseEventToViewport(event);
        return;
    }
    QPlainTextEdit::mouseReleaseEvent(event);
}
