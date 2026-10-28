#pragma once

#include <QMainWindow>
#include "editor.h"
#include "documenthud.h"
#include "menubuilder.h"
#include "preferences.h"
#include "settingssheet.h"
#include "titlebardragstrip.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override = default;

    TitleBarDragStrip* dragStrip() const { return m_dragStrip; }
    FeltEditor* editor() const { return m_editor; }
    DocumentHUD* hud() const { return m_hud; }
    SettingsSheet* settingsSheet() const { return m_settingsSheet; }
    WindowMenuActions menuActions() const { return m_menuActions; }
    Preferences preferences() const { return m_preferences; }
    QString currentFilePath() const { return m_currentFilePath; }
    bool isDirty() const { return m_isDirty; }

    void applyPreferences(const Preferences &prefs);
    void applyTheme(const ThemePalette &palette);
    bool loadFile(const QString &filePath);
    bool writeFile(const QString &filePath);

public slots:
    void newFile();
    void openFile();
    bool saveFile();
    bool saveFileAs();
    void openPreferences();

protected:
    void paintEvent(QPaintEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private slots:
    void updateStats();
    void updateWindowTitle();
    void onDocumentModified();

private:
    void setupUi();
    void persistPreferences();
    bool maybeSave();

    TitleBarDragStrip *m_dragStrip = nullptr;
    FeltEditor *m_editor = nullptr;
    DocumentHUD *m_hud = nullptr;
    SettingsSheet *m_settingsSheet = nullptr;
    WindowMenuActions m_menuActions;
    Preferences m_preferences;
    QString m_currentFilePath;
    bool m_isDirty = false;
};
