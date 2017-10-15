/*
 * songlist.h
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

#ifndef SONGLIST_H
#define SONGLIST_H

#include "song.h"
#include <QAbstractTableModel>
#include <QVector>

class SongList : public QAbstractTableModel {
    Q_OBJECT

	enum : int {
		Duration = 0,
		Title = 1,
		Artist = 2,
		Album = 3,
	};

public:
    explicit SongList(QObject *parent = 0);
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	
	bool add_song(QString filename);
	void remove_song(int pos);
	Song & get_song(int pos);
	
	void setPlaying(QString playing);
	QString getPlaying() const;
	
	void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;

private:
	QVector<Song> songs;
	QString playing;
	
	void refresh();
};

#endif