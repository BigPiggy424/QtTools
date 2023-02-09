#include <QBoxLayout>

#include "ui_toolbox.h"
#include "toolbox.hpp"
#include "toolpage.hpp"

ToolBox::ToolBox(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::ToolBoxClass())
	, m_pScrollAreaWidget(new QWidget())
	, m_pScrollAreaVBoxLayout(new QVBoxLayout(m_pScrollAreaWidget))
	, m_pContentVBoxLayout(new QVBoxLayout())
	, m_mapToolPageList(ToolPageList())
{
	ui->setupUi(this);

	m_pContentVBoxLayout->setContentsMargins(0, 0, 0, 0);
	m_pContentVBoxLayout->setSpacing(3);

	m_pScrollAreaVBoxLayout->setContentsMargins(0, 0, 0, 0);
	m_pScrollAreaVBoxLayout->addLayout(m_pContentVBoxLayout);
	m_pScrollAreaVBoxLayout->addStretch(1);
	
	ui->scrollArea->setWidget(m_pScrollAreaWidget);
}

ToolBox::~ToolBox()
{
	if (m_pContentVBoxLayout != nullptr)
		m_pContentVBoxLayout->deleteLater();
	if (m_pScrollAreaVBoxLayout != nullptr)
		m_pScrollAreaVBoxLayout->deleteLater();
	if (m_pScrollAreaWidget != nullptr)
		m_pScrollAreaWidget->deleteLater();
	delete ui;
}

void ToolBox::addPage(const QString& title)
{
	m_mapToolPageList.emplace(std::make_pair(title, std::make_shared<ToolPage>(title, this)));
	m_pContentVBoxLayout->addWidget(m_mapToolPageList.at(title).get());
}

void ToolBox::addWidget(const QString& title, QWidget* widget)
{
	if (m_mapToolPageList.find(title) == m_mapToolPageList.end())
		addPage(title);
	std::shared_ptr<ToolPage> page = m_mapToolPageList.at(title);
	page->addWidget(widget);
}

void ToolBox::removePage(const QString& title)
{
	if (m_mapToolPageList.find(title) == m_mapToolPageList.end())
		return;
	m_pContentVBoxLayout->removeWidget(m_mapToolPageList.at(title).get());
	m_mapToolPageList.erase(title);
}

void ToolBox::removeWidget(const QString& title, QWidget* widget)
{
	if (m_mapToolPageList.find(title) == m_mapToolPageList.end())
		return;
	std::shared_ptr<ToolPage> page = m_mapToolPageList.at(title);
	page->removeWidget(widget);
}

void ToolBox::setContentsMargins(const QMargins &margins)
{
	m_pContentVBoxLayout->setContentsMargins(margins);
}

void ToolBox::setContentsMargins(int left, int top, int right, int bottom)
{
	m_pContentVBoxLayout->setContentsMargins(left, top, right, bottom);
}

void ToolBox::setSpacing(int spacing)
{
	m_pContentVBoxLayout->setSpacing(spacing);
}
