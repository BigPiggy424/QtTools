/**
 * @file graphicsview.cpp
 * @author ldk
 * @brief 
 * @version 0.1
 * @date 2023-02-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <QWidget>
#include <QGraphicsPixmapItem>
#include <qevent.h>

#include "graphicsview.hpp"
#include "graphicsviewinterface.hpp"

GraphicsView::GraphicsView
(
    GraphicsViewInterface*  controller,
    QWidget*                parent,
    double                  minZoom,
    double                  maxZoom
)
    : QGraphicsView(parent)
    , m_bIsTranslate(false)
    , m_pScene(new QGraphicsScene())
    , m_pImageItem(new QGraphicsPixmapItem())
    , m_pTimer(new QTimer(this))
    , m_pController(controller)
    , m_dMinZoom(minZoom)
    , m_dMaxZoom(maxZoom)
{
    m_pScene->addItem(m_pImageItem);
    setScene(m_pScene);
    // 隐藏滚动条
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // 抗锯齿
    setRenderHint(QPainter::Antialiasing);
    // 设置 scene 的位置和大小
    setSceneRect(INT_MIN / 2, INT_MIN / 2, INT_MAX, INT_MAX);
    // 设置 scene 在 view 的中心点作为锚点
    setTransformationAnchor(QGraphicsView::AnchorViewCenter);

    centerOn(0, 0);

    connect(m_pTimer, &QTimer::timeout, [this] { setImage(); });
};

GraphicsView::~GraphicsView()
{
    m_pTimer->deleteLater();
    m_pScene->deleteLater();
    delete m_pImageItem;
}

/* @brief 显示图像 */
void GraphicsView::setImage()
{
    try
    {
        // 若没有图像则返回
        if (m_pController->m_qtImage.isNull())
            return;
        // 设置显示图像
        m_pImageItem->setPixmap(QPixmap::fromImage(m_pController->m_qtImage));
        // 设置中心坐标
        QPoint newCenter(m_pController->m_qtImage.width() / 2,
                        m_pController->m_qtImage.height() / 2);
        centerOn(newCenter);
        show();
        update();
    }
    catch (const std::exception& e)
    {
        throw(e);
    }
}

void GraphicsView::mousePressEvent(QMouseEvent* event)
{
    // 若没有图像则不执行鼠标事件
    if (m_pController->m_qtImage.isNull())
        return;

    if (event->button() == Qt::RightButton)
    {
        // 触发图像仿射变换信号
        m_bIsTranslate = true;
        m_qtLastMousePos = event->pos();
    }
}

void GraphicsView::mouseMoveEvent(QMouseEvent* event)
{
    // 若没有图像则不执行鼠标事件
    if (m_pController->m_qtImage.isNull())
        return;

    if (m_bIsTranslate)
    {
        QPointF mouseDelta = event->pos() - m_qtLastMousePos;
        Translate(mouseDelta);
    }
    m_qtLastMousePos = event->pos();
    m_pController->setPosition(mapToScene(m_qtLastMousePos).toPoint());
    emit m_pController->mouseMoveEvent();
}

void GraphicsView::mouseReleaseEvent(QMouseEvent* event)
{
    // 若没有图像则不执行鼠标事件
    if (m_pController->m_qtImage.isNull())
        return;

    if (event->button() == Qt::RightButton)
        m_bIsTranslate = false;
}

void GraphicsView::wheelEvent(QWheelEvent* event)
{
    // 若没有图像则不执行鼠标事件
    if (m_pController->m_qtImage.isNull())
        return;
    // 滚轮的滚动量
    QPoint scrollAmount = event->angleDelta();
    // 正值表示放大负值表示缩小
    scrollAmount.y() > 0 ? ZoomIn() : ZoomOut();
}

void GraphicsView::Zoom(double scaleFactor)
{
    // 防止过小或过大
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < m_dMinZoom || factor > m_dMaxZoom)
        return;
    scale(scaleFactor, scaleFactor);
}

void GraphicsView::Translate(QPointF delta)
{
    int w = viewport()->width();
    int h = viewport()->height();
    QPoint newCenter(w / 2. - delta.x() + 0.5, h / 2. - delta.y() + 0.5);
    centerOn(mapToScene(newCenter));
}
