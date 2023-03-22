/**
 * @file graphicsviewinterface.cpp
 * @author ldk
 * @brief 图像显示控件
 * @version 0.1
 * @date 2023-02-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <QBoxLayout>
#include <QImageReader>
#include <QtCore/qglobal.h>

#include "graphicsviewinterface.hpp"
#include "graphicsview.hpp"

GraphicsViewInterface::GraphicsViewInterface
(
	QBoxLayout* panel,
	QWidget*    parent,
	double      minZoom,
	double      maxZoom
)
	: m_bDynamically(false)
	, m_qtImage(QImage())
	, m_pWidget(new GraphicsView(this, parent, minZoom, maxZoom))
	, m_Position(QPoint())
{
	Init(panel);
}

GraphicsViewInterface::GraphicsViewInterface
(
	QBoxLayout*   panel,
	const QImage& image,
	QWidget*      parent,
	double        minZoom,
	double        maxZoom
)
	: m_bDynamically(false)
	, m_qtImage(QImage(image.copy()))
	, m_pWidget(new GraphicsView(this, parent, minZoom, maxZoom))
	, m_Position(QPoint())
{
	Init(panel);
}

GraphicsViewInterface::~GraphicsViewInterface()
{
	StaticMode();
	if (m_pWidget)
		m_pWidget->deleteLater();
}

/**
 * @brief 初始化
 * @param panel 展示图像的容器
 */
void GraphicsViewInterface::Init(QBoxLayout* panel)
{
	if (panel)
		panel->addWidget(m_pWidget);
}

int GraphicsViewInterface::width() const noexcept
{
	return m_pWidget->width();
}

int GraphicsViewInterface::height() const noexcept
{
	return m_pWidget->height();
}

void GraphicsViewInterface::setMinZoom(double minZoom)
{
	m_pWidget->setMinZoom(minZoom);
}

double GraphicsViewInterface::getMinZoom() const noexcept
{
	return m_pWidget->getMinZoom();
}

void GraphicsViewInterface::setMaxZoom(double maxZoom)
{
	m_pWidget->setMaxZoom(maxZoom);
}

double GraphicsViewInterface::getMaxZoom() const noexcept
{
	return m_pWidget->getMaxZoom();
}

/**
 * @brief 设置图像显示模式为动态更新模式
 * @remarks 调用DynamicMode显示完毕以后需要调用StaticMode改回静态显示模式，否则会报错
 */
void GraphicsViewInterface::DynamicMode(int _RefreshTime)
{
	// 若已经设为动态更新模式则不进行操作
	if (isDynamicMode())
		return;
	// 开启动态刷新模式
	m_pWidget->dynamicMode(_RefreshTime);
	// 转换状态需要刷新图像，否则会报错
	m_qtImage = QImage();
	m_bDynamically.store(true, std::memory_order_release);
}

/**
 * @brief 设置图像显示模式为静态显示模式
 *
 */
void GraphicsViewInterface::StaticMode()
{
	// 若已经设为静态显示模式则不进行操作
	if (isStaticMode())
		return;
	// 开启静态显示模式
	m_pWidget->staticMode();
	// 转换状态需要刷新图像，否则会报错
	m_qtImage = QImage();
	m_bDynamically.store(false, std::memory_order_release);
}

/**
 * @brief 静态设置图像
 *
 * @param image 待展示的图像
 */
void GraphicsViewInterface::setImageStatically(const QImage& _image)
{
    m_qtImage = _image.copy();
	m_pWidget->setImage();
}

/**
 * @brief 静态设置图像
 *
 * @param path 待展示图像的路径
 */
void GraphicsViewInterface::setImageStatically(const QString& _path)
{
	QImageReader reader(_path);
	reader.setDecideFormatFromContent(true);
	m_qtImage = reader.read();
	m_pWidget->setImage();
}

QPoint GraphicsViewInterface::getIamgePosition(const QPoint& _pos)
{
    return m_pWidget->mapToScene(_pos).toPoint();
}
