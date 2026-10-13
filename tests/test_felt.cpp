#include <QtTest>
#include "../stats.h"
#include "../highlighter.h"
#include "../editor.h"
#include <QTextDocument>
#include <QTextBlock>
#include <QTextLayout>

class TestFelt : public QObject {
    Q_OBJECT

private slots:
    void computes_word_and_char_counts();
    void calculates_reading_time_accurately();
    void formats_summary_with_middle_dot();
    void highlights_heading_prefix_and_text();
    void highlights_blockquote_prefix_and_text();
    void highlights_bold_and_italic_in_situ();
    void dims_inactive_blocks_in_focus_mode();
    void illuminates_active_block_in_focus_mode();
    void centers_cursor_in_typewriter_mode();
};

void TestFelt::computes_word_and_char_counts() {
    const QString text = QStringLiteral("In the quiet, thoughts settle into natural shapes.");
    DocumentStats stats = DocumentStats::compute(text);

    QCOMPARE(stats.wordCount, 8);
    QCOMPARE(stats.charCount, 50);
    QCOMPARE(stats.charNoSpacesCount, 43);
}

void TestFelt::calculates_reading_time_accurately() {
    DocumentStats emptyStats = DocumentStats::compute(QString());
    QCOMPARE(emptyStats.readingTimeMinutes, 0);

    QString fiftyWords;
    for (int i = 0; i < 50; ++i) fiftyWords += QStringLiteral("word ");
    DocumentStats s50 = DocumentStats::compute(fiftyWords);
    QCOMPARE(s50.readingTimeMinutes, 1);

    QString fourHundredWords;
    for (int i = 0; i < 400; ++i) fourHundredWords += QStringLiteral("prose ");
    DocumentStats s400 = DocumentStats::compute(fourHundredWords);
    QCOMPARE(s400.readingTimeMinutes, 2);
}

void TestFelt::formats_summary_with_middle_dot() {
    DocumentStats emptyStats = DocumentStats::compute(QString());
    QCOMPARE(DocumentStats::formatSummary(emptyStats), QStringLiteral("0 words · 0 min read"));

    const QString sample = QStringLiteral("Writing here feels like breathing in an empty room.");
    DocumentStats stats = DocumentStats::compute(sample);
    QVERIFY(DocumentStats::formatSummary(stats).contains(QStringLiteral("·")));
    QCOMPARE(stats.wordCount, 9);
}

void TestFelt::highlights_heading_prefix_and_text() {
    QTextDocument doc;
    doc.setPlainText(QStringLiteral("# Chapter One: The Quiet Canvas"));
    FeltHighlighter highlighter(&doc);
    highlighter.rehighlight();

    QTextBlock block = doc.firstBlock();
    QTextLayout *layout = block.layout();
    QVERIFY(layout != nullptr);

    const auto formats = layout->formats();
    QVERIFY(!formats.isEmpty());
    QCOMPARE(formats.last().format.fontWeight(), QFont::Bold);
}

void TestFelt::highlights_blockquote_prefix_and_text() {
    QTextDocument doc;
    doc.setPlainText(QStringLiteral("> This is a quiet monologue."));
    FeltHighlighter highlighter(&doc);
    highlighter.rehighlight();

    QTextBlock block = doc.firstBlock();
    QTextLayout *layout = block.layout();
    QVERIFY(layout != nullptr);

    const auto formats = layout->formats();
    QVERIFY(!formats.isEmpty());
    QVERIFY(formats.last().format.fontItalic());
}

void TestFelt::highlights_bold_and_italic_in_situ() {
    QTextDocument doc;
    doc.setPlainText(QStringLiteral("This is **bold** and *italic* text."));
    FeltHighlighter highlighter(&doc);
    highlighter.rehighlight();

    QTextBlock block = doc.firstBlock();
    QTextLayout *layout = block.layout();
    QVERIFY(layout != nullptr);

    const auto formats = layout->formats();
    QVERIFY(formats.size() >= 2);

    bool foundBold = false;
    bool foundItalic = false;
    for (const auto &fmt : formats) {
        if (fmt.format.fontWeight() == QFont::Bold) foundBold = true;
        if (fmt.format.fontItalic()) foundItalic = true;
    }
    QVERIFY(foundBold);
    QVERIFY(foundItalic);
}

void TestFelt::dims_inactive_blocks_in_focus_mode() {
    QTextDocument doc;
    doc.setPlainText(QStringLiteral("First paragraph.\n\nSecond paragraph.\n\nThird paragraph."));
    FeltHighlighter highlighter(&doc);
    highlighter.setFocusMode(true);
    highlighter.setCurrentBlockNumber(2); // Cursor on second paragraph

    QTextBlock block0 = doc.findBlockByNumber(0);
    QTextLayout *layout0 = block0.layout();
    QVERIFY(layout0 != nullptr);

    const auto formats0 = layout0->formats();
    QVERIFY(!formats0.isEmpty());
    QCOMPARE(formats0.first().format.foreground().color(), QColor(0x38, 0x38, 0x38));
}

void TestFelt::illuminates_active_block_in_focus_mode() {
    QTextDocument doc;
    doc.setPlainText(QStringLiteral("First paragraph.\n\nSecond paragraph.\n\nThird paragraph."));
    FeltHighlighter highlighter(&doc);
    highlighter.setFocusMode(true);
    highlighter.setCurrentBlockNumber(2);

    QTextBlock block2 = doc.findBlockByNumber(2);
    QTextLayout *layout2 = block2.layout();
    QVERIFY(layout2 != nullptr);

    const auto formats2 = layout2->formats();
    QVERIFY(!formats2.isEmpty());
    QCOMPARE(formats2.first().format.foreground().color(), QColor(0xf0, 0xf0, 0xf0));
}

void TestFelt::centers_cursor_in_typewriter_mode() {
    FeltEditor editor;
    editor.setTypewriterMode(true);
    QVERIFY(editor.isTypewriterMode());
    QVERIFY(editor.centerOnScroll());

    editor.toggleTypewriterMode();
    QVERIFY(!editor.isTypewriterMode());
    QVERIFY(!editor.centerOnScroll());
}

QTEST_MAIN(TestFelt)
#include "test_felt.moc"
