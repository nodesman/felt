#pragma once

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>
#include <vector>

class FeltHighlighter : public QSyntaxHighlighter {
    Q_OBJECT

public:
    explicit FeltHighlighter(QTextDocument *parent = nullptr);

    void setFocusMode(bool enabled);
    bool isFocusMode() const;

    void setCurrentBlockNumber(int blockNumber);
    int currentBlockNumber() const;

protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightingRule {
        QRegularExpression pattern;
        QTextCharFormat format;
    };

    void initializeFormats();
    void buildRules();

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
};
