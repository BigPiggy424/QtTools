/**
 * @file graphicsviewinterface.hpp
 * @author ldk 
 * @brief 图像显示控件
 * @version 0.1
 * @date 2023-02-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef _GRAPHICS_CONTROLLER_HPP_
#define _GRAPHICS_CONTROLLER_HPP_

#include <atomic>

#include <QImage>

class QBoxLayout;
class GraphicsView;

/**
 * @brief 
 * 图像显示控件，封装了图像加载、缩放、平移\选点操作。
 * 构造函数需要一个QBoxLayout* panel作为参数，
 * 并调用panel->addWidget()将图像显示控件放置在其中
 */
class GraphicsViewInterface : public QObject
{
    Q_OBJECT

public:
    explicit GraphicsViewInterface
    (
        QBoxLayout* panel,
        QWidget*    parent = nullptr,
        double      minZoom = 0.005,
        double      maxZoom = 200
    );
    explicit GraphicsViewInterface
    (
        QBoxLayout*   panel,
        const QImage& image,
        QWidget*      parent = nullptr,
        double        minZoom = 0.005,
        double        maxZoom = 200
    );
    ~GraphicsViewInterface();

    void    Init(QBoxLayout* panel);
    int     width() const noexcept;
    int     height() const noexcept;
    double  getMinZoom() const noexcept;
    double  getMaxZoom() const noexcept;
    void    setMinZoom(double minZoom);
    void    setMaxZoom(double maxZoom);
    void    DynamicMode(int _RefreshTime = 15);
    void    StaticMode();
    void    setImageStatically(const QImage& _image);
    void    setImageStatically(const QString& _path);
    QPoint  getIamgePosition(const QPoint& _pos);

    /* 是否动态显示模式 */
    inline
    bool isDynamicMode() const noexcept {
        return m_bDynamically.load(std::memory_order_acquire);
    }

    /* 是否静态显示模式 */
    inline
    bool isStaticMode() const noexcept {
        return !m_bDynamically.load(std::memory_order_acquire);
    }

    /**
     * @brief 获取当前图像
     *
     * @return const QImage& 获取当前图像
     */
    inline
    const QImage& getImage() noexcept { return m_qtImage; }

    /**
     * @brief 设置图像
     *
     * @param image 待展示图像的路径
     */
    inline
    void setImage(const QImage& _image)
    {
        if (m_bDynamically.load(std::memory_order_acquire))
            setImageDynamically(_image);
        else
            setImageStatically(_image);
    }

    /**
     * @brief 设置图像
     *
     * @param path 待展示图像的路径
     */
    inline
    void setImage(const QString& _path) { setImageStatically(_path); }

    /**
     * @brief 动态设置图像
     *
     * @param image 待展示图像的路径
     */
    inline
    void setImageDynamically(const QImage& _image) { m_qtImage = _image; }

    /**
     * @brief 设置当前鼠标位置
     *
     * @param position 当前位置
     */
    inline
    void setPosition(const QPoint& position) { m_Position = position; }
    
    /**
     * @brief 获取当前鼠标位置
     *
     * @return QPoint&& 当前鼠标位置
     */
    inline
    QPoint&& getPosition() const noexcept { return std::move(m_Position); }

    /**
     * @brief 获取当前像素点颜色
     *
     * @return QColor&& 当前像素点颜色
     */
    inline
    QColor getPositionColor() const noexcept
    {
        if (!m_qtImage.isNull())
            return m_qtImage.pixelColor(m_Position);
        else
            return QColor();
    }
    
signals:
    void mouseMoveEvent();

private:
    friend class        GraphicsView;
    std::atomic_bool    m_bDynamically;    // 是否动态更新图像
    GraphicsView*       m_pWidget;         // 用于操作绘图的控件
    QImage              m_qtImage;         // 当前显示图像
    mutable QPoint      m_Position;        // 当前像素点颜色
};


#endif // !_GRAPHICS_CONTROLLER_HPP_