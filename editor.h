#pragma once

#include <QPlainTextEdit>
#include "highlighter.h"

class FeltEditor : public QPlainTextEdit {
    Q_OBJECT

public:
    explicit FeltEditor(QWidget *parent = nullptr);

    void setFocusMode(bool enabled);
    bool isFocusMode() const;
    void toggleFocusMode();

    void setTypewriterMode(bool enabled);
    bool isTypewriterMode() const;
    void toggleTypewriterMode();

    FeltHighlighter* highlighter() const { return m_highlighter; }
    int maxColumnWidth() const { return m_maxColumnWidth; }

signals:
    void statsChanged();
    void focusModeToggled(bool enabled);
    void typewriterModeToggled(bool enabled);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private slots:
    void onCursorPositionChanged();
    void onTextChanged();

private:
    void updateMargins();
    void setupTypography();
    void forwardMouseEventToViewport(QMouseEvent *event);

    FeltHighlighter *m_highlighter = nullptr;
    bool m_typewriterMode = true;
    int m_maxColumnWidth = 720;
};
