#include "menubuilder.h"
#include "mainwindow.h"
#include "editor.h"
#include <QMenuBar>
#include <QMenu>
#include <QKeySequence>

namespace {

void toggleWindowFullScreen(MainWindow *window) {
    if (window->isFullScreen()) {
        window->showNormal();
        return;
    }
    window->showFullScreen();
}

} // namespace

WindowMenuActions MenuBuilder::populate(MainWindow *window) {
    WindowMenuActions actions;
    FeltEditor *editor = window->editor();

    QMenu *fileMenu = window->menuBar()->addMenu(QObject::tr("&File"));
    actions.newAction = fileMenu->addAction(QObject::tr("&New"), QKeySequence::New, window, &MainWindow::newFile);
    actions.openAction = fileMenu->addAction(QObject::tr("&Open..."), QKeySequence::Open, window, &MainWindow::openFile);
    actions.saveAction = fileMenu->addAction(QObject::tr("&Save"), QKeySequence::Save, window, &MainWindow::saveFile);
    actions.saveAsAction = fileMenu->addAction(QObject::tr("Save &As..."), QKeySequence::SaveAs, window, &MainWindow::saveFileAs);
    fileMenu->addSeparator();
    actions.preferencesAction = fileMenu->addAction(
        QObject::tr("&Preferences..."),
        QKeySequence::Preferences,
        window,
        &MainWindow::openPreferences
    );
    fileMenu->addSeparator();
    actions.quitAction = fileMenu->addAction(QObject::tr("&Quit"), QKeySequence::Quit, window, &QWidget::close);

    QMenu *viewMenu = window->menuBar()->addMenu(QObject::tr("&View"));
    actions.focusAction = viewMenu->addAction(
        QObject::tr("Toggle &Focus Mode"),
        QKeySequence(Qt::CTRL | Qt::Key_D),
        editor,
        &FeltEditor::toggleFocusMode
    );
    actions.focusAction->setCheckable(true);
    actions.focusAction->setChecked(editor->isFocusMode());
    QObject::connect(editor, &FeltEditor::focusModeToggled, actions.focusAction, &QAction::setChecked);

    actions.typewriterAction = viewMenu->addAction(
        QObject::tr("Toggle &Typewriter Mode"),
        QKeySequence(Qt::CTRL | Qt::Key_T),
        editor,
        &FeltEditor::toggleTypewriterMode
    );
    actions.typewriterAction->setCheckable(true);
    actions.typewriterAction->setChecked(editor->isTypewriterMode());
    QObject::connect(editor, &FeltEditor::typewriterModeToggled, actions.typewriterAction, &QAction::setChecked);

    viewMenu->addSeparator();
    actions.fullScreenAction = viewMenu->addAction(
        QObject::tr("Toggle Full &Screen"),
        QKeySequence::FullScreen,
        window,
        [window]() { toggleWindowFullScreen(window); }
    );

    return actions;
}
