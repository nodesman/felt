#include "documenthud.h"
#include "stats.h"
#include "themeregistry.h"
#include <QHBoxLayout>

DocumentHUD::DocumentHUD(QWidget *parent)
    : QWidget(parent)
    , m_palette(ThemeRegistry::obsidianVoid())
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(40, 0, 40, 16);
    layout->addStretch(1);

    m_label = new QLabel(this);
    layout->addWidget(m_label);

    applyTheme(m_palette);
    updateFromText(QString());
}

void DocumentHUD::updateFromText(const QString &plainText) {
    const DocumentStats stats = DocumentStats::compute(plainText);
    m_label->setText(DocumentStats::formatSummary(stats));
}

void DocumentHUD::applyTheme(const ThemePalette &palette) {
    m_palette = palette;
    const QString hudColor = m_palette.hudText().name();
    m_label->setStyleSheet(QStringLiteral(
        "QLabel { "
        "  color: %1; "
        "  font-family: 'Menlo', 'Monaco', 'Courier New', monospace; "
        "  font-size: 11px; "
        "  letter-spacing: 0.5px; "
        "}"
    ).arg(hudColor));
}

QString DocumentHUD::summaryText() const {
    return m_label->text();
}
