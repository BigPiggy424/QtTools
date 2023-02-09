#ifndef _TOOL_BOX_HPP_
#define _TOOL_BOX_HPP_

#include <map>
#include <memory>

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class ToolBoxClass; };
QT_END_NAMESPACE

class QVBoxLayout;
class ToolPage;

typedef std::map<QString, std::shared_ptr<ToolPage>> ToolPageList;

class __declspec(dllexport) ToolBox : public QWidget
{
	Q_OBJECT

public:
	explicit ToolBox(QWidget* parent = nullptr);
	~ToolBox();
	void addPage(const QString& title);
	void addWidget(const QString& title, QWidget* widget);
	void removePage(const QString& title);
	void removeWidget(const QString& title, QWidget* widget);
	void setContentsMargins(const QMargins& margins);
	void setContentsMargins(int left, int top, int right, int bottom);
	void setSpacing(int spacing);

private:
	Ui::ToolBoxClass* ui;
	QWidget*     m_pScrollAreaWidget;     // ScrollArea的控件
	QVBoxLayout* m_pScrollAreaVBoxLayout; // ScrollArea的布局
	QVBoxLayout* m_pContentVBoxLayout;    // 控件列表布局
	ToolPageList m_mapToolPageList;       // 存放ToolPage的列表
};

#endif // !_TOOL_BOX_HPP_