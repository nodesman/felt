#include "mainwindow.h"
#include "stats.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileInfo>
#include <QFile>
#include <QTextStream>
#include <QTextBlock>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi();
    setupMenus();
    applyTheme();

    resize(1000, 750);

    // Initial placeholder text matching felt_ui.jpg
    const QString initialProse = QStringLiteral(
        "We create rituals to protect our attention from the noise of the world. In the quiet, thoughts settle into their natural shapes without rushing to justify themselves.\n\n"
        "Writing here feels like breathing in an empty room. No metrics, no expectations, just the honest cadence of words finding their place upon the dark page.\n\n"
        "Tomorrow will ask for answers, but tonight is for questions that have no hurry to be resolved."
    );

    m_editor->setPlainText(initialProse);
    m_editor->document()->setModified(false);
    m_isDirty = false;

    // Position cursor at the end of the second paragraph (Block 2)
    QTextBlock block2 = m_editor->document()->findBlockByNumber(2);
    if (block2.isValid()) {
        QTextCursor cur(block2);
        cur.movePosition(QTextCursor::EndOfBlock);
        m_editor->setTextCursor(cur);
    }

    // Default to active Focus Mode and Typewriter Mode
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

    // Whisper-quiet bottom HUD
    QWidget *bottomBar = new QWidget(this);
    QHBoxLayout *bottomLayout = new QHBoxLayout(bottomBar);
    bottomLayout->setContentsMargins(40, 0, 40, 16);

    bottomLayout->addStretch(1);

    m_statsLabel = new QLabel(this);
    m_statsLabel->setStyleSheet(QStringLiteral(
        "QLabel { "
        "  color: #4a5568; "
        "  font-family: 'Menlo', 'Monaco', 'Courier New', monospace; "
        "  font-size: 11px; "
        "  letter-spacing: 0.5px; "
        "}"
    ));
    bottomLayout->addWidget(m_statsLabel);

    mainLayout->addWidget(bottomBar);

    setCentralWidget(centralContainer);

    connect(m_editor, &FeltEditor::statsChanged, this, &MainWindow::updateStats);
    connect(m_editor->document(), &QTextDocument::contentsChanged, this, &MainWindow::onDocumentModified);
}

void MainWindow::setupMenus() {
    // File Menu
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    QAction *newAct = fileMenu->addAction(tr("&New"), QKeySequence::New, this, &MainWindow::newFile);
    newAct->setStatusTip(tr("Create a new document"));

    QAction *openAct = fileMenu->addAction(tr("&Open..."), QKeySequence::Open, this, &MainWindow::openFile);
    openAct->setStatusTip(tr("Open an existing markdown file"));

    QAction *saveAct = fileMenu->addAction(tr("&Save"), QKeySequence::Save, this, &MainWindow::saveFile);
    saveAct->setStatusTip(tr("Save the document to disk"));

    QAction *saveAsAct = fileMenu->addAction(tr("Save &As..."), QKeySequence::SaveAs, this, &MainWindow::saveFileAs);
    saveAsAct->setStatusTip(tr("Save the document with a new name"));

    fileMenu->addSeparator();

    QAction *quitAct = fileMenu->addAction(tr("&Quit"), QKeySequence::Quit, this, &QWidget::close);
    quitAct->setStatusTip(tr("Exit Felt"));

    // View Menu
    QMenu *viewMenu = menuBar()->addMenu(tr("&View"));

    QAction *focusAct = viewMenu->addAction(tr("Toggle &Focus Mode"), QKeySequence(Qt::CTRL | Qt::Key_D),
                                           m_editor, &FeltEditor::toggleFocusMode);
    focusAct->setCheckable(true);
    focusAct->setChecked(true);
    connect(m_editor, &FeltEditor::focusModeToggled, focusAct, &QAction::setChecked);

    QAction *typewriterAct = viewMenu->addAction(tr("Toggle &Typewriter Mode"), QKeySequence(Qt::CTRL | Qt::Key_T),
                                                m_editor, &FeltEditor::toggleTypewriterMode);
    typewriterAct->setCheckable(true);
    typewriterAct->setChecked(true);
    connect(m_editor, &FeltEditor::typewriterModeToggled, typewriterAct, &QAction::setChecked);

    viewMenu->addSeparator();

    QAction *fullscreenAct = viewMenu->addAction(tr("Toggle Full &Screen"), QKeySequence::FullScreen, this, [this]() {
        if (isFullScreen()) {
            showNormal();
        } else {
            showFullScreen();
        }
    });
    fullscreenAct->setStatusTip(tr("Enter distraction-free full screen"));
}

void MainWindow::applyTheme() {
    setStyleSheet(QStringLiteral(
        "QMainWindow { background-color: #080808; }"
        "QMenuBar { background-color: #080808; color: #888888; border: none; font-size: 12px; }"
        "QMenuBar::item { background: transparent; padding: 4px 8px; }"
        "QMenuBar::item:selected { background-color: #1a1a1a; color: #ffffff; border-radius: 3px; }"
        "QMenu { background-color: #121212; color: #cccccc; border: 1px solid #222222; border-radius: 4px; padding: 4px; }"
        "QMenu::item:selected { background-color: #1e3a8a; color: #ffffff; border-radius: 2px; }"
        "QMenu::separator { height: 1px; background: #222222; margin: 4px 0px; }"
    ));
}

void MainWindow::updateStats() {
    DocumentStats stats = DocumentStats::compute(m_editor->toPlainText());
    m_statsLabel->setText(DocumentStats::formatSummary(stats));
}

void MainWindow::updateWindowTitle() {
    QString title = QStringLiteral("Felt");
    if (!m_currentFilePath.isEmpty()) {
        QFileInfo fi(m_currentFilePath);
        title = QStringLiteral("%1 — %2").arg(fi.fileName(), title);
    } else {
        title = QStringLiteral("Untitled — %1").arg(title);
    }

    if (m_isDirty) {
        title = QStringLiteral("● ") + title;
    }

    setWindowTitle(title);
}

void MainWindow::onDocumentModified() {
    bool dirty = m_editor->document()->isModified();
    if (m_isDirty != dirty) {
        m_isDirty = dirty;
        updateWindowTitle();
    }
}

void MainWindow::newFile() {
    if (maybeSave()) {
        m_editor->clear();
        m_currentFilePath.clear();
        m_isDirty = false;
        m_editor->document()->setModified(false);
        updateWindowTitle();
        updateStats();
    }
}

void MainWindow::openFile() {
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this,
            tr("Open Document"), QString(),
            tr("Markdown Files (*.md *.markdown);;Text Files (*.txt);;All Files (*)"));
        if (!fileName.isEmpty()) {
            loadFile(fileName);
        }
    }
}

bool MainWindow::saveFile() {
    if (m_currentFilePath.isEmpty()) {
        return saveFileAs();
    }
    return writeFile(m_currentFilePath);
}

bool MainWindow::saveFileAs() {
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save Document"), QString(),
        tr("Markdown Files (*.md);;Text Files (*.txt);;All Files (*)"));
    if (fileName.isEmpty()) {
        return false;
    }
    return writeFile(fileName);
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
    } else if (ret == QMessageBox::Cancel) {
        return false;
    }
    return true;
}

bool MainWindow::loadFile(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Felt"),
            tr("Cannot read file %1:\n%2.").arg(QDir::toNativeSeparators(filePath), file.errorString()));
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
        QMessageBox::warning(this, tr("Felt"),
            tr("Cannot write file %1:\n%2.").arg(QDir::toNativeSeparators(filePath), file.errorString()));
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
    } else {
        event->ignore();
    }
}
