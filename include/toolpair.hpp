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

#define ReadLock(mutex) std::shared_lock<std::shared_mutex> lock(mutex)
#define WriteLock(mutex) std::scoped_lock<std::shared_mutex> lock(mutex)

template <typename Widget>
class ToolPair : public QWidget
{
public:
	inline explicit ToolPair(QWidget* parent);
	inline explicit ToolPair(const QString& text, QWidget* parent);
	inline explicit ToolPair(const QString& text, Widget* widget, QWidget* parent);
	inline ~ToolPair();

	inline QString key() const noexcept;
	inline Widget* getValueWidget();

	inline void setKey(const QString& text);
	inline void setKeyAlignment(Qt::Alignment alignment);
	inline void setValueWidget(const Widget& widget);
	inline void setFixedSize(const QSize& size);
	inline void setFixedSize(int w, int h);
	inline void setFixedWidth(int w);
	inline void setFixedHeight(int h);
	inline void setMinimumSize(const QSize& size);
	inline void setMinimumSize(int w, int h);
	inline void setMinimumWidth(int w);
	inline void setMinimumHeight(int h);
	inline void setMaxiumSize(const QSize& size);
	inline void setMaxiumSize(int w, int h);
	inline void setMaxiumWidth(int w);
	inline void setMaxiumHeight(int h);
	inline void setStretch(int firstStretch = 1, int secondStretch = 1);

	inline QString title() const noexcept { return key(); }
	inline void setTitle(const QString& title) { setKey(title); }

private:
	static constexpr const char* DefaultTitle { "ToolPair" };

	QLabel*      m_pKey;
	Widget*      m_pWidget;
	QHBoxLayout* m_pHLayout;
	mutable std::shared_mutex m_Mutex;

	inline void Init(const QString& text);
};

template <typename Widget>
inline ToolPair<Widget>::ToolPair(QWidget* parent)
	: QWidget(parent)
	, m_pKey(new QLabel(parent))
	, m_pWidget(new Widget(parent))
	, m_pHLayout(new QHBoxLayout(parent))
	, m_Mutex(std::shared_mutex())
{
	Init(DefaultTitle);
}

template <typename Widget>
inline ToolPair<Widget>::ToolPair(const QString& text, QWidget* parent)
	: QWidget(parent)
	, m_pKey(new QLabel(parent))
	, m_pWidget(new Widget(parent))
	, m_pHLayout(new QHBoxLayout(parent))
	, m_Mutex(std::shared_mutex())
{
	Init(text);
}

template <typename Widget>
inline ToolPair<Widget>::ToolPair(const QString& text, Widget* widget, QWidget* parent)
	: QWidget(parent)
	, m_pKey(new QLabel(parent))
	, m_pWidget(widget)
	, m_pHLayout(new QHBoxLayout(parent))
	, m_Mutex(std::shared_mutex())
{
	Init(text);
}

template <typename Widget>
inline ToolPair<Widget>::~ToolPair()
{
	WriteLock(m_Mutex);
	if (m_pHLayout)
		m_pHLayout->deleteLater();
	if (m_pWidget)
		delete m_pWidget;
	if (m_pKey)
		m_pKey->deleteLater();
}

template <typename Widget>
inline QString ToolPair<Widget>::key() const noexcept
{
	ReadLock(m_Mutex);
	return m_pKey->text();
}

template <typename Widget>
inline Widget* ToolPair<Widget>::getValueWidget()
{
	ReadLock(m_Mutex);
	return m_pWidget;
}

template <typename Widget>
inline void ToolPair<Widget>::setKey(const QString& text)
{
	WriteLock(m_Mutex);
	m_pKey->setText(text);
}

template <typename Widget>
inline void ToolPair<Widget>::setKeyAlignment(Qt::Alignment alignment)
{
	WriteLock(m_Mutex);
	m_pKey->setAlignment(alignment);
}

template <typename Widget>
inline void ToolPair<Widget>::setValueWidget(const Widget& widget)
{
	WriteLock(m_Mutex);
	if (m_pWidget)
		delete m_pWidget;
	m_pWidget = new Widget(widget);
}

template <typename Widget>
inline void ToolPair<Widget>::setFixedSize(const QSize& size)
{
	WriteLock(m_Mutex);
	m_pKey->setFixedSize(size);
	m_pWidget->setFixedSize(size);
}

template <typename Widget>
inline void ToolPair<Widget>::setFixedSize(int w, int h)
{
	WriteLock(m_Mutex);
	m_pKey->setFixedSize(w, h);
	m_pWidget->setFixedSize(w, h);
}

template <typename Widget>
inline void ToolPair<Widget>::setFixedWidth(int w)
{
	WriteLock(m_Mutex);
	m_pKey->setFixedWidth(w);
	m_pWidget->setFixedWidth(w);
}

template <typename Widget>
inline void ToolPair<Widget>::setFixedHeight(int h)
{
	WriteLock(m_Mutex);
	m_pKey->setFixedHeight(h);
	m_pWidget->setFixedHeight(h);
}

template <typename Widget>
inline void ToolPair<Widget>::setMinimumSize(const QSize &size)
{
	WriteLock(m_Mutex);
	m_pKey->setMinimumSize(size);
	m_pWidget->setMinimumSize(size);
}

template <typename Widget>
inline void ToolPair<Widget>::setMinimumSize(int w, int h)
{
	WriteLock(m_Mutex);
	m_pKey->setMinimumSize(w, h);
	m_pWidget->setMinimumSize(w, h);
}

template <typename Widget>
inline void ToolPair<Widget>::setMinimumWidth(int w)
{
	WriteLock(m_Mutex);
	m_pKey->setMinimumWidth(w);
	m_pWidget->setMinimumWidth(w);
}

template <typename Widget>
inline void ToolPair<Widget>::setMinimumHeight(int h)
{
	WriteLock(m_Mutex);
	m_pKey->setMinimumHeight(h);
	m_pWidget->setMinimumHeight(h);
}

template <typename Widget>
inline void ToolPair<Widget>::setMaxiumSize(const QSize &size)
{
	WriteLock(m_Mutex);
	m_pKey->setMaxiumSize(size);
	m_pWidget->setMaxiumSize(size);
}

template <typename Widget>
inline void ToolPair<Widget>::setMaxiumSize(int w, int h)
{
	WriteLock(m_Mutex);
	m_pKey->setMaxiumSize(w, h);
	m_pWidget->setMaxiumSize(w, h);
}

template <typename Widget>
inline void ToolPair<Widget>::setMaxiumWidth(int w)
{
	WriteLock(m_Mutex);
	m_pKey->setMaxiumWidth(w);
	m_pWidget->setMaxiumWidth(w);
}

template <typename Widget>
inline void ToolPair<Widget>::setMaxiumHeight(int h)
{
	WriteLock(m_Mutex);
	m_pKey->setMaxiumHeight(h);
	m_pWidget->setMaxiumHeight(h);
}

template <typename Widget>
inline void ToolPair<Widget>::setStretch(int firstStretch, int secondStretch)
{
	WriteLock(m_Mutex);
	m_pHLayout->setStretch(0, firstStretch);
	m_pHLayout->setStretch(1, secondStretch);
}

template <typename Widget>
inline void ToolPair<Widget>::Init(const QString& text)
{
	WriteLock(m_Mutex);
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