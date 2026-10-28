#include "zinsser.h"
#include "../measure.h"
#include "../editor.h"
#include "../mainwindow.h"
#include "../documenthud.h"
#include "../menubuilder.h"

void runCoreArchitectureSuites() {
    describe("Document Measure and Viewport Margin Calculus", [] {
        it("centers the writing column symmetrically when window width exceeds measure", [] {
            const DocumentMeasure measure(720);
            const ViewportMargins margins = measure.computeMargins(1120);
            QCOMPARE(margins.left, 200);
            QCOMPARE(margins.right, 200);
        });

        it("clamps side margins to forty pixels when window is narrower than measure", [] {
            const DocumentMeasure measure(720);
            const ViewportMargins margins = measure.computeMargins(680);
            QCOMPARE(margins.left, 40);
            QCOMPARE(margins.right, 40);
        });

        it("clamps configured column width within five hundred twenty and nine hundred sixty pixels", [] {
            QCOMPARE(DocumentMeasure(300).columnWidth(), 520);
            QCOMPARE(DocumentMeasure(720).columnWidth(), 720);
            QCOMPARE(DocumentMeasure(1400).columnWidth(), 960);
        });

        it("preserves eighty pixel top and bottom breathing margins across all window sizes", [] {
            const DocumentMeasure measure(720);
            for (int w : {60, 480, 800, 1440}) {
                const ViewportMargins margins = measure.computeMargins(w);
                QCOMPARE(margins.top, 80);
                QCOMPARE(margins.bottom, 80);
            }
        });

        it("prevents negative viewport geometry when window width is smaller than minimum margins", [] {
            const DocumentMeasure measure(720);
            const ViewportMargins tiny = measure.computeMargins(60);
            QVERIFY(tiny.left >= 0);
            QVERIFY(60 - (tiny.left + tiny.right) > 0);

            const ViewportMargins zero = measure.computeMargins(0);
            QCOMPARE(zero.left, 0);
            QCOMPARE(zero.right, 0);
        });
    });

    describe("Declarative Menu Actions and Font Priority Resolution", [] {
        it("selects first available monospace family from ordered priority list without else branches", [] {
            const QStringList installed = {QStringLiteral("Arial"), QStringLiteral("Monaco"), QStringLiteral("Courier New")};
            const QStringList priority = {QStringLiteral("Menlo"), QStringLiteral("Monaco"), QStringLiteral("Courier New")};
            const QFont resolved = FeltEditor::resolveMonospaceFont(installed, priority, 18);
            QCOMPARE(resolved.family(), QStringLiteral("Monaco"));
            QCOMPARE(resolved.pointSize(), 18);
        });

        it("falls back to system fixed font when no priority family is installed", [] {
            const QStringList installed = {QStringLiteral("Helvetica")};
            const QStringList priority = {QStringLiteral("NonExistentMonoA"), QStringLiteral("NonExistentMonoB")};
            const QFont fallback = FeltEditor::resolveMonospaceFont(installed, priority, 15);
            QCOMPARE(fallback.pointSize(), 15);
            QVERIFY(!fallback.family().isEmpty());
        });

        it("binds file, view, focus, typewriter, and preferences shortcuts declaratively", [] {
            MainWindow window;
            const WindowMenuActions actions = window.menuActions();
            QVERIFY(actions.newAction != nullptr);
            QVERIFY(actions.openAction != nullptr);
            QVERIFY(actions.saveAction != nullptr);
            QVERIFY(actions.saveAsAction != nullptr);
            QVERIFY(actions.preferencesAction != nullptr);
            QVERIFY(actions.quitAction != nullptr);
            QCOMPARE(actions.focusAction->shortcut(), QKeySequence(Qt::CTRL | Qt::Key_D));
            QCOMPARE(actions.typewriterAction->shortcut(), QKeySequence(Qt::CTRL | Qt::Key_T));
        });

        it("synchronizes checkable menu actions with editor focus and typewriter states", [] {
            MainWindow window;
            const WindowMenuActions actions = window.menuActions();
            QVERIFY(actions.focusAction->isChecked());
            QVERIFY(actions.typewriterAction->isChecked());

            window.editor()->setFocusMode(false);
            QVERIFY(!actions.focusAction->isChecked());

            window.editor()->setTypewriterMode(false);
            QVERIFY(!actions.typewriterAction->isChecked());
            QVERIFY(!window.hud()->summaryText().isEmpty());
        });
    });
}
