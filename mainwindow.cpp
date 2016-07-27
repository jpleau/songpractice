/*
 * mainwindow.cpp
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

#include "mainwindow.h"
#include "song.h"
#include "songlist.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QFileDialog>
#include <QKeyEvent>
#include <QMediaPlaylist>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    song_model.reset(new SongList);

    connect(ui->button_open, &QPushButton::clicked, [=]() {
        QString filename = QFileDialog::getOpenFileName(nullptr, QString(), settings.getLastDir());
		if (!filename.isEmpty()) {
			QDir currentDir;
			if (song_model->add_song(filename)) {
				settings.addPath(filename);
				settings.setLastDir(currentDir.absoluteFilePath(filename));
			}	
		}
    });

    connect(ui->button_delete, &QPushButton::clicked, [=]() { deleteSong(); });

    connect(ui->table, &QTableView::doubleClicked, [=](const QModelIndex &index) { playSong(index.row()); });

    connect(ui->button_play, &QPushButton::clicked, [=]() {
        int pos = get_selected_row();
        if (pos > -1) {
            playSong(pos);
        }
    });

    connect(ui->button_stop, &QPushButton::clicked, [=]() { player.stop(); });

    ui->table->setModel(song_model.get());

    ui->table->horizontalHeader()->setStretchLastSection(true);

    ui->table->installEventFilter(this);

    ui->button_play->setIcon(QIcon::fromTheme("media-playback-start"));
    ui->button_stop->setIcon(QIcon::fromTheme("media-playback-stop"));
    ui->button_open->setIcon(QIcon::fromTheme("document-open"));
    ui->button_delete->setIcon(QIcon::fromTheme("edit-delete"));

    for (auto &p : settings.getPaths()) {
        song_model->add_song(p);
    }

    playlist.reset(new QMediaPlaylist);
	
	ui->table->horizontalHeader()->setSectionResizeMode (QHeaderView::Interactive);
	ui->table->setSortingEnabled(true);
}

MainWindow::~MainWindow() {
    delete ui;
}

int MainWindow::get_selected_row() const {
    auto selection = ui->table->selectionModel();
    if (selection->hasSelection() && song_model->rowCount() > 0) {
        return selection->currentIndex().row();
    }
    return -1;
}

void MainWindow::playSong(int pos) {
    playlist->clear();
	
	if (ui->countdown->isChecked()) {
		playlist->addMedia(QUrl("qrc:/count.ogg"));	
	}
    
    Song &song = song_model->get_song(pos);
    playlist->addMedia(QUrl::fromLocalFile(song.get_filename()));
    player.setPlaylist(playlist.get());
    player.play();
	song_model->setPlaying(song.get_filename());
}

void MainWindow::deleteSong() {
    int pos = get_selected_row();
    if (pos > -1) {
        Song &song = song_model->get_song(pos);
        settings.removePath(song.get_filename());
		
		if (song_model->getPlaying() == song.get_filename()) {
			player.stop();
		}
		
		song_model->remove_song(pos);
    }
}

bool MainWindow::eventFilter(QObject *object, QEvent *event) {
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *key_event = dynamic_cast<QKeyEvent *>(event);
        switch (key_event->key()) {
        case Qt::Key_Delete:
            deleteSong();
            return true;

        case Qt::Key_Space:
            player.stop();
            playlist->setCurrentIndex(0);
            player.play();
            return true;
        }
    }

	return QWidget::eventFilter(object, event);
}
