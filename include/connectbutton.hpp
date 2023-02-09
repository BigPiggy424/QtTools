/**
 * @file connectbutton.hpp
 * @author ldk
 * @brief 原用于相机的连接按钮，双击按钮或者单击图标触发事件
 * @version 0.1
 * @date 2023-02-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef _CONNECT_BUTTON_HPP_
#define _CONNECT_BUTTON_HPP_

#include <functional>

#include "doubleclickedbutton.hpp"

class QPushButton;
class QHBoxLayout;

typedef std::function<void(QPushButton*)> Function;

/**
 * @brief 
 * 双击按钮或者单击图标可触发由setDoubleClickedEvent设定的function
 */
class ConnectButton : public DoubleClickedButton
{
	Q_OBJECT

public:
	explicit ConnectButton(QWidget* parent);
	~ConnectButton();

	virtual void setDoubleClickedEvent(Function function) override;

public slots:
	void changeConnectionStatus();

protected slots:
	virtual void mouseDoubleClickEvent(QMouseEvent* event) override;
	void clickEvent();

private:
	static constexpr const char* ICON_CONNECTED{ ":/icons/connected.png" };
	static constexpr const char* ICON_DISCONNECTED{ ":/icons/disconnected.png" };

	bool         m_bConnected;
	Function     m_clickFunc;
	QPushButton* m_pPushButton;
	QHBoxLayout* m_pHLayout;

	inline
	const char* Icon() { return m_bConnected ? ICON_CONNECTED : ICON_DISCONNECTED; }

};

#endif // !_CONNECT_BUTTON_HPP_