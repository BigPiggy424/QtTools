#include <QHBoxLayout>
#include <QLabel>

#include "ui_toolpage.h"
#include "toolpage.hpp"

ToolPage::ToolPage(const QString& title, QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::ToolPageClass())
    , m_bIsExpanded(true)
    , m_pLabel(new QLabel())
    , m_pLayout(nullptr)
    , m_widgetMap{}
{
	ui->setupUi(this);

    ui->pushButtonFold->setText(title);
    ui->widgetContent->setAttribute(Qt::WA_StyledBackground);

    m_pLabel->setFixedSize(20, 20);
    m_pLabel->setPixmap
    (
        QPixmap(":/icons/down-arrow.png").scaled
        (
            m_pLabel->size(),
            Qt::IgnoreAspectRatio,
            Qt::SmoothTransformation
        )
    );
    m_pLayout = new QHBoxLayout(ui->pushButtonFold);
    m_pLayout->setContentsMargins(0, 0, 5, 0);
    m_pLayout->addStretch(1);
    m_pLayout->addWidget(m_pLabel);

}

ToolPage::~ToolPage()
{
    if (m_pLabel != nullptr)
        m_pLabel->deleteLater();
    if (m_pLayout != nullptr)
        m_pLayout->deleteLater();
	delete ui;
}

void ToolPage::setDoubleClickedEvent(std::function<void(QPushButton*)> _Func)
{
    ui->pushButtonFold->setDoubleClickedEvent(_Func);
}

QWidget* ToolPage::getWidget(const QString& name) const
{
    if (m_widgetMap.find(name) == m_widgetMap.end())
        return nullptr;
    return m_widgetMap.at(name);
}

bool ToolPage::addWidget(const QString& name, QWidget* widget)
{
    if (m_widgetMap.find(name) != m_widgetMap.end())
        return false;
    ui->verticalLayoutContent->addWidget(widget);
    m_widgetMap[name] = widget;
    return true;
}

void ToolPage::removeWidget(const QString &name)
{
    QWidget* widget = m_widgetMap[name];
    m_widgetMap.erase(name);
    ui->verticalLayoutContent->removeWidget(widget);
}

void ToolPage::expand()
{
    ui->widgetContent->show();
    m_bIsExpanded = true;
    m_pLabel->setPixmap
    (
        QPixmap(":/icons/down-arrow.png").scaled
        (
            m_pLabel->size(),
            Qt::IgnoreAspectRatio,
            Qt::SmoothTransformation
        )
    );
}

void ToolPage::collapse()
{
    ui->widgetContent->hide();
    m_bIsExpanded = false;
    m_pLabel->setPixmap
    (
        QPixmap(":/icons/left-arrow.png").scaled
        (
            m_pLabel->size(),
            Qt::IgnoreAspectRatio,
            Qt::SmoothTransformation
        )
    );
}

void ToolPage::on_pushButtonFold_clicked()
{
    if (m_bIsExpanded)
        collapse();
    else
        expand();
}