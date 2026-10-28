#include "preferences.h"
#include "themeregistry.h"
#include "measure.h"
#include <algorithm>

Preferences::Preferences() = default;

QString Preferences::themeKey() const { return m_dynamics.themeKey; }
ThemePalette Preferences::palette() const { return ThemeRegistry::resolve(m_dynamics.themeKey); }
QString Preferences::fontFamily() const { return m_typography.fontFamily; }
int Preferences::fontSize() const { return m_typography.fontSize; }
int Preferences::columnWidth() const { return m_typography.columnWidth; }
int Preferences::caretWidth() const { return m_typography.caretWidth; }
bool Preferences::focusMode() const { return m_dynamics.focusMode; }
bool Preferences::typewriterMode() const { return m_dynamics.typewriterMode; }
bool Preferences::hudVisible() const { return m_dynamics.hudVisible; }

void Preferences::setThemeKey(const QString &key) {
    m_dynamics.themeKey = ThemeRegistry::resolve(key).key();
}

QString Preferences::resolveAvailableFont(const QString &requested, const QStringList &installedFamilies) {
    if (!requested.isEmpty() && installedFamilies.contains(requested)) {
        return requested;
    }
    const QStringList priority = {
        QStringLiteral("Menlo"),
        QStringLiteral("Monaco"),
        QStringLiteral("SF Mono"),
        QStringLiteral("Courier New")
    };
    for (const QString &candidate : priority) {
        if (installedFamilies.contains(candidate)) {
            return candidate;
        }
    }
    return QStringLiteral("Menlo");
}

void Preferences::setFontFamily(const QString &family, const QStringList &installedFamilies) {
    m_typography.fontFamily = resolveAvailableFont(family, installedFamilies);
}

void Preferences::setFontSize(int size) {
    m_typography.fontSize = std::clamp(size, kMinFontSize, kMaxFontSize);
}

void Preferences::setColumnWidth(int width) {
    m_typography.columnWidth = DocumentMeasure::clampColumnWidth(width);
}

void Preferences::setCaretWidth(int width) {
    m_typography.caretWidth = std::clamp(width, kMinCaretWidth, kMaxCaretWidth);
}

void Preferences::setFocusMode(bool enabled) { m_dynamics.focusMode = enabled; }
void Preferences::setTypewriterMode(bool enabled) { m_dynamics.typewriterMode = enabled; }
void Preferences::setHudVisible(bool visible) { m_dynamics.hudVisible = visible; }

void Preferences::saveTo(QSettings &settings) const {
    settings.setValue(QStringLiteral("themeKey"), m_dynamics.themeKey);
    settings.setValue(QStringLiteral("fontFamily"), m_typography.fontFamily);
    settings.setValue(QStringLiteral("fontSize"), m_typography.fontSize);
    settings.setValue(QStringLiteral("columnWidth"), m_typography.columnWidth);
    settings.setValue(QStringLiteral("caretWidth"), m_typography.caretWidth);
    settings.setValue(QStringLiteral("focusMode"), m_dynamics.focusMode);
    settings.setValue(QStringLiteral("typewriterMode"), m_dynamics.typewriterMode);
    settings.setValue(QStringLiteral("hudVisible"), m_dynamics.hudVisible);
    settings.sync();
}

Preferences Preferences::loadFrom(QSettings &settings, const QStringList &installedFamilies) {
    Preferences prefs;
    prefs.setThemeKey(settings.value(QStringLiteral("themeKey"), QStringLiteral("obsidian_void")).toString());
    prefs.setFontFamily(settings.value(QStringLiteral("fontFamily"), QStringLiteral("Menlo")).toString(), installedFamilies);
    prefs.setFontSize(settings.value(QStringLiteral("fontSize"), kDefaultFontSize).toInt());
    prefs.setColumnWidth(settings.value(QStringLiteral("columnWidth"), DocumentMeasure::kDefaultColumnWidth).toInt());
    prefs.setCaretWidth(settings.value(QStringLiteral("caretWidth"), 2).toInt());
    prefs.setFocusMode(settings.value(QStringLiteral("focusMode"), true).toBool());
    prefs.setTypewriterMode(settings.value(QStringLiteral("typewriterMode"), true).toBool());
    prefs.setHudVisible(settings.value(QStringLiteral("hudVisible"), true).toBool());
    return prefs;
}
