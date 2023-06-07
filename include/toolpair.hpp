/**
 * @file toolpair.hpp
 * @author ldk
 * @brief 
 * @version 0.1
 * @date 2023-02-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef _TOOL_PAIR_HPP_
#define _TOOL_PAIR_HPP_

#include <shared_mutex>

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>

template <typename Widget>
class ToolPair : public QWidget
{
public:
	explicit ToolPair(QWidget* parent);
	explicit ToolPair(const QString& text, QWidget* parent);
	explicit ToolPair(const QString& text, Widget* widget, QWidget* parent);
	~ToolPair();

	QString key() const noexcept;
	Widget* getValueWidget();

	void setKey(const QString& text);
	void setKeyAlignment(Qt::Alignment alignment);
	void setValueWidget(const Widget& widget);
	void setFixedSize(const QSize& size);
	void setFixedSize(int w, int h);
	void setFixedWidth(int w);
	void setFixedHeight(int h);
	void setMinimumSize(const QSize& size);
	void setMinimumSize(int w, int h);
	void setMinimumWidth(int w);
	void setMinimumHeight(int h);
	void setMaxiumSize(const QSize& size);
	void setMaxiumSize(int w, int h);
	void setMaxiumWidth(int w);
	void setMaxiumHeight(int h);
	void setStretch(int firstStretch = 1, int secondStretch = 1);

	QString title() const noexcept { return key(); }
	void setTitle(const QString& title) { setKey(title); }

private:
	static constexpr const char* DefaultTitle { "ToolPair" };

	QLabel*			m_pKey;
	Widget*			m_pWidget;
	QHBoxLayout*	m_pHLayout;

	void Init(const QString& text);
};

template <typename Widget>
ToolPair<Widget>::ToolPair(QWidget* parent)
	: QWidget(parent)
	, m_pKey(new QLabel(parent))
	, m_pWidget(new Widget(parent))
	, m_pHLayout(new QHBoxLayout(parent))
{
	Init(DefaultTitle);
}

template <typename Widget>
ToolPair<Widget>::ToolPair(const QString& text, QWidget* parent)
	: QWidget(parent)
	, m_pKey(new QLabel(parent))
	, m_pWidget(new Widget(parent))
	, m_pHLayout(new QHBoxLayout(parent))
{
	Init(text);
}

template <typename Widget>
ToolPair<Widget>::ToolPair(const QString& text, Widget* widget, QWidget* parent)
	: QWidget(parent)
	, m_pKey(new QLabel(parent))
	, m_pWidget(widget)
	, m_pHLayout(new QHBoxLayout(parent))
{
	Init(text);
}

template <typename Widget>
ToolPair<Widget>::~ToolPair()
{
	if (m_pHLayout)
		m_pHLayout->deleteLater();
	if (m_pWidget)
		delete m_pWidget;
	if (m_pKey)
		m_pKey->deleteLater();
}

template <typename Widget>
QString ToolPair<Widget>::key() const noexcept
{
	return m_pKey->text();
}

template <typename Widget>
Widget* ToolPair<Widget>::getValueWidget()
{
	return m_pWidget;
}

template <typename Widget>
void ToolPair<Widget>::setKey(const QString& text)
{
	m_pKey->setText(text);
}

template <typename Widget>
void ToolPair<Widget>::setKeyAlignment(Qt::Alignment alignment)
{
	m_pKey->setAlignment(alignment);
}

template <typename Widget>
void ToolPair<Widget>::setValueWidget(const Widget& widget)
{
	if (m_pWidget)
		delete m_pWidget;
	m_pWidget = new Widget(widget);
}

template <typename Widget>
void ToolPair<Widget>::setFixedSize(const QSize& size)
{
	m_pKey->setFixedSize(size);
	m_pWidget->setFixedSize(size);
}

template <typename Widget>
void ToolPair<Widget>::setFixedSize(int w, int h)
{
	m_pKey->setFixedSize(w, h);
	m_pWidget->setFixedSize(w, h);
}

template <typename Widget>
void ToolPair<Widget>::setFixedWidth(int w)
{
	m_pKey->setFixedWidth(w);
	m_pWidget->setFixedWidth(w);
}

template <typename Widget>
void ToolPair<Widget>::setFixedHeight(int h)
{
	m_pKey->setFixedHeight(h);
	m_pWidget->setFixedHeight(h);
}

template <typename Widget>
void ToolPair<Widget>::setMinimumSize(const QSize &size)
{
	m_pKey->setMinimumSize(size);
	m_pWidget->setMinimumSize(size);
}

template <typename Widget>
void ToolPair<Widget>::setMinimumSize(int w, int h)
{
	m_pKey->setMinimumSize(w, h);
	m_pWidget->setMinimumSize(w, h);
}

template <typename Widget>
void ToolPair<Widget>::setMinimumWidth(int w)
{
	m_pKey->setMinimumWidth(w);
	m_pWidget->setMinimumWidth(w);
}

template <typename Widget>
void ToolPair<Widget>::setMinimumHeight(int h)
{
	m_pKey->setMinimumHeight(h);
	m_pWidget->setMinimumHeight(h);
}

template <typename Widget>
void ToolPair<Widget>::setMaxiumSize(const QSize &size)
{
	m_pKey->setMaxiumSize(size);
	m_pWidget->setMaxiumSize(size);
}

template <typename Widget>
void ToolPair<Widget>::setMaxiumSize(int w, int h)
{
	m_pKey->setMaxiumSize(w, h);
	m_pWidget->setMaxiumSize(w, h);
}

template <typename Widget>
void ToolPair<Widget>::setMaxiumWidth(int w)
{
	m_pKey->setMaxiumWidth(w);
	m_pWidget->setMaxiumWidth(w);
}

template <typename Widget>
void ToolPair<Widget>::setMaxiumHeight(int h)
{
	m_pKey->setMaxiumHeight(h);
	m_pWidget->setMaxiumHeight(h);
}

template <typename Widget>
void ToolPair<Widget>::setStretch(int firstStretch, int secondStretch)
{
	m_pHLayout->setStretch(0, firstStretch);
	m_pHLayout->setStretch(1, secondStretch);
}

template <typename Widget>
void ToolPair<Widget>::Init(const QString& text)
{
	m_pKey->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_pWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_pKey->setText(text);
	m_pKey->setAlignment(Qt::AlignCenter);
	m_pHLayout->setContentsMargins(0, 0, 0, 0);
	m_pHLayout->setSpacing(0);
	m_pHLayout->addWidget(m_pKey);
	m_pHLayout->addWidget(m_pWidget);
	m_pHLayout->setStretch(0, 1);
	m_pHLayout->setStretch(1, 1);
	setLayout(m_pHLayout);
}

#endif // !_TOOL_PAIR_HPP_