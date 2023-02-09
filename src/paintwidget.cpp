/**
 * @file paintwidget.cpp
 * @author ldk
 * @brief 
 * @version 0.1
 * @date 2023-02-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "paintwidget.hpp"

void drawGraphic(QPainter* painter, const DrawShape& shape, QPointF start, QPointF stop)
{
    switch (shape)
    {
    case DrawShape::None:
        return;
    case DrawShape::Line:
        painter->drawLine(start, stop);
        return;
    case DrawShape::Ellipse:
        painter->drawEllipse(start + stop / 2, stop.x() / 2, stop.y() / 2);
        return;
    case DrawShape::Rectangle:
        painter->drawRect(start.x(), start.y(), stop.x() - start.x(), stop.y() - start.y());
        return;
    default:
        return;
    }
}

PaintWidget::PaintWidget
(
    QWidget* parent,
    DrawShape shape,
    const QColor& color,
    qreal penSize
)
    : QWidget(parent)
    , m_Shape(shape)
    , m_Pen(color, penSize)
    , m_startPoint(0, 0)
    , m_stopPoint(0, 0)
{}

PaintWidget::PaintWidget
(
    QWidget* parent,
    DrawShape shape,
    const QPen& pen
)
    : QWidget(parent)
    , m_Shape(shape)
    , m_Pen(pen)
    , m_startPoint(0, 0)
    , m_stopPoint(0, 0)
{}