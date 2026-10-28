#include "windowchrome.h"
#include "theme.h"
#include <QGuiApplication>
#include <QWidget>
#include <QWindow>

#ifdef Q_OS_MACOS
#import <AppKit/AppKit.h>
#endif

namespace {

bool isCocoaPlatform(QWidget *window) {
    return window && QGuiApplication::platformName() == QStringLiteral("cocoa");
}

} // namespace

void WindowChrome::applyUnifiedFramelessChrome(QWidget *window) {
    if (!isCocoaPlatform(window)) {
        return;
    }
#ifdef Q_OS_MACOS
    NSView *nsView = reinterpret_cast<NSView *>(window->winId());
    if (!nsView) {
        return;
    }
    NSWindow *nsWindow = [nsView window];
    if (!nsWindow) {
        return;
    }
    nsWindow.styleMask |= NSWindowStyleMaskFullSizeContentView;
    nsWindow.titlebarAppearsTransparent = YES;
    nsWindow.titleVisibility = NSWindowTitleHidden;
    nsWindow.movableByWindowBackground = YES;
#endif
}

void WindowChrome::syncTitlebarBackground(QWidget *window, const ThemePalette &palette) {
    if (!isCocoaPlatform(window)) {
        return;
    }
#ifdef Q_OS_MACOS
    NSView *nsView = reinterpret_cast<NSView *>(window->winId());
    if (!nsView) {
        return;
    }
    NSWindow *nsWindow = [nsView window];
    if (!nsWindow) {
        return;
    }
    const QColor bg = palette.bgOuter();
    nsWindow.backgroundColor = [NSColor colorWithSRGBRed:bg.redF()
                                                   green:bg.greenF()
                                                    blue:bg.blueF()
                                                   alpha:1.0];
#endif
}

void WindowChrome::startNativeWindowDrag(QWidget *window) {
    if (!window) {
        return;
    }
#ifdef Q_OS_MACOS
    if (isCocoaPlatform(window)) {
        NSView *nsView = reinterpret_cast<NSView *>(window->winId());
        NSWindow *nsWindow = nsView ? [nsView window] : nil;
        NSEvent *currentEvent = [NSApp currentEvent];
        if (nsWindow && currentEvent) {
            [nsWindow performWindowDragWithEvent:currentEvent];
            return;
        }
    }
#endif
    if (window->windowHandle()) {
        window->windowHandle()->startSystemMove();
    }
}
