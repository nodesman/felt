#include "mainwindow.h"
#include "themeregistry.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QVBoxLayout>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QTextBlock>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi();
    m_menuActions = MenuBuilder::populate(this);
    applyTheme(ThemeRegistry::obsidianVoid());

    resize(1000, 750);

    const QString initialProse = QStringLiteral(
        "We create rituals to protect our attention from the noise of the world. In the quiet, thoughts settle into their natural shapes without rushing to justify themselves.\n\n"
        "Writing here feels like breathing in an empty room. No metrics, no expectations, just the honest cadence of words finding their place upon the dark page.\n\n"
        "Tomorrow will ask for answers, but tonight is for questions that have no hurry to be resolved."
    );

    m_editor->setPlainText(initialProse);
    m_editor->document()->setModified(false);
    m_isDirty = false;
    QTextBlock block2 = m_editor->document()->findBlockByNumber(2);
    if (block2.isValid()) {
        QTextCursor cur(block2);
        cur.movePosition(QTextCursor::EndOfBlock);
        m_editor->setTextCursor(cur);
    }
    m_editor->setFocusMode(true);
    m_editor->setTypewriterMode(true);
    updateStats();
    updateWindowTitle();
}

void MainWindow::setupUi() {
    QWidget *centralContainer = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralContainer);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    m_editor = new FeltEditor(this);
    mainLayout->addWidget(m_editor, 1);
    m_hud = new DocumentHUD(this);
    mainLayout->addWidget(m_hud);
    setCentralWidget(centralContainer);
    connect(m_editor, &FeltEditor::statsChanged, this, &MainWindow::updateStats);
    connect(m_editor->document(), &QTextDocument::contentsChanged, this, &MainWindow::onDocumentModified);
}

void MainWindow::applyTheme(const ThemePalette &palette) {
    m_editor->applyTheme(palette);
    m_hud->applyTheme(palette);

    const QString bg = palette.bgOuter().name();
    const QString fg = palette.textMain().name();
    const QString muted = palette.hudText().name();
    const QString selBg = palette.selectionBg().name();
    const QString selFg = palette.selectionText().name();
    setStyleSheet(QStringLiteral(
        "QMainWindow { background-color: %1; }"
        "QMenuBar { background-color: %1; color: %3; border: none; font-size: 12px; }"
        "QMenuBar::item { background: transparent; padding: 4px 8px; }"
        "QMenuBar::item:selected { background-color: %4; color: %5; border-radius: 3px; }"
        "QMenu { background-color: %1; color: %2; border: 1px solid %3; border-radius: 4px; padding: 4px; }"
        "QMenu::item:selected { background-color: %4; color: %5; border-radius: 2px; }"
    ).arg(bg, fg, muted, selBg, selFg));
}

void MainWindow::updateStats() {
    m_hud->updateFromText(m_editor->toPlainText());
}

void MainWindow::updateWindowTitle() {
    QString docName = QStringLiteral("Untitled");
    if (!m_currentFilePath.isEmpty()) {
        docName = QFileInfo(m_currentFilePath).fileName();
    }
    QString title = QStringLiteral("%1 — Felt").arg(docName);
    if (m_isDirty) {
        title = QStringLiteral("● ") + title;
    }
    setWindowTitle(title);
}

void MainWindow::onDocumentModified() {
    const bool dirty = m_editor->document()->isModified();
    if (m_isDirty != dirty) {
        m_isDirty = dirty;
        updateWindowTitle();
    }
}

void MainWindow::newFile() {
    if (!maybeSave()) {
        return;
    }
    m_editor->clear();
    m_currentFilePath.clear();
    m_isDirty = false;
    m_editor->document()->setModified(false);
    updateWindowTitle();
    updateStats();
}

void MainWindow::openFile() {
    if (!maybeSave()) {
        return;
    }
    const QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Document"), QString(),
        tr("Markdown Files (*.md *.markdown);;Text Files (*.txt);;All Files (*)"));
    if (!fileName.isEmpty()) {
        loadFile(fileName);
    }
}

bool MainWindow::saveFile() {
    if (m_currentFilePath.isEmpty()) {
        return saveFileAs();
    }
    return writeFile(m_currentFilePath);
}

bool MainWindow::saveFileAs() {
    const QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save Document"), QString(),
        tr("Markdown Files (*.md);;Text Files (*.txt);;All Files (*)"));
    if (fileName.isEmpty()) {
        return false;
    }
    return writeFile(fileName);
}

void MainWindow::openPreferences() {
}

bool MainWindow::maybeSave() {
    if (!m_isDirty) {
        return true;
    }
    const QMessageBox::StandardButton ret = QMessageBox::warning(this, tr("Felt"),
        tr("The document has been modified.\nDo you want to save your changes?"),
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    if (ret == QMessageBox::Save) {
        return saveFile();
    }
    if (ret == QMessageBox::Cancel) {
        return false;
    }
    return true;
}

bool MainWindow::loadFile(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        return false;
    }
    QTextStream in(&file);
    m_editor->setPlainText(in.readAll());
    file.close();
    m_currentFilePath = filePath;
    m_editor->document()->setModified(false);
    m_isDirty = false;
    updateWindowTitle();
    updateStats();
    return true;
}

bool MainWindow::writeFile(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        return false;
    }
    QTextStream out(&file);
    out << m_editor->toPlainText();
    file.close();
    m_currentFilePath = filePath;
    m_editor->document()->setModified(false);
    m_isDirty = false;
    updateWindowTitle();
    return true;
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (maybeSave()) {
        event->accept();
        return;
    }
    event->ignore();
}
