#pragma once

#include "preferences.h"
#include <QDialog>
#include <QComboBox>
#include <QSlider>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include <vector>

struct SettingsControls {
    std::vector<QPushButton *> themeButtons;
    QComboBox *fontCombo = nullptr;
    QSlider *fontSizeSlider = nullptr;
    QLabel *fontSizeValueLabel = nullptr;
    QSlider *columnWidthSlider = nullptr;
    QLabel *columnWidthValueLabel = nullptr;
    QCheckBox *focusCheck = nullptr;
    QCheckBox *typewriterCheck = nullptr;
    QCheckBox *hudCheck = nullptr;
    std::vector<QPushButton *> caretButtons;
};

class SettingsSheet : public QDialog {
    Q_OBJECT

public:
    explicit SettingsSheet(QWidget *parent = nullptr);

    void syncFromPreferences(const Preferences &prefs);
    void applyTheme(const ThemePalette &palette);
    SettingsControls controls() const { return m_controls; }

signals:
    void themeSelected(const QString &themeKey);
    void fontFamilySelected(const QString &family);
    void fontSizeChanged(int pointSize);
    void columnWidthChanged(int pixels);
    void caretWidthChanged(int pixels);
    void focusModeToggled(bool enabled);
    void typewriterModeToggled(bool enabled);
    void hudVisibilityToggled(bool visible);

private:
    void buildLayout();
    void updateActiveButtons(const QString &themeKey, int caretWidth);

    SettingsControls m_controls;
    ThemePalette m_palette;
};
