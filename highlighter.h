#pragma once

#include "theme.h"
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>
#include <vector>

class FeltHighlighter : public QSyntaxHighlighter {
    Q_OBJECT

public:
    explicit FeltHighlighter(QTextDocument *parent = nullptr);

    void setTheme(const ThemePalette &palette);
    ThemePalette theme() const;

    void setFocusMode(bool enabled);
    bool isFocusMode() const;

    void setCurrentBlockNumber(int blockNumber);
    int currentBlockNumber() const;

    QTextCharFormat normalFormat() const { return m_normalFormat; }
    QTextCharFormat dimmedFormat() const { return m_dimmedFormat; }
    QTextCharFormat header1Format() const { return m_header1Format; }
    QTextCharFormat header2Format() const { return m_header2Format; }
    QTextCharFormat header3Format() const { return m_header3Format; }
    QTextCharFormat boldFormat() const { return m_boldFormat; }
    QTextCharFormat italicFormat() const { return m_italicFormat; }
    QTextCharFormat codeFormat() const { return m_codeFormat; }
    QTextCharFormat quoteFormat() const { return m_quoteFormat; }
    QTextCharFormat listFormat() const { return m_listFormat; }
    QTextCharFormat breakFormat() const { return m_breakFormat; }

protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightingRule {
        QRegularExpression pattern;
        QTextCharFormat format;
    };

    void initializeFormats();
    void buildRules();

    ThemePalette m_palette;
    bool m_focusMode = false;
    int m_currentBlockNumber = 0;

    std::vector<HighlightingRule> m_rules;

    QTextCharFormat m_normalFormat;
    QTextCharFormat m_dimmedFormat;
    QTextCharFormat m_header1Format;
    QTextCharFormat m_header2Format;
    QTextCharFormat m_header3Format;
    QTextCharFormat m_boldFormat;
    QTextCharFormat m_italicFormat;
    QTextCharFormat m_codeFormat;
    QTextCharFormat m_quoteFormat;
    QTextCharFormat m_listFormat;
    QTextCharFormat m_breakFormat;
};
