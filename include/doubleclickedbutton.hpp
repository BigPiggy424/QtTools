/**
 * @file doubleclickedbutton.hpp
 * @author ldk
 * @brief 双击事件按钮
 * @version 0.1
 * @date 2023-02-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef _DOUBLE_CLICKED_BUTTON_HPP_
#define _DOUBLE_CLICKED_BUTTON_HPP_

#include <functional>

#include <QPushButton>

/**
 * @brief 
 * 双击按钮可触发由setDoubleClickedEvent设定的function
 */
class DoubleClickedButton : public QPushButton
{
	Q_OBJECT

public:
	explicit DoubleClickedButton(QWidget *parent);
	~DoubleClickedButton();

	virtual void setDoubleClickedEvent(std::function<void(QPushButton*)> _Func);

protected:
	virtual void mouseDoubleClickEvent(QMouseEvent* event) override;

private:
	std::function<void(QPushButton*)> m_doubleClickedFunction;
};

#endif // !_DOUBLE_CLICKED_BUTTON_HPP_