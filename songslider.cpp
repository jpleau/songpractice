/*
 * songslider.cpp
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

#include "songslider.h"

#include <QDebug>

SongSlider::SongSlider(QWidget* parent) : QSlider(parent) {
}

SongSlider::~SongSlider() {
}

bool SongSlider::isSliding() const {
	return sliding;
}

void SongSlider::mouseReleaseEvent(QMouseEvent* event) {
   if (event->button() == Qt::LeftButton) {   
	   int newVal = minimum() + ((maximum()-minimum()) * event->x()) / width();
	   emit onClick(newVal);
	   sliding = false;
   }
   
   QSlider::mouseReleaseEvent(event);
}

void SongSlider::mouseMoveEvent(QMouseEvent* event) {
	if (event->button() == Qt::NoButton) {
		int newVal = minimum() + ((maximum()-minimum()) * event->x()) / width();
		emit onMove(newVal);
	}
	
	QSlider::mouseMoveEvent(event);
}

void SongSlider::mousePressEvent(QMouseEvent* event) {
   if (event->button() == Qt::LeftButton) {   
		sliding = true;
   }
   
   QSlider::mousePressEvent(event);
}

