#include "mainwindow.h"
#include "themeregistry.h"
#include "windowchrome.h"
#include <QFontDatabase>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QShowEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QVBoxLayout>
#include <QFileInfo>
#include <QFile>
#include <QTextStream>
#include <QTextBlock>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setupUi();
    m_menuActions = MenuBuilder::populate(this);
    QSettings settings(QStringLiteral("com.nodesman.felt"), QStringLiteral("Felt"));
    applyPreferences(Preferences::loadFrom(settings, QFontDatabase::families()));
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
    updateStats();
    updateWindowTitle();
}

void MainWindow::paintEvent(QPaintEvent *event) {
    QMainWindow::paintEvent(event);
    QPainter(this).fillRect(rect(), m_preferences.palette().bgOuter());
}

void MainWindow::showEvent(QShowEvent *event) {
    QMainWindow::showEvent(event);
    WindowChrome::applyUnifiedFramelessChrome(this);
    WindowChrome::syncTitlebarBackground(this, m_preferences.palette());
}

void MainWindow::setupUi() {
    QWidget *centralContainer = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralContainer);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    m_dragStrip = new TitleBarDragStrip(this);
    mainLayout->addWidget(m_dragStrip);
    m_editor = new FeltEditor(this);
    mainLayout->addWidget(m_editor, 1);
    m_hud = new DocumentHUD(this);
    mainLayout->addWidget(m_hud);
    setCentralWidget(centralContainer);
    connect(m_editor, &FeltEditor::statsChanged, this, &MainWindow::updateStats);
    connect(m_editor->document(), &QTextDocument::contentsChanged, this, &MainWindow::onDocumentModified);
    connect(m_editor, &FeltEditor::focusModeToggled, this, [this](bool on) { m_preferences.setFocusMode(on); persistPreferences(); });
    connect(m_editor, &FeltEditor::typewriterModeToggled, this, [this](bool on) { m_preferences.setTypewriterMode(on); persistPreferences(); });
}

void MainWindow::applyPreferences(const Preferences &prefs) {
    m_preferences = prefs;
    m_editor->setTypography(prefs.fontFamily(), prefs.fontSize());
    m_editor->setColumnWidth(prefs.columnWidth());
    m_editor->setCaretWidth(prefs.caretWidth());
    m_editor->setFocusMode(prefs.focusMode());
    m_editor->setTypewriterMode(prefs.typewriterMode());
    m_hud->setVisible(prefs.hudVisible());
    applyTheme(prefs.palette());
}

void MainWindow::persistPreferences() {
    QSettings settings(QStringLiteral("com.nodesman.felt"), QStringLiteral("Felt"));
    m_preferences.saveTo(settings);
}

void MainWindow::applyTheme(const ThemePalette &palette) {
    m_preferences.setThemeKey(palette.key());
    if (m_dragStrip) m_dragStrip->applyTheme(palette);
    m_editor->applyTheme(palette);
    m_hud->applyTheme(palette);
    if (m_settingsSheet) m_settingsSheet->applyTheme(palette);
    WindowChrome::syncTitlebarBackground(this, palette);
    setStyleSheet(QStringLiteral(
        "QMenuBar { background-color: %1; color: %3; border: none; font-size: 12px; }"
        "QMenuBar::item { background: transparent; padding: 4px 8px; }"
        "QMenuBar::item:selected { background-color: %4; color: %5; border-radius: 3px; }"
        "QMenu { background-color: %1; color: %2; border: 1px solid %3; border-radius: 4px; padding: 4px; }"
        "QMenu::item:selected { background-color: %4; color: %5; border-radius: 2px; }"
    ).arg(palette.bgOuter().name(), palette.textMain().name(), palette.hudText().name(),
          palette.selectionBg().name(), palette.selectionText().name()));
    update();
}

void MainWindow::openPreferences() {
    if (!m_settingsSheet) {
        m_settingsSheet = new SettingsSheet(this);
        connect(m_settingsSheet, &SettingsSheet::themeSelected, this, [this](const QString &k) { applyTheme(ThemeRegistry::resolve(k)); persistPreferences(); });
        connect(m_settingsSheet, &SettingsSheet::fontFamilySelected, this, [this](const QString &f) { m_preferences.setFontFamily(f, QFontDatabase::families()); m_editor->setTypography(m_preferences.fontFamily(), m_preferences.fontSize()); persistPreferences(); });
        connect(m_settingsSheet, &SettingsSheet::fontSizeChanged, this, [this](int s) { m_preferences.setFontSize(s); m_editor->setTypography(m_preferences.fontFamily(), m_preferences.fontSize()); persistPreferences(); });
        connect(m_settingsSheet, &SettingsSheet::columnWidthChanged, this, [this](int w) { m_preferences.setColumnWidth(w); m_editor->setColumnWidth(m_preferences.columnWidth()); persistPreferences(); });
        connect(m_settingsSheet, &SettingsSheet::caretWidthChanged, this, [this](int c) { m_preferences.setCaretWidth(c); m_editor->setCaretWidth(m_preferences.caretWidth()); persistPreferences(); });
        connect(m_settingsSheet, &SettingsSheet::focusModeToggled, m_editor, &FeltEditor::setFocusMode);
        connect(m_settingsSheet, &SettingsSheet::typewriterModeToggled, m_editor, &FeltEditor::setTypewriterMode);
        connect(m_settingsSheet, &SettingsSheet::hudVisibilityToggled, this, [this](bool v) { m_preferences.setHudVisible(v); m_hud->setVisible(v); persistPreferences(); });
    }
    m_settingsSheet->syncFromPreferences(m_preferences);
    m_settingsSheet->applyTheme(m_preferences.palette());
    m_settingsSheet->show();
    m_settingsSheet->raise();
    m_settingsSheet->activateWindow();
}

void MainWindow::updateStats() { m_hud->updateFromText(m_editor->toPlainText()); }

void MainWindow::updateWindowTitle() {
    QString docName = m_currentFilePath.isEmpty() ? QStringLiteral("Untitled") : QFileInfo(m_currentFilePath).fileName();
    QString title = QStringLiteral("%1 — Felt").arg(docName);
    if (m_isDirty) title = QStringLiteral("● ") + title;
    setWindowTitle(title);
}

void MainWindow::onDocumentModified() {
    const bool dirty = m_editor->document()->isModified();
    if (m_isDirty != dirty) { m_isDirty = dirty; updateWindowTitle(); }
}

void MainWindow::newFile() {
    if (!maybeSave()) return;
    m_editor->clear(); m_currentFilePath.clear(); m_isDirty = false;
    m_editor->document()->setModified(false); updateWindowTitle(); updateStats();
}

void MainWindow::openFile() {
    if (!maybeSave()) return;
    const QString f = QFileDialog::getOpenFileName(this, tr("Open Document"), QString(), tr("Markdown Files (*.md *.markdown);;All Files (*)"));
    if (!f.isEmpty()) loadFile(f);
}

bool MainWindow::saveFile() { return m_currentFilePath.isEmpty() ? saveFileAs() : writeFile(m_currentFilePath); }

bool MainWindow::saveFileAs() {
    const QString f = QFileDialog::getSaveFileName(this, tr("Save Document"), QString(), tr("Markdown Files (*.md);;All Files (*)"));
    return f.isEmpty() ? false : writeFile(f);
}

bool MainWindow::maybeSave() {
    if (!m_isDirty) return true;
    const auto ret = QMessageBox::warning(this, tr("Felt"), tr("Save changes?"), QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    if (ret == QMessageBox::Save) return saveFile();
    return ret != QMessageBox::Cancel;
}

bool MainWindow::loadFile(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QFile::ReadOnly | QFile::Text)) return false;
    m_editor->setPlainText(QTextStream(&file).readAll());
    m_currentFilePath = filePath; m_editor->document()->setModified(false); m_isDirty = false;
    updateWindowTitle(); updateStats();
    return true;
}

bool MainWindow::writeFile(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QFile::WriteOnly | QFile::Text)) return false;
    QTextStream(&file) << m_editor->toPlainText();
    m_currentFilePath = filePath; m_editor->document()->setModified(false); m_isDirty = false;
    updateWindowTitle();
    return true;
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (maybeSave()) { event->accept(); return; }
    event->ignore();
}
