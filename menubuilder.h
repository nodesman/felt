#pragma once

#include <QAction>

class MainWindow;

struct WindowMenuActions {
    QAction *newAction = nullptr;
    QAction *openAction = nullptr;
    QAction *saveAction = nullptr;
    QAction *saveAsAction = nullptr;
    QAction *preferencesAction = nullptr;
    QAction *quitAction = nullptr;
    QAction *focusAction = nullptr;
    QAction *typewriterAction = nullptr;
    QAction *fullScreenAction = nullptr;
};

class MenuBuilder {
public:
    static WindowMenuActions populate(MainWindow *window);
};
