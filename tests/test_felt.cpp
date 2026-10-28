#include "zinsser.h"
#include "../theme.h"
#include "../themeregistry.h"
#include "../stats.h"
#include "../highlighter.h"
#include "../editor.h"
#include <QTextDocument>
#include <QTextBlock>
#include <QTextLayout>

void runCoreArchitectureSuites();

class TestFelt : public QObject {
    Q_OBJECT

private slots:
    void verifies_theme_registry_and_contrast_invariants();
    void verifies_document_stats_and_hud_formatting();
    void verifies_highlighter_and_focus_mode_invariants();
    void verifies_core_architecture_and_measure() { runCoreArchitectureSuites(); }
};

void TestFelt::verifies_theme_registry_and_contrast_invariants() {
    describe("ThemeRegistry and WCAG Contrast Invariants", [] {
        it("resolves obsidian, parchment, slate, and midnight palettes by key", [] {
            QCOMPARE(ThemeRegistry::resolve(QStringLiteral("obsidian_void")).displayName(), QStringLiteral("Obsidian Void"));
            QCOMPARE(ThemeRegistry::resolve(QStringLiteral("warm_parchment")).displayName(), QStringLiteral("Warm Parchment"));
            QCOMPARE(ThemeRegistry::resolve(QStringLiteral("slate_concrete")).displayName(), QStringLiteral("Slate Concrete"));
            QCOMPARE(ThemeRegistry::resolve(QStringLiteral("midnight_indigo")).displayName(), QStringLiteral("Midnight Indigo"));
            QCOMPARE(ThemeRegistry::allPalettes().size(), static_cast<size_t>(4));
        });

        it("falls back to obsidian void when queried with an unknown key", [] {
            const ThemePalette fallback = ThemeRegistry::resolve(QStringLiteral("nonexistent_theme"));
            const ThemePalette obsidian = ThemeRegistry::obsidianVoid();
            QCOMPARE(fallback.key(), obsidian.key());
            QCOMPARE(fallback.bgOuter(), obsidian.bgOuter());
            QCOMPARE(fallback.bgVignetteCenter(), obsidian.bgVignetteCenter());
            QCOMPARE(fallback.textMain(), obsidian.textMain());
            QCOMPARE(fallback.textDimmed(), obsidian.textDimmed());
            QCOMPARE(fallback.accent(), obsidian.accent());
            QCOMPARE(fallback.selectionBg(), obsidian.selectionBg());
            QCOMPARE(fallback.selectionText(), obsidian.selectionText());
            QCOMPARE(fallback.markdownPrefix(), obsidian.markdownPrefix());
            QCOMPARE(fallback.hudText(), obsidian.hudText());
            QCOMPARE(fallback.codeBg(), obsidian.codeBg());
            QCOMPARE(fallback.codeText(), obsidian.codeText());
            QCOMPARE(fallback.quoteText(), obsidian.quoteText());
        });

        it("enforces wcag aaa contrast ratio above seven to one between body text and vignette center", [] {
            for (const ThemePalette &palette : ThemeRegistry::allPalettes()) {
                QVERIFY2(palette.bodyContrastRatio() >= 7.0, palette.key().toUtf8().constData());
            }
        });

        it("enforces dimmed text contrast between two and four to one against vignette center for focus mode", [] {
            for (const ThemePalette &palette : ThemeRegistry::allPalettes()) {
                const double ratio = palette.dimmedContrastRatio();
                QVERIFY2(ratio >= 2.0 && ratio <= 4.0, palette.key().toUtf8().constData());
            }
            const double obsidianDimmed = ThemeRegistry::obsidianVoid().dimmedContrastRatio();
            QVERIFY(obsidianDimmed > 2.3 && obsidianDimmed < 2.6);
        });

        it("enforces high-visibility contrast for active caret and text selection across all palettes", [] {
            for (const ThemePalette &palette : ThemeRegistry::allPalettes()) {
                QVERIFY2(palette.caretContrastRatio() >= 3.0, palette.key().toUtf8().constData());
                QVERIFY2(palette.selectionContrastRatio() >= 4.5, palette.key().toUtf8().constData());
            }
        });
    });
}

void TestFelt::verifies_document_stats_and_hud_formatting() {
    describe("DocumentStats word and reading-time counter", [] {
        it("returns zero words and zero minutes for empty or whitespace-only documents", [] {
            const DocumentStats empty = DocumentStats::compute(QString());
            QCOMPARE(empty.wordCount, 0);
            QCOMPARE(empty.charCount, 0);
            QCOMPARE(empty.readingTimeMinutes, 0);

            const DocumentStats spacesOnly = DocumentStats::compute(QStringLiteral("   \n\t  "));
            QCOMPARE(spacesOnly.wordCount, 0);
            QCOMPARE(spacesOnly.charCount, 7);
            QCOMPARE(spacesOnly.charNoSpacesCount, 0);
            QCOMPARE(spacesOnly.readingTimeMinutes, 0);
        });

        it("computes word, character, and non-space counts across multi-paragraph prose", [] {
            const QString prose = QStringLiteral("In the quiet, thoughts settle.\n\nClarity emerges.");
            const DocumentStats stats = DocumentStats::compute(prose);
            QCOMPARE(stats.wordCount, 7);
            QCOMPARE(stats.charCount, 48);
            QCOMPARE(stats.charNoSpacesCount, 41);
        });

        it("counts whitespace-delimited words accurately across multibyte utf8 scripts", [] {
            const QString tamilAndAccented = QString::fromUtf8("அறம் செய விரும்பு ஆறுவது சினம் café naïve");
            const DocumentStats stats = DocumentStats::compute(tamilAndAccented);
            QCOMPARE(stats.wordCount, 7);
            QVERIFY(stats.charNoSpacesCount > 20);
        });

        it("rounds reading time at two hundred words per minute with one minute floor for non-empty text", [] {
            QString fiftyWords;
            for (int i = 0; i < 50; ++i) fiftyWords += QStringLiteral("word ");
            QCOMPARE(DocumentStats::compute(fiftyWords).readingTimeMinutes, 1);

            QString fourHundredWords;
            for (int i = 0; i < 400; ++i) fourHundredWords += QStringLiteral("prose ");
            QCOMPARE(DocumentStats::compute(fourHundredWords).readingTimeMinutes, 2);
            QCOMPARE(DocumentStats::formatReadingTime(0), QStringLiteral("0 min read"));
            QCOMPARE(DocumentStats::formatReadingTime(1), QStringLiteral("1 min read"));
            QCOMPARE(DocumentStats::formatReadingTime(2), QStringLiteral("2 min read"));
        });

        it("formats summary string with middle-dot separator for the bottom hud", [] {
            QCOMPARE(DocumentStats::formatSummary(DocumentStats::compute(QString())), QStringLiteral("0 words · 0 min read"));
            QCOMPARE(DocumentStats::formatSummary(DocumentStats::compute(QStringLiteral("Solitude"))), QStringLiteral("1 word · 1 min read"));
            QCOMPARE(DocumentStats::formatSummary(DocumentStats::compute(QStringLiteral("Two words"))), QStringLiteral("2 words · 1 min read"));
        });
    });
}

void TestFelt::verifies_highlighter_and_focus_mode_invariants() {
    describe("FeltHighlighter and Focus Mode Invariants", [] {
        it("highlights heading, blockquote, and inline emphasis spans in situ", [] {
            QTextDocument doc;
            doc.setPlainText(QStringLiteral("# Chapter One\n> Quiet monologue\nThis is **bold** and *italic* text."));
            FeltHighlighter highlighter(&doc);
            highlighter.rehighlight();
            const auto h1Formats = doc.findBlockByNumber(0).layout()->formats();
            QVERIFY(!h1Formats.isEmpty());
            QCOMPARE(h1Formats.last().format.fontWeight(), QFont::Bold);
            const auto quoteFormats = doc.findBlockByNumber(1).layout()->formats();
            QVERIFY(!quoteFormats.isEmpty());
            QVERIFY(quoteFormats.last().format.fontItalic());
            const auto inlineFormats = doc.findBlockByNumber(2).layout()->formats();
            QVERIFY(inlineFormats.size() >= 2);
        });

        it("dims inactive blocks to palette dimmed color when focus mode is enabled", [] {
            QTextDocument doc;
            doc.setPlainText(QStringLiteral("First paragraph.\n\nSecond paragraph."));
            FeltHighlighter highlighter(&doc);
            highlighter.setFocusMode(true);
            highlighter.setCurrentBlockNumber(2);

            const auto formats0 = doc.findBlockByNumber(0).layout()->formats();
            QVERIFY(!formats0.isEmpty());
            QCOMPARE(formats0.first().format.foreground().color(), ThemeRegistry::obsidianVoid().textDimmed());
        });

        it("illuminates active block in palette main text color during focus mode", [] {
            QTextDocument doc;
            doc.setPlainText(QStringLiteral("First paragraph.\n\nSecond paragraph."));
            FeltHighlighter highlighter(&doc);
            highlighter.setFocusMode(true);
            highlighter.setCurrentBlockNumber(2);

            const auto formats2 = doc.findBlockByNumber(2).layout()->formats();
            QVERIFY(!formats2.isEmpty());
            QCOMPARE(formats2.first().format.foreground().color(), ThemeRegistry::obsidianVoid().textMain());
        });

        it("preserves unmodified document clean state across theme and focus mode transitions", [] {
            QTextDocument doc;
            doc.setPlainText(QStringLiteral("Unmodified manuscript paragraph.\n\nSecond paragraph."));
            FeltHighlighter highlighter(&doc);
            doc.setModified(false);

            highlighter.setFocusMode(true);
            QVERIFY(!doc.isModified());
            highlighter.setCurrentBlockNumber(2);
            QVERIFY(!doc.isModified());
            highlighter.setTheme(ThemeRegistry::warmParchment());
            QVERIFY(!doc.isModified());
            QCOMPARE(highlighter.theme().key(), QStringLiteral("warm_parchment"));
        });

        it("activates editor focus genuinely and handles mouse clicks under offscreen platform", [] {
            FeltEditor editor;
            editor.resize(1000, 800);
            editor.setPlainText(QStringLiteral("First line.\nSecond line.\nThird line."));
            editor.show();
            editor.activateWindow();
            QVERIFY(QTest::qWaitForWindowActive(&editor));

            QTest::mouseClick(editor.viewport(), Qt::LeftButton, Qt::NoModifier, QPoint(50, 50));
            QVERIFY(editor.hasFocus());
            QTest::mouseClick(&editor, Qt::LeftButton, Qt::NoModifier, QPoint(10, 10));
            QCOMPARE(editor.toPlainText().split('\n').size(), 3);
        });
    });
}

QTEST_MAIN(TestFelt)
#include "test_felt.moc"
