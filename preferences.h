#pragma once

#include "theme.h"
#include <QSettings>
#include <QString>
#include <QStringList>

struct TypographyConfig {
    QString fontFamily = QStringLiteral("Menlo");
    int fontSize = 16;
    int columnWidth = 720;
    int caretWidth = 2;
};

struct WritingDynamicsConfig {
    QString themeKey = QStringLiteral("obsidian_void");
    bool focusMode = true;
    bool typewriterMode = true;
    bool hudVisible = true;
};

class Preferences {
public:
    static constexpr int kMinFontSize = 13;
    static constexpr int kMaxFontSize = 26;
    static constexpr int kDefaultFontSize = 16;
    static constexpr int kMinCaretWidth = 1;
    static constexpr int kMaxCaretWidth = 3;

    Preferences();

    QString themeKey() const;
    ThemePalette palette() const;
    QString fontFamily() const;
    int fontSize() const;
    int columnWidth() const;
    int caretWidth() const;
    bool focusMode() const;
    bool typewriterMode() const;
    bool hudVisible() const;

    void setThemeKey(const QString &key);
    void setFontFamily(const QString &family, const QStringList &installedFamilies);
    void setFontSize(int size);
    void setColumnWidth(int width);
    void setCaretWidth(int width);
    void setFocusMode(bool enabled);
    void setTypewriterMode(bool enabled);
    void setHudVisible(bool visible);

    void saveTo(QSettings &settings) const;
    static Preferences loadFrom(QSettings &settings, const QStringList &installedFamilies);
    static QString resolveAvailableFont(const QString &requested, const QStringList &installedFamilies);

private:
    TypographyConfig m_typography;
    WritingDynamicsConfig m_dynamics;
};
