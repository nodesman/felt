#include "zinsser.h"
#include "../preferences.h"
#include "../themeregistry.h"
#include "../editor.h"
#include "../mainwindow.h"
#include "../settingssheet.h"
#include <QTemporaryFile>

void runUiAndPreferencesSuites() {
    describe("Preferences State Machine and Storage Persistence", [] {
        it("roundtrips theme, typography, measure, and dynamics settings through storage", [] {
            QTemporaryFile tempIni;
            QVERIFY(tempIni.open());
            QSettings storage(tempIni.fileName(), QSettings::IniFormat);
            const QStringList installed = {QStringLiteral("Menlo"), QStringLiteral("Monaco")};

            Preferences prefs;
            prefs.setThemeKey(QStringLiteral("warm_parchment"));
            prefs.setFontFamily(QStringLiteral("Monaco"), installed);
            prefs.setFontSize(20);
            prefs.setColumnWidth(840);
            prefs.setCaretWidth(3);
            prefs.setFocusMode(false);
            prefs.setTypewriterMode(false);
            prefs.setHudVisible(false);
            prefs.saveTo(storage);

            const Preferences loaded = Preferences::loadFrom(storage, installed);
            QCOMPARE(loaded.themeKey(), QStringLiteral("warm_parchment"));
            QCOMPARE(loaded.palette().key(), QStringLiteral("warm_parchment"));
            QCOMPARE(loaded.fontFamily(), QStringLiteral("Monaco"));
            QCOMPARE(loaded.fontSize(), 20);
            QCOMPARE(loaded.columnWidth(), 840);
            QCOMPARE(loaded.caretWidth(), 3);
            QVERIFY(!loaded.focusMode());
            QVERIFY(!loaded.typewriterMode());
            QVERIFY(!loaded.hudVisible());
        });

        it("clamps out of range font sizes to thirteen through twenty six points", [] {
            Preferences prefs;
            prefs.setFontSize(6);
            QCOMPARE(prefs.fontSize(), 13);
            prefs.setFontSize(42);
            QCOMPARE(prefs.fontSize(), 26);
        });

        it("clamps out of range column widths to five hundred twenty through nine hundred sixty pixels", [] {
            Preferences prefs;
            prefs.setColumnWidth(200);
            QCOMPARE(prefs.columnWidth(), 520);
            prefs.setColumnWidth(1600);
            QCOMPARE(prefs.columnWidth(), 960);
            prefs.setCaretWidth(0);
            QCOMPARE(prefs.caretWidth(), 1);
            prefs.setCaretWidth(9);
            QCOMPARE(prefs.caretWidth(), 3);
        });

        it("falls back to priority monospace family when stored font name is unavailable on system", [] {
            Preferences prefs;
            prefs.setFontFamily(QStringLiteral("MissingFont"), {QStringLiteral("SF Mono")});
            QCOMPARE(prefs.fontFamily(), QStringLiteral("SF Mono"));
            prefs.setFontFamily(QString(), {QStringLiteral("Helvetica")});
            QCOMPARE(prefs.fontFamily(), QStringLiteral("Menlo"));
        });

        it("restores default obsidian configuration when storage keys are missing or corrupted", [] {
            QTemporaryFile tempIni;
            QVERIFY(tempIni.open());
            QSettings emptyStorage(tempIni.fileName(), QSettings::IniFormat);
            const Preferences defaults = Preferences::loadFrom(emptyStorage, {QStringLiteral("Menlo")});
            QCOMPARE(defaults.themeKey(), QStringLiteral("obsidian_void"));
            QCOMPARE(defaults.fontSize(), 16);
            QCOMPARE(defaults.columnWidth(), 720);
        });
    });

    describe("FeltEditor list continuation and indentation", [] {
        it("continues unordered hyphen, asterisk, and plus bullets on enter", [] {
            FeltEditor editor;
            editor.setPlainText(QStringLiteral("- First bullet"));
            QTextCursor cur = editor.textCursor();
            cur.movePosition(QTextCursor::End);
            editor.setTextCursor(cur);
            QTest::keyClick(&editor, Qt::Key_Return);
            QCOMPARE(editor.toPlainText(), QStringLiteral("- First bullet\n- "));
        });

        it("increments numbered list markers on enter after non-empty item", [] {
            FeltEditor editor;
            editor.setPlainText(QStringLiteral("1. First step"));
            QTextCursor cur = editor.textCursor();
            cur.movePosition(QTextCursor::End);
            editor.setTextCursor(cur);
            QTest::keyClick(&editor, Qt::Key_Return);
            QCOMPARE(editor.toPlainText(), QStringLiteral("1. First step\n2. "));
        });

        it("splits list item text cleanly at the cursor when enter is pressed mid-line", [] {
            FeltEditor editor;
            editor.setPlainText(QStringLiteral("- HelloWorld"));
            QTextCursor cur = editor.textCursor();
            cur.setPosition(7);
            editor.setTextCursor(cur);
            QTest::keyClick(&editor, Qt::Key_Return);
            QCOMPARE(editor.toPlainText(), QStringLiteral("- Hello\n- World"));
        });

        it("preserves leading indentation when continuing nested list items", [] {
            FeltEditor editor;
            editor.setPlainText(QStringLiteral("    * Nested bullet"));
            QTextCursor cur = editor.textCursor();
            cur.movePosition(QTextCursor::End);
            editor.setTextCursor(cur);
            QTest::keyClick(&editor, Qt::Key_Return);
            QCOMPARE(editor.toPlainText(), QStringLiteral("    * Nested bullet\n    * "));
        });

        it("clears empty list marker and restores plain paragraph on second enter", [] {
            FeltEditor editor;
            editor.setPlainText(QStringLiteral("- Item\n- "));
            QTextCursor cur = editor.textCursor();
            cur.movePosition(QTextCursor::End);
            editor.setTextCursor(cur);
            QTest::keyClick(&editor, Qt::Key_Return);
            QCOMPARE(editor.toPlainText(), QStringLiteral("- Item\n"));
        });

        it("indents active list item by four spaces when tab is pressed", [] {
            FeltEditor editor;
            editor.setPlainText(QStringLiteral("- Bullet"));
            QTest::keyClick(&editor, Qt::Key_Tab);
            QCOMPARE(editor.toPlainText(), QStringLiteral("    - Bullet"));
        });

        it("unindents nested list item by four spaces when shift-tab is pressed", [] {
            FeltEditor editor;
            editor.setPlainText(QStringLiteral("    - Bullet"));
            QTest::keyClick(&editor, Qt::Key_Backtab);
            QCOMPARE(editor.toPlainText(), QStringLiteral("- Bullet"));
        });

        it("reverts auto-inserted list marker and newline in a single undo step", [] {
            FeltEditor editor;
            editor.setPlainText(QStringLiteral("- Atomic item"));
            QTextCursor cur = editor.textCursor();
            cur.movePosition(QTextCursor::End);
            editor.setTextCursor(cur);
            QTest::keyClick(&editor, Qt::Key_Return);
            QCOMPARE(editor.toPlainText(), QStringLiteral("- Atomic item\n- "));
            editor.undo();
            QCOMPARE(editor.toPlainText(), QStringLiteral("- Atomic item"));
        });
    });
}
