/**
 * @file paintwidget.hpp
 * @author ldk
 * @brief 
 * @version 0.1
 * @date 2023-02-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef _PAINT_WIDGET_HPP_
#define _PAINT_WIDGET_HPP_

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>

enum class DrawShape
{
    None,
    Line,
    Ellipse,
    Rectangle
};

void drawGraphic(QPainter* painter, const DrawShape& shape, QPointF start, QPointF stop);

class PaintWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PaintWidget
    (
        QWidget* parent,
        DrawShape shape,
        const QColor& color = Qt::black,
        qreal penSize = 2
    );
    explicit PaintWidget
    (
        QWidget* parent,
        DrawShape shape,
        const QPen& pen
    );
    ~PaintWidget() = default;

    inline
    const QPen& pen() const noexcept { return m_Pen; }

    inline
    void setPen(const QPen& pen) { m_Pen = pen; }
    
    inline
    void setPen(const QColor& color, qreal penSize) { m_Pen = QPen(color, penSize); }

    inline
    const DrawShape& getShape() const noexcept { return m_Shape; }

    inline
    void setShape(const DrawShape& shape) { m_Shape = shape; }

    inline
    QPointF startPoint() const noexcept { return m_startPoint; }

    inline
    void setStartPoint(const QPointF& start) const { m_startPoint = start; }

    inline
    void setStartPoint(qreal x, qreal y) const { m_startPoint = QPointF(x, y); }

    inline
    QPointF stopPoint() const noexcept { return m_stopPoint; }

    inline
    void setStopPoint(const QPointF& stop) const { m_stopPoint = stop; }

    inline
    void setStopPoint(qreal x, qreal y) const { m_stopPoint = QPointF(x, y); }
    
protected slots:
    inline
    void paintEvent(QPaintEvent* event = nullptr)
    {
        QPainter painter(this);
        painter.setPen(m_Pen);
        drawGraphic(&painter, m_Shape, m_startPoint, m_stopPoint);
    }

private:
    QPen m_Pen;
    DrawShape m_Shape;
    mutable QPointF m_startPoint;
    mutable QPointF m_stopPoint;
};

#endif // !_PAINT_WIDGET_HPP_