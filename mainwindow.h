#pragma once

#include <QMainWindow>
#include <QLabel>
#include "editor.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override = default;

    FeltEditor* editor() const { return m_editor; }
    QString currentFilePath() const { return m_currentFilePath; }
    bool isDirty() const { return m_isDirty; }

public slots:
    void newFile();
    void openFile();
    bool saveFile();
    bool saveFileAs();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void updateStats();
    void updateWindowTitle();
    void onDocumentModified();

private:
    void setupUi();
    void setupMenus();
    void applyTheme();
    bool maybeSave();
    bool loadFile(const QString &filePath);
    bool writeFile(const QString &filePath);

    FeltEditor *m_editor = nullptr;
    QLabel *m_statsLabel = nullptr;
    QString m_currentFilePath;
    bool m_isDirty = false;
};
