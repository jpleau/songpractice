﻿/*
 * mainwindow.h
 * Copyright (C) 2016  Jason Pleau <jason@jpleau.ca>
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "settings.h"
#include "song.h"

#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QVector>

#include <memory>

namespace Ui {
class MainWindow;
}

class SongList;
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
	
private:
    Ui::MainWindow *ui;
	std::unique_ptr<SongList> song_model;
	QMediaPlayer player;
	
	int get_selected_row() const;

	void playSong(int pos);
	
	void deleteSong();
	
	Settings settings;
	
	bool eventFilter(QObject *object, QEvent *event);
	
	std::unique_ptr<QMediaPlaylist> playlist;
	
	QString playing;
	
	Song *current_song = nullptr;
	
	void update_label(int secs);
	
	void toggle_loop_fields(bool enabled);
	
	int loop_from;
	int loop_to;
	
	QString last_song_path;
};

#endif
