/**
 * @file graphicsview.hpp
 * @author ldk
 * @brief 
 * @version 0.1
 * @date 2023-02-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef _GRAPHICS_VIEW_HPP_
#define _GRAPHICS_VIEW_HPP_

#include <QTimer>
#include <QGraphicsView>

class GraphicsViewInterface;

class GraphicsView : public QGraphicsView
{
	Q_OBJECT

public:
	explicit GraphicsView
    (
        GraphicsViewInterface*  controller,
        QWidget*                parent = nullptr,
        double                  minZoom = 0.005,
        double                  maxZoom = 200
    );
	~GraphicsView();

    void setImage();

    inline int    width() { return viewport()->width(); }
    inline int    height() { return viewport()->height(); }
    inline double getMinZoom() { return m_dMinZoom; }
    inline double getMaxZoom() { return m_dMaxZoom; }
    inline void   setMinZoom(double minZoom) { m_dMinZoom = minZoom; }
    inline void   setMaxZoom(double maxZoom) { m_dMaxZoom = maxZoom; }
    inline QPoint getMousePosition() { return m_qtLastMousePos; }
    inline void   dynamicMode(int _time) { m_pTimer->start(_time); }
    inline void   staticMode() { m_pTimer->stop(); }

protected:
    virtual void mousePressEvent(QMouseEvent*) override;
    virtual void mouseMoveEvent(QMouseEvent*) override;
    virtual void mouseReleaseEvent(QMouseEvent*) override;
    virtual void wheelEvent(QWheelEvent*) override;

public slots:
    void ZoomIn() { Zoom(1.05); }
    void ZoomOut() { Zoom(0.95); }
    void Zoom(double);
    void Translate(QPointF);

private:
    bool                    m_bIsTranslate;      // 是否通过鼠标对图像进行仿射变换操作
    double                  m_dMaxZoom;          // 图像缩放最大倍数
    double                  m_dMinZoom;          // 图像缩放最小倍数
    QPoint                  m_qtLastMousePos;    // 鼠标最后落在位置
    QGraphicsScene*         m_pScene;            // 放置图像控件地场景
    QGraphicsPixmapItem*    m_pImageItem;        // 放置图像的控件
    QTimer*                 m_pTimer;            // 用于动态更新图像的计时器
    GraphicsViewInterface*  m_pController;       // 接口控件
};

#endif // !_GRAPHICS_VIEW_HPP_