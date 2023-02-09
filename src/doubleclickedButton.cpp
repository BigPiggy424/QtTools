/**
 * @file doubleclickedButton.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-02-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <qevent.h>

#include "doubleclickedbutton.hpp"

DoubleClickedButton::DoubleClickedButton(QWidget *parent)
	: QPushButton(parent)
	, m_doubleClickedFunction([](QPushButton*) {})
{
	this->setParent(parent);
}

DoubleClickedButton::~DoubleClickedButton()
{}

void DoubleClickedButton::setDoubleClickedEvent(std::function<void(QPushButton*)> _Func)
{
	m_doubleClickedFunction = _Func;
}

void DoubleClickedButton::mouseDoubleClickEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
		m_doubleClickedFunction(this);
}
