#include "highlighter.h"
#include <QColor>
#include <QFont>
#include <QTextBlock>
#include <QTextDocument>

FeltHighlighter::FeltHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    initializeFormats();
    buildRules();
}

void FeltHighlighter::setFocusMode(bool enabled) {
    if (m_focusMode != enabled) {
        m_focusMode = enabled;
        rehighlight();
    }
}

bool FeltHighlighter::isFocusMode() const {
    return m_focusMode;
}

void FeltHighlighter::setCurrentBlockNumber(int blockNumber) {
    if (m_currentBlockNumber != blockNumber) {
        int oldBlock = m_currentBlockNumber;
        m_currentBlockNumber = blockNumber;
        if (m_focusMode && document()) {
            QTextBlock prev = document()->findBlockByNumber(oldBlock);
            if (prev.isValid()) {
                rehighlightBlock(prev);
            }
            QTextBlock curr = document()->findBlockByNumber(m_currentBlockNumber);
            if (curr.isValid()) {
                rehighlightBlock(curr);
            }
        }
    }
}

int FeltHighlighter::currentBlockNumber() const {
    return m_currentBlockNumber;
}

void FeltHighlighter::initializeFormats() {
    // Active text: warm bone white (#F0F0F0) matching felt_ui.jpg
    m_normalFormat.setForeground(QColor(0xf0, 0xf0, 0xf0));

    // Dimmed text: soft dark slate (#383838) matching felt_ui.jpg
    m_dimmedFormat.setForeground(QColor(0x38, 0x38, 0x38));

    // H1: pure white (#FFFFFF), Bold
    m_header1Format.setForeground(QColor(0xff, 0xff, 0xff));
    m_header1Format.setFontWeight(QFont::Bold);

    // H2: crisp off-white (#F5F5F5), Bold
    m_header2Format.setForeground(QColor(0xf5, 0xf5, 0xf5));
    m_header2Format.setFontWeight(QFont::Bold);

    // H3: soft white (#E5E5E5), DemiBold
    m_header3Format.setForeground(QColor(0xe5, 0xe5, 0xe5));
    m_header3Format.setFontWeight(QFont::DemiBold);

    // Bold: #FFFFFF, Bold
    m_boldFormat.setForeground(QColor(0xff, 0xff, 0xff));
    m_boldFormat.setFontWeight(QFont::Bold);

    // Italic: #D0D0D0, Italic
    m_italicFormat.setForeground(QColor(0xd0, 0xd0, 0xd0));
    m_italicFormat.setFontItalic(true);

    // Code: Sky blue on dark background
    m_codeFormat.setForeground(QColor(0x60, 0xa5, 0xfa));
    m_codeFormat.setBackground(QColor(0x18, 0x18, 0x18));

    // Blockquote: Muted grey (#9CA3AF), Italic
    m_quoteFormat.setForeground(QColor(0x9c, 0xa3, 0xaf));
    m_quoteFormat.setFontItalic(true);

    // List marker: Electric cyan-blue bullet (#3B82F6)
    m_listFormat.setForeground(QColor(0x3b, 0x82, 0xf6));
    m_listFormat.setFontWeight(QFont::Bold);

    // Scene break: Faint horizontal pause (#4A5568)
    m_breakFormat.setForeground(QColor(0x4a, 0x55, 0x68));
}

void FeltHighlighter::buildRules() {
    m_rules.clear();

    // H1: # Title
    HighlightingRule h1Rule;
    h1Rule.pattern = QRegularExpression(QStringLiteral("^#\\s+.*$"));
    h1Rule.format = m_header1Format;
    m_rules.push_back(h1Rule);

    // H2: ## Subtitle
    HighlightingRule h2Rule;
    h2Rule.pattern = QRegularExpression(QStringLiteral("^##\\s+.*$"));
    h2Rule.format = m_header2Format;
    m_rules.push_back(h2Rule);

    // H3: ### Section
    HighlightingRule h3Rule;
    h3Rule.pattern = QRegularExpression(QStringLiteral("^###\\s+.*$"));
    h3Rule.format = m_header3Format;
    m_rules.push_back(h3Rule);

    // Blockquote: > quote
    HighlightingRule quoteRule;
    quoteRule.pattern = QRegularExpression(QStringLiteral("^>\\s+.*$"));
    quoteRule.format = m_quoteFormat;
    m_rules.push_back(quoteRule);

    // List item: - item, * item, 1. item
    HighlightingRule listRule;
    listRule.pattern = QRegularExpression(QStringLiteral("^(\\s*[-*+]|\\s*\\d+\\.)\\s"));
    listRule.format = m_listFormat;
    m_rules.push_back(listRule);

    // Scene break: --- or ***
    HighlightingRule breakRule;
    breakRule.pattern = QRegularExpression(QStringLiteral("^(\\s*---+\\s*|\\s*\\*\\*\\*+\\s*)$"));
    breakRule.format = m_breakFormat;
    m_rules.push_back(breakRule);

    // Inline code: `code`
    HighlightingRule codeRule;
    codeRule.pattern = QRegularExpression(QStringLiteral("`[^`]+`"));
    codeRule.format = m_codeFormat;
    m_rules.push_back(codeRule);

    // Bold: **bold** or __bold__
    HighlightingRule boldRule;
    boldRule.pattern = QRegularExpression(QStringLiteral("(\\*\\*|__)(.*?)\\1"));
    boldRule.format = m_boldFormat;
    m_rules.push_back(boldRule);

    // Italic: *italic* (with symmetric negative lookarounds)
    HighlightingRule italicStarRule;
    italicStarRule.pattern = QRegularExpression(QStringLiteral("(?<!\\*)\\*(?!\\*)(.+?)(?<!\\*)\\*(?!\\*)"));
    italicStarRule.format = m_italicFormat;
    m_rules.push_back(italicStarRule);

    // Italic: _italic_ (with symmetric negative lookarounds)
    HighlightingRule italicUnderRule;
    italicUnderRule.pattern = QRegularExpression(QStringLiteral("(?<!_)_(?!_)(.+?)(?<!_)_(?!_)"));
    italicUnderRule.format = m_italicFormat;
    m_rules.push_back(italicUnderRule);
}

void FeltHighlighter::highlightBlock(const QString &text) {
    const int blockIdx = currentBlock().blockNumber();
    const bool isCurrentBlock = (blockIdx == m_currentBlockNumber);

    if (m_focusMode && !isCurrentBlock) {
        // Dim the entire non-active block
        setFormat(0, text.length(), m_dimmedFormat);
        return;
    }

    // Baseline format for active or un-dimmed block
    setFormat(0, text.length(), m_normalFormat);

    // Apply syntax rules
    for (const auto &rule : m_rules) {
        QRegularExpressionMatchIterator it = rule.pattern.globalMatch(text);
        while (it.hasNext()) {
            QRegularExpressionMatch match = it.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
}
