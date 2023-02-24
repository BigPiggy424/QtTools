/**
 * @file graphicscontroller.cpp
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

#include "graphicscontroller.hpp"
#include "graphicsview.hpp"

GraphicsController::GraphicsController
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

GraphicsController::GraphicsController
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

GraphicsController::~GraphicsController()
{
	StaticMode();
	if (m_pWidget)
		m_pWidget->deleteLater();
}

/**
 * @brief 初始化
 * @param panel 展示图像的容器
 */
void GraphicsController::Init(QBoxLayout* panel)
{
	if (panel)
		panel->addWidget(m_pWidget);
}

int GraphicsController::width() const noexcept
{
	return m_pWidget->width();
}

int GraphicsController::height() const noexcept
{
	return m_pWidget->height();
}

void GraphicsController::setMinZoom(double minZoom)
{
	m_pWidget->setMinZoom(minZoom);
}

double GraphicsController::getMinZoom() const noexcept
{
	return m_pWidget->getMinZoom();
}

void GraphicsController::setMaxZoom(double maxZoom)
{
	m_pWidget->setMaxZoom(maxZoom);
}

double GraphicsController::getMaxZoom() const noexcept
{
	return m_pWidget->getMaxZoom();
}

/**
 * @brief 设置图像显示模式为动态更新模式
 * @remarks 调用DynamicMode显示完毕以后需要调用StaticMode改回静态显示模式，否则会报错
 */
void GraphicsController::DynamicMode(ushort _RefreshTime)
{
	// 若已经设为动态更新模式则不进行操作
	if (isDynamicMode())
		return;
	// 开启动态刷新模式
	m_pWidget->dynamicMode(_RefreshTime);
	// 转换状态需要刷新图像，否则会报错
	m_qtImage = QImage();
	m_bDynamically = true;
}

/**
 * @brief 设置图像显示模式为静态显示模式
 *
 */
void GraphicsController::StaticMode()
{
	// 若已经设为静态显示模式则不进行操作
	if (isStaticMode())
		return;
	// 开启静态显示模式
	m_pWidget->staticMode();
	// 转换状态需要刷新图像，否则会报错
	m_qtImage = QImage();
	m_bDynamically = false;
}

/**
 * @brief 静态设置图像
 *
 * @param image 待展示的图像
 */
void GraphicsController::setImageStatically(const QImage& _image)
{
    m_qtImage = _image.copy();
	m_pWidget->setImage();
}

/**
 * @brief 静态设置图像
 *
 * @param path 待展示图像的路径
 */
void GraphicsController::setImageStatically(const QString& _path)
{
	QImageReader reader(_path);
	reader.setDecideFormatFromContent(true);
	m_qtImage = reader.read();
	m_pWidget->setImage();
}