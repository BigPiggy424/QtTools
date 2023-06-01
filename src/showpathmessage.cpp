#include <QApplication>
#include <QClipboard>
#include <QFileInfo>
#include <QMessageBox>
#include <QPushButton>

#include "showpathmessage.hpp"

void showPathMessage(const QString& message, const QString& path, QWidget* parent)
{
    QMessageBox messageBox(QMessageBox::Information, "提示", message, QMessageBox::Close, parent);
    messageBox.setButtonText(QMessageBox::Close, "关闭");
    messageBox.setDetailedText(path);
    QPushButton* copyButton = messageBox.addButton("复制", QMessageBox::ActionRole);
    messageBox.exec();
    if (messageBox.clickedButton() == copyButton) {
        QString dirPath = QFileInfo(path).absolutePath();
        QApplication::clipboard()->setText(dirPath);
        QMessageBox::information(parent ? parent : &messageBox, "提示", "已复制文件夹路径：" + dirPath);
    }
}