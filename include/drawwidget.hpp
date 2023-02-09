/**
 * @file drawwidget.hpp
 * @author ldk
 * @brief 
 * @version 0.1
 * @date 2023-02-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef _DRAW_WIDGET_HPP_
#define _DRAW_WIDGET_HPP_

#include <QMouseEvent>

#include "PaintWidget.hpp"

class DrawWidget : public PaintWidget
{
    Q_OBJECT

public:
    explicit DrawWidget
    (
        QWidget* parent = nullptr,
        DrawShape shape = DrawShape::None,
        const QColor& color = Qt::black,
        qreal penSize = 1
    );
    explicit DrawWidget
    (
        QWidget* parent,
        DrawShape shape = DrawShape::None,
        const QPen& pen = QPen(Qt::black, 1)
    );
    ~DrawWidget() = default;

signals:
    void drawFinished();

protected:
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;

private:
    bool m_isPressed;
};

#endif // !_DRAW_WIDGET_HPP_