#pragma once

#include <QPlainTextEdit>
#include <QStringList>
#include "highlighter.h"
#include "measure.h"
#include "theme.h"

class FeltEditor : public QPlainTextEdit {
    Q_OBJECT

public:
    explicit FeltEditor(QWidget *parent = nullptr);

    void applyTheme(const ThemePalette &palette);
    ThemePalette currentTheme() const;

    void setTypography(const QString &family, int pointSize);
    void setColumnWidth(int width);
    int maxColumnWidth() const { return m_measure.columnWidth(); }
    void setCaretWidth(int pixels);

    void setFocusMode(bool enabled);
    bool isFocusMode() const;
    void toggleFocusMode();

    void setTypewriterMode(bool enabled);
    bool isTypewriterMode() const;
    void toggleTypewriterMode();

    FeltHighlighter* highlighter() const { return m_highlighter; }

    static QFont resolveMonospaceFont(const QStringList &installedFamilies,
                                      const QStringList &priorityOrder,
                                      int pointSize = 16);

signals:
    void statsChanged();
    void focusModeToggled(bool enabled);
    void typewriterModeToggled(bool enabled);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void onCursorPositionChanged();
    void onTextChanged();

private:
    void updateMargins();
    void applyLineSpacingToAllBlocks();
    bool handleListReturnKey();
    bool handleTabKey(bool unindent);

    FeltHighlighter *m_highlighter = nullptr;
    DocumentMeasure m_measure;
    bool m_typewriterMode = true;
    bool m_applyingLineSpacing = false;
};
