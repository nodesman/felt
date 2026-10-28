#pragma once

class QWidget;
class ThemePalette;

class WindowChrome {
public:
    static void applyUnifiedFramelessChrome(QWidget *window);
    static void syncTitlebarBackground(QWidget *window, const ThemePalette &palette);
    static void startNativeWindowDrag(QWidget *window);
};
