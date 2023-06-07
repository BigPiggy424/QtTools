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

using ToolPageList = std::map<QString, std::shared_ptr<ToolPage>>;

class ToolBox : public QWidget
{
	Q_OBJECT

public:
	explicit ToolBox(QWidget* parent = nullptr);
	~ToolBox();
	QWidget* getWidget(const QString& category, const QString& name) const;
	bool addPage(const QString& category);
	bool addWidget(const QString& category, const QString& name, QWidget* widget);
	void removePage(const QString& category);
	void removeWidget(const QString& category, const QString& name);
	void setContentsMargins(const QMargins& margins);
	void setContentsMargins(int left, int top, int right, int bottom);
	void setSpacing(int spacing);

	ToolPage& operator[](const QString& category) const;

private:
	Ui::ToolBoxClass* ui;
	QWidget*     m_pScrollAreaWidget;     // ScrollArea的控件
	QVBoxLayout* m_pScrollAreaVBoxLayout; // ScrollArea的布局
	QVBoxLayout* m_pContentVBoxLayout;    // 控件列表布局
	ToolPageList m_mapToolPageList;       // 存放ToolPage的列表
};

#endif // !_TOOL_BOX_HPP_