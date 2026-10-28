#include "settingssheet.h"
#include "themeregistry.h"
#include "measure.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSignalBlocker>

SettingsSheet::SettingsSheet(QWidget *parent)
    : QDialog(parent)
    , m_palette(ThemeRegistry::obsidianVoid())
{
    setWindowTitle(tr("Preferences"));
    setFixedSize(520, 480);
    setModal(false);
    buildLayout();
    applyTheme(m_palette);
}

void SettingsSheet::buildLayout() {
    QVBoxLayout *root = new QVBoxLayout(this);
    root->setContentsMargins(28, 24, 28, 24);
    root->setSpacing(16);

    QLabel *title = new QLabel(tr("PREFERENCES"), this);
    title->setObjectName(QStringLiteral("sectionHeader"));
    root->addWidget(title);

    QHBoxLayout *swatchRow = new QHBoxLayout();
    swatchRow->setSpacing(10);
    for (const ThemePalette &pal : ThemeRegistry::allPalettes()) {
        QPushButton *btn = new QPushButton(pal.displayName(), this);
        btn->setProperty("themeKey", pal.key());
        btn->setCheckable(true);
        btn->setCursor(Qt::PointingHandCursor);
        const QString key = pal.key();
        connect(btn, &QPushButton::clicked, this, [this, key]() {
            updateActiveButtons(key, -1);
            emit themeSelected(key);
        });
        m_controls.themeButtons.push_back(btn);
        swatchRow->addWidget(btn);
    }
    root->addLayout(swatchRow);

    QHBoxLayout *fontRow = new QHBoxLayout();
    fontRow->addWidget(new QLabel(tr("Typeface"), this));
    m_controls.fontCombo = new QComboBox(this);
    m_controls.fontCombo->addItems({
        QStringLiteral("Menlo"),
        QStringLiteral("Monaco"),
        QStringLiteral("SF Mono"),
        QStringLiteral("Courier New")
    });
    connect(m_controls.fontCombo, &QComboBox::currentTextChanged,
            this, &SettingsSheet::fontFamilySelected);
    fontRow->addWidget(m_controls.fontCombo, 1);
    root->addLayout(fontRow);

    QHBoxLayout *sizeRow = new QHBoxLayout();
    sizeRow->addWidget(new QLabel(tr("Font Size"), this));
    m_controls.fontSizeSlider = new QSlider(Qt::Horizontal, this);
    m_controls.fontSizeSlider->setRange(Preferences::kMinFontSize, Preferences::kMaxFontSize);
    m_controls.fontSizeValueLabel = new QLabel(QStringLiteral("16 pt"), this);
    connect(m_controls.fontSizeSlider, &QSlider::valueChanged, this, [this](int val) {
        const int clamped = std::clamp(val, Preferences::kMinFontSize, Preferences::kMaxFontSize);
        m_controls.fontSizeValueLabel->setText(QStringLiteral("%1 pt").arg(clamped));
        emit fontSizeChanged(clamped);
    });
    sizeRow->addWidget(m_controls.fontSizeSlider, 1);
    sizeRow->addWidget(m_controls.fontSizeValueLabel);
    root->addLayout(sizeRow);

    QHBoxLayout *colRow = new QHBoxLayout();
    colRow->addWidget(new QLabel(tr("Column Measure"), this));
    m_controls.columnWidthSlider = new QSlider(Qt::Horizontal, this);
    m_controls.columnWidthSlider->setRange(DocumentMeasure::kMinColumnWidth, DocumentMeasure::kMaxColumnWidth);
    m_controls.columnWidthValueLabel = new QLabel(QStringLiteral("720 px"), this);
    connect(m_controls.columnWidthSlider, &QSlider::valueChanged, this, [this](int val) {
        const int clamped = DocumentMeasure::clampColumnWidth(val);
        m_controls.columnWidthValueLabel->setText(QStringLiteral("%1 px").arg(clamped));
        emit columnWidthChanged(clamped);
    });
    colRow->addWidget(m_controls.columnWidthSlider, 1);
    colRow->addWidget(m_controls.columnWidthValueLabel);
    root->addLayout(colRow);

    m_controls.focusCheck = new QCheckBox(tr("Focus Mode (Illuminate Active Paragraph — Cmd+D)"), this);
    m_controls.typewriterCheck = new QCheckBox(tr("Typewriter Scrolling (Center Active Line — Cmd+T)"), this);
    m_controls.hudCheck = new QCheckBox(tr("Show Reading Statistics HUD"), this);
    connect(m_controls.focusCheck, &QCheckBox::toggled, this, &SettingsSheet::focusModeToggled);
    connect(m_controls.typewriterCheck, &QCheckBox::toggled, this, &SettingsSheet::typewriterModeToggled);
    connect(m_controls.hudCheck, &QCheckBox::toggled, this, &SettingsSheet::hudVisibilityToggled);
    root->addWidget(m_controls.focusCheck);
    root->addWidget(m_controls.typewriterCheck);
    root->addWidget(m_controls.hudCheck);

    QHBoxLayout *caretRow = new QHBoxLayout();
    caretRow->addWidget(new QLabel(tr("Caret Width"), this));
    for (int w : {1, 2, 3}) {
        QPushButton *cbtn = new QPushButton(QStringLiteral("%1px").arg(w), this);
        cbtn->setProperty("caretWidth", w);
        cbtn->setCheckable(true);
        connect(cbtn, &QPushButton::clicked, this, [this, w]() {
            updateActiveButtons(QString(), w);
            emit caretWidthChanged(w);
        });
        m_controls.caretButtons.push_back(cbtn);
        caretRow->addWidget(cbtn);
    }
    root->addLayout(caretRow);
    root->addStretch(1);
}

void SettingsSheet::updateActiveButtons(const QString &themeKey, int caretWidth) {
    if (!themeKey.isEmpty()) {
        for (QPushButton *btn : m_controls.themeButtons) {
            const QSignalBlocker blocker(btn);
            btn->setChecked(btn->property("themeKey").toString() == themeKey);
        }
    }
    if (caretWidth > 0) {
        for (QPushButton *btn : m_controls.caretButtons) {
            const QSignalBlocker blocker(btn);
            btn->setChecked(btn->property("caretWidth").toInt() == caretWidth);
        }
    }
}

void SettingsSheet::syncFromPreferences(const Preferences &prefs) {
    updateActiveButtons(prefs.themeKey(), prefs.caretWidth());
    const QSignalBlocker fontBlocker(m_controls.fontCombo);
    const QSignalBlocker sizeBlocker(m_controls.fontSizeSlider);
    const QSignalBlocker colBlocker(m_controls.columnWidthSlider);
    const QSignalBlocker focusBlocker(m_controls.focusCheck);
    const QSignalBlocker typeBlocker(m_controls.typewriterCheck);
    const QSignalBlocker hudBlocker(m_controls.hudCheck);

    m_controls.fontCombo->setCurrentText(prefs.fontFamily());
    m_controls.fontSizeSlider->setValue(prefs.fontSize());
    m_controls.fontSizeValueLabel->setText(QStringLiteral("%1 pt").arg(prefs.fontSize()));
    m_controls.columnWidthSlider->setValue(prefs.columnWidth());
    m_controls.columnWidthValueLabel->setText(QStringLiteral("%1 px").arg(prefs.columnWidth()));
    m_controls.focusCheck->setChecked(prefs.focusMode());
    m_controls.typewriterCheck->setChecked(prefs.typewriterMode());
    m_controls.hudCheck->setChecked(prefs.hudVisible());
}

void SettingsSheet::applyTheme(const ThemePalette &palette) {
    m_palette = palette;
    setStyleSheet(QStringLiteral(
        "QDialog { background-color: %1; color: %2; }"
        "QLabel { color: %2; font-family: 'Menlo', monospace; font-size: 12px; }"
        "QLabel#sectionHeader { color: %3; font-size: 11px; font-weight: bold; letter-spacing: 1.5px; }"
        "QCheckBox { color: %2; font-family: 'Menlo', monospace; font-size: 12px; spacing: 8px; }"
        "QPushButton { background-color: %4; color: %2; border: 1px solid %5; border-radius: 6px; padding: 6px 10px; font-family: 'Menlo', monospace; font-size: 11px; }"
        "QPushButton:checked { border: 2px solid %3; font-weight: bold; }"
        "QComboBox { background-color: %4; color: %2; border: 1px solid %5; border-radius: 4px; padding: 4px 8px; font-family: 'Menlo', monospace; }"
    ).arg(palette.bgVignetteCenter().name(),
          palette.textMain().name(),
          palette.accent().name(),
          palette.bgOuter().name(),
          palette.markdownPrefix().name()));
}
