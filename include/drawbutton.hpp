/**
 * @file drawbutton.hpp
 * @author ldk
 * @brief 
 * @version 0.1
 * @date 2023-02-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef _DRAW_BUTTON_HPP_
#define _DRAW_BUTTON_HPP_

#include <QPen>
#include <QPushButton>

class DrawWidget;

class DrawButton : public QPushButton
{
	Q_OBJECT

public:
	DrawButton(QWidget* parent);
	~DrawButton();

	inline const QPen& getPen() const { return m_Pen; }
	inline void setPen(const QPen& pen) { m_Pen = pen; }
	inline void setPen(const QColor& color, qreal penSize) { m_Pen = QPen(color, penSize); }
	inline void setCanvas(QWidget* canvas) { m_Canvas = canvas; }
	inline bool drawing() { return m_pDrawWidget != nullptr; }

public slots:
	void startDraw(const QRect& canvas);
	void startDraw(int canvas_x, int canvas_y, int canvasWidth, int canvasHeight);
	void startDraw(QWidget* canvas);
	void finishDraw();

	inline void startDraw() { startDraw(m_Canvas); }

	inline const QPointF& getStartPoint() const noexcept { return m_start; }
	inline const QPointF& getStopPoint() const noexcept { return m_stop; }

protected slots:
	void setPoints();

private:
	QPen        m_Pen;
	DrawWidget* m_pDrawWidget;
	QWidget*    m_Canvas;
	QPointF     m_start;
	QPointF     m_stop;
};

#endif // !_DRAW_BUTTON_HPP_