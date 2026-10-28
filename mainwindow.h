#pragma once

#include <QMainWindow>
#include "editor.h"
#include "documenthud.h"
#include "menubuilder.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override = default;

    FeltEditor* editor() const { return m_editor; }
    DocumentHUD* hud() const { return m_hud; }
    WindowMenuActions menuActions() const { return m_menuActions; }
    QString currentFilePath() const { return m_currentFilePath; }
    bool isDirty() const { return m_isDirty; }

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
    void closeEvent(QCloseEvent *event) override;

private slots:
    void updateStats();
    void updateWindowTitle();
    void onDocumentModified();

private:
    void setupUi();
    bool maybeSave();

    FeltEditor *m_editor = nullptr;
    DocumentHUD *m_hud = nullptr;
    WindowMenuActions m_menuActions;
    QString m_currentFilePath;
    bool m_isDirty = false;
};
