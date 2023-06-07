#ifndef _TOOL_PAGE_HPP_
#define _TOOL_PAGE_HPP_

#include <functional>
#include <unordered_map>

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class ToolPageClass; };
QT_END_NAMESPACE

class QHBoxLayout;
class QLabel;
class QPushButton;

class ToolPage : public QWidget
{
	Q_OBJECT

public:
	explicit ToolPage(const QString& title, QWidget* parent = nullptr);
	~ToolPage();

	void setDoubleClickedEvent(std::function<void(QPushButton*)> _Func);

	QWidget* operator[](const QString& name) const;

public slots:
	QWidget* getWidget(const QString& name) const;
	bool addWidget(const QString& name, QWidget* widget);
	void removeWidget(const QString& name);
	void expand();
	void collapse();

protected slots:
	void on_pushButtonFold_clicked();

private:
	Ui::ToolPageClass* ui;
	bool               m_bIsExpanded;
	QLabel*            m_pLabel;
	QHBoxLayout*       m_pLayout;
	std::unordered_map<QString, QWidget*> m_widgetMap;
};

#endif // !_TOOL_PAGE_HPP_