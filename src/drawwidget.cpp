/**
 * @file drawwidget.cpp
 * @author ldk
 * @brief 
 * @version 0.1
 * @date 2023-02-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "drawwidget.hpp"

DrawWidget::DrawWidget
(
    QWidget* parent,
    DrawShape shape,
    const QColor& color,
    qreal penSize
)
    : PaintWidget{ parent, shape, color, 1 }
    , m_isPressed(false)
{}

void DrawWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_isPressed = true;
        setStartPoint(event->pos());
    }
}

void DrawWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (!m_isPressed)
        return;
    setStopPoint(event->pos());
    update();
}

void DrawWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_isPressed = false;
        emit drawFinished();
    }
}