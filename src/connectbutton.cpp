/**
 * @file connectbutton.cpp
 * @author ldk
 * @brief 原用于相机的连接按钮，双击按钮或者单击图标触发事件
 * @version 0.1
 * @date 2023-02-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <QPushButton>
#include <QHBoxLayout>
#include <QMouseEvent>

#include "connectbutton.hpp"

#ifndef ICON
#define ICON Icon()
#endif // !ICON

ConnectButton::ConnectButton(QWidget *parent)
	: DoubleClickedButton(parent)
	, m_bConnected(false)
	, m_pPushButton(new QPushButton(this))
	, m_pHLayout(new QHBoxLayout(this))
{
	m_pPushButton->setFixedSize(22, 22);
	m_pPushButton->setIcon(QIcon(ICON));
	m_pPushButton->setIconSize(QSize(20, 20));
	connect(m_pPushButton, &QPushButton::clicked, this, &ConnectButton::clickEvent);

	m_pHLayout->setContentsMargins(0, 0, 0, 0);
	m_pHLayout->addStretch(1);
	m_pHLayout->addWidget(m_pPushButton);
}

ConnectButton::~ConnectButton()
{
	if (m_pPushButton)
		m_pPushButton->deleteLater();
	if (m_pHLayout)
		m_pHLayout->deleteLater();
}

void ConnectButton::setDoubleClickedEvent(Function function)
{
	m_clickFunc = function;
}

void ConnectButton::changeConnectionStatus()
{
	m_bConnected = !m_bConnected;
	// ÇÐ»»Í¼±ê
	m_pPushButton->setIcon(QIcon(ICON));
	m_pPushButton->setIconSize(QSize(20, 20));
}

void ConnectButton::mouseDoubleClickEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
		clickEvent();
}

void ConnectButton::clickEvent()
{
	if (m_bConnected)
		return;
	changeConnectionStatus();
	m_clickFunc(this);
}
