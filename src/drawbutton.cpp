/**
 * @file drawbutton.cpp
 * @author ldk
 * @brief 点击按钮以后可以用鼠标在指定区域绘图
 * @version 0.1
 * @date 2023-02-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "drawbutton.hpp"
#include "drawwidget.hpp"

DrawButton::DrawButton(QWidget* parent, QWidget* canvas)
	: QPushButton(parent)
    , m_pDrawWidget(nullptr)
    , m_pCanvas(canvas == nullptr ? parent : canvas)
    , m_start(0, 0)
    , m_stop(0, 0)
{}

DrawButton::~DrawButton()
{
    finishDraw();
}

void DrawButton::startDraw(const QRect& canvas)
{
    finishDraw();
    m_pDrawWidget = new DrawWidget(parentWidget(), DrawShape::Rectangle, Qt::red, 2);
    connect(m_pDrawWidget, &DrawWidget::drawFinished, this, &DrawButton::setPoints);
    m_pDrawWidget->setGeometry(canvas);
    m_pDrawWidget->show();
}

void DrawButton::startDraw(int canvas_x, int canvas_y, int canvasWidth, int canvasHeight)
{
    finishDraw();
    m_pDrawWidget = new DrawWidget(parentWidget(), DrawShape::Rectangle, Qt::red, 2);
    connect(m_pDrawWidget, &DrawWidget::drawFinished, this, &DrawButton::setPoints);
    m_pDrawWidget->setGeometry(QRect(canvas_x, canvas_y, canvasWidth, canvasHeight));
    m_pDrawWidget->show();
}

void DrawButton::startDraw(QWidget* canvas)
{
    finishDraw();
    m_pDrawWidget = new DrawWidget(canvas, DrawShape::Rectangle, Qt::red, 2);
    connect(m_pDrawWidget, &DrawWidget::drawFinished, this, &DrawButton::setPoints);
    m_pDrawWidget->setGeometry(0, 0, INT_MAX, INT_MAX);
    m_pDrawWidget->show();
}

void DrawButton::finishDraw()
{
    if (m_pDrawWidget == nullptr)
        return;
    m_pDrawWidget->close();
    delete m_pDrawWidget;
    m_pDrawWidget = nullptr;
}

void DrawButton::setPoints()
{
    m_start = m_pDrawWidget->startPoint();
    m_stop = m_pDrawWidget->stopPoint();
}
