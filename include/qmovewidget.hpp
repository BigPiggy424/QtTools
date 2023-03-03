#ifndef _Q_MOVE_WIDGET_HPP_
#define _Q_MOVE_WIDGET_HPP_

#include <QObject>
#include <QWidget>
#include <QMouseEvent>

/* @brief 为QWidget添加鼠标拖拽功能 */
#define QMoveWidget                                                          \
private:                                                                     \
void mousePressEvent(QMouseEvent *event)                                     \
{                                                                            \
    m_isPressed = true;                                                      \
    m_startMovePos = event->globalPos() - this->frameGeometry().topLeft();   \
}                                                                            \
void mouseMoveEvent(QMouseEvent *event)                                      \
{                                                                            \
    if(m_isPressed)                                                          \
    {                                                                        \
        move(this->mapToGlobal(event->pos() - m_startMovePos));              \
    }                                                                        \
}                                                                            \
void mouseReleaseEvent(QMouseEvent *)                                        \
{                                                                            \
    m_isPressed = false;                                                     \
}                                                                            \
private:                                                                     \
    bool m_isPressed = false;                                                \
    QPoint m_startMovePos;

#endif // _Q_MOVE_WIDGET_HPP_