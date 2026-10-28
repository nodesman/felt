#pragma once

#include "theme.h"
#include <QWidget>
#include <QLabel>

class DocumentHUD : public QWidget {
    Q_OBJECT

public:
    explicit DocumentHUD(QWidget *parent = nullptr);

    void updateFromText(const QString &plainText);
    void applyTheme(const ThemePalette &palette);
    QString summaryText() const;

private:
    QLabel *m_label = nullptr;
    ThemePalette m_palette;
};
