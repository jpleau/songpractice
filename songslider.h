/*
 * songslider.h
 * Copyright (C) 2017  Jason Pleau <jason@jpleau.ca>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SONGSLIDER_H
#define SONGSLIDER_H

#include <QSlider>
#include <QMouseEvent>

#include <QDebug>

class SongSlider : public QSlider {
	Q_OBJECT
public:
	SongSlider(QWidget *parent = 0); 
	~SongSlider();
	
	bool isSliding() const;
	
signals:
	void onClick(int val);
	void onMove(int val);

protected:
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	
	bool sliding;
	
};

#endif // SONGSLIDER_H