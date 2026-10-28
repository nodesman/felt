#pragma once

#include "theme.h"
#include <QWidget>
#include <QPoint>

class TitleBarDragStrip : public QWidget {
    Q_OBJECT

public:
    static constexpr int kStripHeight = 36;

    explicit TitleBarDragStrip(QWidget *parent = nullptr);

    void applyTheme(const ThemePalette &palette);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QPoint m_dragOffset;
    bool m_dragging = false;
};
