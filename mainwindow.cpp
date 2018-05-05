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

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), loop_from(0), loop_to(0) {
    ui->setupUi(this);

    song_model.reset(new SongList);

    connect(ui->button_open, &QPushButton::clicked, [=]() {
        QStringList filenames = QFileDialog::getOpenFileNames(nullptr, QString(), settings.getLastDir());
		QDir currentDir;
		if (!filenames.isEmpty()) {
			for (auto filename : filenames) {
				if (song_model->add_song(filename)) {
					settings.addPath(filename);
					settings.setLastDir(currentDir.absoluteFilePath(filename));
				}	
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
	
	connect(ui->button_pause, &QPushButton::clicked, [=]() {
		switch (player.state()) {
			case QMediaPlayer::PlayingState:
				player.pause();
				break;
				
			case QMediaPlayer::PausedState:
				player.play();
				break;
				
			default:
				return;
		}

	});

    ui->table->setModel(song_model.get());

    ui->table->horizontalHeader()->setStretchLastSection(true);

    ui->table->installEventFilter(this);

    for (auto &p : settings.getPaths()) {
        song_model->add_song(p);
    }
	
	toggle_loop_fields(false);

    playlist.reset(new QMediaPlaylist);
	
	ui->table->horizontalHeader()->setSectionResizeMode (QHeaderView::Interactive);
	
	
	ui->slider->setValue(0);
	connect(&player, &QMediaPlayer::positionChanged, [=](auto position) {
		ui->position_label->setText("00:00 / 00:00");
		if (playlist->nextIndex() == -1) {
			if (current_song != nullptr) {
				if (!ui->slider->isSliding()) {
					int secs = static_cast<int>(position / 1000);
					this->update_label(secs);
					if (ui->looping->isChecked()) {
						if (loop_from > 0 && loop_to > loop_from) {
							if (secs > loop_to) {
								player.setPosition(loop_from * 1000);
							}
						}
					}
					
				}
			}
		}
	});
	
	connect (&player, &QMediaPlayer::currentMediaChanged, [=](auto ...) {
		ui->slider->setEnabled(playlist->nextIndex() == -1);
		ui->position_label->setText("00:00 / 00:00");
	});
	
	connect(ui->slider, &SongSlider::onClick, [=](int val) {
		int value = val;
		player.setPosition(value * 1000);
	});
	
	connect(ui->slider, &SongSlider::onMove, [=](int secs) {
		update_label(secs);
	});
	
	connect(ui->looping, &QCheckBox::stateChanged, [=](auto state) {
		this->toggle_loop_fields(state == Qt::Checked);
	});
	
	connect(ui->loop_set_from, &QPushButton::clicked, [=]() {
		int secs = static_cast<int>(player.position() / 1000);
		this->loop_from = secs;
		QString val = Song::seconds_to_str(secs);
		ui->loop_from->setText(val);
	});
	
	connect(ui->loop_set_to, &QPushButton::clicked, [=]() {
		int secs = static_cast<int>(player.position() / 1000);
		this->loop_to = secs;
		QString val = Song::seconds_to_str(secs);
		ui->loop_to->setText(val);
	});
}

MainWindow::~MainWindow() {
	current_song = nullptr;
	player.stop();
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
    
    current_song = &song_model->get_song(pos);
	
	QString path = current_song->get_filename();
	
	if (path != last_song_path) {
		loop_from = loop_to = 0;
		ui->loop_from->setText("00:00");
		ui->loop_to->setText("00:00");
		ui->looping->setChecked(false);
	}
	
	last_song_path = path;
    playlist->addMedia(QUrl::fromLocalFile(path));
    player.setPlaylist(playlist.get());
    player.play();
	
	int duration = current_song->get_duration();
	
	ui->slider->setRange(0, duration);
	ui->slider->setValue(0);
	
	song_model->setPlaying(current_song->get_filename());
}

void MainWindow::deleteSong() {
    int pos = get_selected_row();
    if (pos > -1) {
        Song &song = song_model->get_song(pos);
        settings.removePath(song.get_filename());
		
		if (song_model->getPlaying() == song.get_filename()) {
			player.stop();
			ui->slider->setEnabled(false);
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

        case Qt::Key_Space: {
			bool beginning = ui->looping->isChecked() && 
					loop_from >= 0 && loop_to > loop_from && 
					QGuiApplication::keyboardModifiers() & Qt::ControlModifier;

			beginning = true;
			if (beginning) {
				player.stop();
				playlist->setCurrentIndex(0);
				player.play();
			} else{
				player.setPosition(loop_from);
			}
            return true;
		}
			
		case Qt::Key_Escape:
			player.stop();
			playlist->setCurrentIndex(0);
			return true;
        }
    }

	return QWidget::eventFilter(object, event);
}

void MainWindow::update_label(int secs) {
	ui->slider->setValue(secs);
	QString first = Song::seconds_to_str(secs);
	QString second = current_song->get_duration_str();
	ui->position_label->setText(first + " / " + second);
}

void MainWindow::toggle_loop_fields(bool enabled) {
	ui->loop_from->setEnabled(enabled);
	ui->loop_to->setEnabled(enabled);
	ui->loop_set_from->setEnabled(enabled);
	ui->loop_set_to->setEnabled(enabled);
}


