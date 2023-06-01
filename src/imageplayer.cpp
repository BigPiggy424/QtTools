/**
 * @file imageplayer.cpp
 * @author ldk
 * @brief 
 * @version 0.1
 * @date 2023-02-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <QBoxLayout>

#include "imageplayer.hpp"
#include "graphicsviewinterface.hpp"

void setColorInfo(QColor color, QLabel* label, QColorType type)
{
	QString info;
	switch (type)
	{
	case QColorType::RGB:
		info = "RGB: "
			+ QString::number(color.red()) + ", "
			+ QString::number(color.green()) + ", "
			+ QString::number(color.blue());
		break;
	case QColorType::HSV:
		info = "HSV: "
			+ QString::number(color.hue()) + ", "
			+ QString::number(color.saturation()) + ", "
			+ QString::number(color.value());
		break;
	default:
		break;
	}
	label->setText(info);
}

ImagePlayer::ImagePlayer(QWidget* parent)
	: QWidget(parent)
	, m_pInterface(nullptr)
	, m_pImageLayout(new QVBoxLayout(parent))
	, m_pBottomLayout(new QHBoxLayout(parent))
	, m_pPosLabel(new QLabel(parent))
	, m_pRGBLabel(new QLabel(parent))
	, m_pHSVLabel(new QLabel(parent))
{
	m_pInterface = new GraphicsViewInterface(m_pImageLayout, parent);
	Init();
	setLayout(m_pImageLayout);
}

ImagePlayer::~ImagePlayer()
{
	delete m_pInterface;
	m_pImageLayout->deleteLater();
	m_pBottomLayout->deleteLater();
	m_pHSVLabel->deleteLater();
	m_pRGBLabel->deleteLater();
	m_pPosLabel->deleteLater();
}

void ImagePlayer::Init()
{
	m_pPosLabel->setFixedHeight(32);
	m_pRGBLabel->setFixedHeight(32);
	m_pHSVLabel->setFixedHeight(32);

	m_pBottomLayout->setContentsMargins(0, 0, 0, 0);
	m_pBottomLayout->setSpacing(0);
	m_pBottomLayout->addWidget(m_pPosLabel, 1);
	m_pBottomLayout->addStretch(1);
	m_pBottomLayout->addWidget(m_pRGBLabel, 1);
	m_pBottomLayout->addStretch(1);
	m_pBottomLayout->addWidget(m_pHSVLabel, 1);
	m_pBottomLayout->addStretch(3);

	m_pImageLayout->setContentsMargins(0, 0, 0, 0);
	m_pImageLayout->setSpacing(0);
	m_pImageLayout->addLayout(m_pBottomLayout);
	m_pImageLayout->setStretch(0, 12);
	m_pImageLayout->setStretch(1, 1);

	connect(m_pInterface, &GraphicsViewInterface::mouseMoveEvent, this, &ImagePlayer::setPosInfo);
}

int ImagePlayer::width() const noexcept
{
    return m_pInterface->width();
}

int ImagePlayer::height() const noexcept
{
    return m_pInterface->height();
}

double ImagePlayer::getMinZoom() const noexcept
{
    return m_pInterface->getMinZoom();
}

double ImagePlayer::getMaxZoom() const noexcept
{
    return m_pInterface->getMaxZoom();
}

void ImagePlayer::setMinZoom(double minZoom)
{
    m_pInterface->setMinZoom(minZoom);
}

void ImagePlayer::setMaxZoom(double maxZoom)
{
    m_pInterface->setMaxZoom(maxZoom);
}

void ImagePlayer::DynamicMode(int _RefreshTime)
{
    m_pInterface->DynamicMode(_RefreshTime);
}

void ImagePlayer::StaticMode()
{
    m_pInterface->StaticMode();
}

bool ImagePlayer::isDynamicMode() const noexcept
{
    return m_pInterface->isDynamicMode();
}

bool ImagePlayer::isStaticMode() const noexcept
{
    return m_pInterface->isStaticMode();
}

const QImage& ImagePlayer::getImage() noexcept
{
    return m_pInterface->getImage();
}

void ImagePlayer::setImage(const QImage& image)
{
    m_pInterface->setImage(image);
}

QPoint ImagePlayer::getImagePosition(const QPoint& pos)
{
    return m_pInterface->getIamgePosition(pos);
}

void ImagePlayer::setPosInfo()
{
	setPositionInfo(m_pInterface->getPosition(), m_pPosLabel);
	setColorInfo(m_pInterface->getPositionColor(), m_pRGBLabel);
	setColorInfo(m_pInterface->getPositionColor(), m_pHSVLabel, QColorType::HSV);
}