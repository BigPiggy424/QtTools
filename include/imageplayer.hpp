/**
 * @file imageplayer.hpp
 * @author ldk
 * @brief 
 * @version 0.1
 * @date 2023-02-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef _IMAGE_PLAYER_HPP_
#define _IMAGE_PLAYER_HPP_

#include <QLabel>
#include <QWidget>

class GraphicsViewInterface;
class QHBoxLayout;
class QVBoxLayout;

enum class QColorType
{
    RGB,
    HSV
};

/**
 * @brief 设置颜色信息
 *
 * @param color 颜色
 * @param label 显示信息的控件
 * @param type 颜色类型
 */
void setColorInfo(QColor color, QLabel* label, QColorType type = QColorType::RGB);

/**
 * @brief 设置坐标信息
 *
 * @param position 坐标
 * @param label 显示信息的控件
 */
inline
void setPositionInfo(const QPoint& position, QLabel* label)
{
    QString info =
        "x: " + QString::number(position.x()) +
        "  y: " + QString::number(position.y());
    label->setText(info);
}

class ImagePlayer : public QWidget
{
    Q_OBJECT;

public:
    explicit ImagePlayer(QWidget* parent = nullptr);
    ~ImagePlayer();

    void Init();

    int width() const noexcept;
    int height() const noexcept;
    double getMinZoom() const noexcept;
    double getMaxZoom() const noexcept;
    void setMinZoom(double minZoom);
    void setMaxZoom(double maxZoom);
    void DynamicMode(int _RefreshTime = 15);
    void StaticMode();
    bool isDynamicMode() const noexcept;
    bool isStaticMode() const noexcept;
    const QImage& getImage() noexcept;
    void setImage(const QImage& image);
    QPoint getImagePosition(const QPoint& pos);

public slots:
    void setPosInfo();

private:
    GraphicsViewInterface*  m_pController;
    QVBoxLayout*            m_pImageLayout;
    QHBoxLayout*            m_pBottomLayout;
    QLabel*                 m_pPosLabel;
    QLabel*                 m_pRGBLabel;
    QLabel*                 m_pHSVLabel;
};

#endif // !_CYS_IMAGE_DISPLAY_IMAGE_PLAYER_HPP_