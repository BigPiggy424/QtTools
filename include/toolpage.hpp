#ifndef _TOOL_PAGE_HPP_
#define _TOOL_PAGE_HPP_

#include <functional>

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

public slots:
	void addWidget(QWidget* widget);
	void removeWidget(QWidget* widget);
	void expand();
	void collapse();

protected slots:
	void on_pushButtonFold_clicked();

private:
	Ui::ToolPageClass* ui;
	bool               m_bIsExpanded;
	QLabel*            m_pLabel;
	QHBoxLayout*       m_pLayout;
};

#endif // !_TOOL_PAGE_HPP_