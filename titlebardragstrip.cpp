#include "titlebardragstrip.h"
#include "themeregistry.h"
#include <QMouseEvent>

TitleBarDragStrip::TitleBarDragStrip(QWidget *parent)
    : QWidget(parent)
{
    setFixedHeight(kStripHeight);
    setAttribute(Qt::WA_StyledBackground, true);
    applyTheme(ThemeRegistry::obsidianVoid());
}

void TitleBarDragStrip::applyTheme(const ThemePalette &palette) {
    setStyleSheet(QStringLiteral("TitleBarDragStrip { background-color: %1; border: none; }")
                      .arg(palette.bgOuter().name()));
}

void TitleBarDragStrip::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && window()) {
        m_dragging = true;
        m_dragOffset = event->globalPosition().toPoint() - window()->frameGeometry().topLeft();
        event->accept();
        return;
    }
    QWidget::mousePressEvent(event);
}

void TitleBarDragStrip::mouseMoveEvent(QMouseEvent *event) {
    if (m_dragging && (event->buttons() & Qt::LeftButton) && window()) {
        window()->move(event->globalPosition().toPoint() - m_dragOffset);
        event->accept();
        return;
    }
    QWidget::mouseMoveEvent(event);
}

void TitleBarDragStrip::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && m_dragging) {
        m_dragging = false;
        event->accept();
        return;
    }
    QWidget::mouseReleaseEvent(event);
}
