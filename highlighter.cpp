#include "highlighter.h"
#include "themeregistry.h"
#include <QColor>
#include <QFont>
#include <QTextBlock>
#include <QTextDocument>

namespace {

void rehighlightPreservingCleanState(QSyntaxHighlighter *highlighter, QTextDocument *doc) {
    if (!doc) {
        return;
    }
    const bool wasModified = doc->isModified();
    highlighter->rehighlight();
    if (!wasModified) {
        doc->setModified(false);
    }
}

void rehighlightBlockPreservingCleanState(QSyntaxHighlighter *highlighter,
                                          QTextDocument *doc,
                                          const QTextBlock &block) {
    if (!doc || !block.isValid()) {
        return;
    }
    const bool wasModified = doc->isModified();
    highlighter->rehighlightBlock(block);
    if (!wasModified) {
        doc->setModified(false);
    }
}

} // namespace

FeltHighlighter::FeltHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
    , m_palette(ThemeRegistry::obsidianVoid())
{
    initializeFormats();
    buildRules();
}

void FeltHighlighter::setTheme(const ThemePalette &palette) {
    m_palette = palette;
    initializeFormats();
    buildRules();
    rehighlightPreservingCleanState(this, document());
}

ThemePalette FeltHighlighter::theme() const {
    return m_palette;
}

void FeltHighlighter::setFocusMode(bool enabled) {
    if (m_focusMode == enabled) {
        return;
    }
    m_focusMode = enabled;
    rehighlightPreservingCleanState(this, document());
}

bool FeltHighlighter::isFocusMode() const {
    return m_focusMode;
}

void FeltHighlighter::setCurrentBlockNumber(int blockNumber) {
    if (m_currentBlockNumber == blockNumber) {
        return;
    }
    const int oldBlock = m_currentBlockNumber;
    m_currentBlockNumber = blockNumber;
    if (!m_focusMode || !document()) {
        return;
    }
    rehighlightBlockPreservingCleanState(this, document(), document()->findBlockByNumber(oldBlock));
    rehighlightBlockPreservingCleanState(this, document(), document()->findBlockByNumber(m_currentBlockNumber));
}

int FeltHighlighter::currentBlockNumber() const {
    return m_currentBlockNumber;
}

void FeltHighlighter::initializeFormats() {
    m_normalFormat.setForeground(m_palette.textMain());
    m_dimmedFormat.setForeground(m_palette.textDimmed());

    m_header1Format.setForeground(m_palette.textMain());
    m_header1Format.setFontWeight(QFont::Bold);

    m_header2Format.setForeground(m_palette.textMain());
    m_header2Format.setFontWeight(QFont::Bold);

    m_header3Format.setForeground(m_palette.textMain());
    m_header3Format.setFontWeight(QFont::DemiBold);

    m_boldFormat.setForeground(m_palette.textMain());
    m_boldFormat.setFontWeight(QFont::Bold);

    m_italicFormat.setForeground(m_palette.textMain());
    m_italicFormat.setFontItalic(true);

    m_codeFormat.setForeground(m_palette.codeText());
    m_codeFormat.setBackground(m_palette.codeBg());

    m_quoteFormat.setForeground(m_palette.quoteText());
    m_quoteFormat.setFontItalic(true);

    m_listFormat.setForeground(m_palette.accent());
    m_listFormat.setFontWeight(QFont::Bold);

    m_breakFormat.setForeground(m_palette.markdownPrefix());
}

void FeltHighlighter::buildRules() {
    m_rules.clear();

    m_rules.push_back({QRegularExpression(QStringLiteral("^#\\s+.*$")), m_header1Format});
    m_rules.push_back({QRegularExpression(QStringLiteral("^##\\s+.*$")), m_header2Format});
    m_rules.push_back({QRegularExpression(QStringLiteral("^###\\s+.*$")), m_header3Format});
    m_rules.push_back({QRegularExpression(QStringLiteral("^>\\s+.*$")), m_quoteFormat});
    m_rules.push_back({QRegularExpression(QStringLiteral("^(\\s*[-*+]|\\s*\\d+\\.)\\s")), m_listFormat});
    m_rules.push_back({QRegularExpression(QStringLiteral("^(\\s*---+\\s*|\\s*\\*\\*\\*+\\s*)$")), m_breakFormat});
    m_rules.push_back({QRegularExpression(QStringLiteral("`[^`]+`")), m_codeFormat});
    m_rules.push_back({QRegularExpression(QStringLiteral("(\\*\\*|__)(.*?)\\1")), m_boldFormat});
    m_rules.push_back({QRegularExpression(QStringLiteral("(?<!\\*)\\*(?!\\*)(.+?)(?<!\\*)\\*(?!\\*)")), m_italicFormat});
    m_rules.push_back({QRegularExpression(QStringLiteral("(?<!_)_(?!_)(.+?)(?<!_)_(?!_)")), m_italicFormat});
}

void FeltHighlighter::highlightBlock(const QString &text) {
    const int blockIdx = currentBlock().blockNumber();
    const bool isCurrentBlock = (blockIdx == m_currentBlockNumber);

    if (m_focusMode && !isCurrentBlock) {
        setFormat(0, text.length(), m_dimmedFormat);
        return;
    }

    setFormat(0, text.length(), m_normalFormat);

    for (const auto &rule : m_rules) {
        QRegularExpressionMatchIterator it = rule.pattern.globalMatch(text);
        while (it.hasNext()) {
            QRegularExpressionMatch match = it.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
}
