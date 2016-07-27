/*
 * songlist.cpp
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

#include "songlist.h"

#include <QFont>
#include <QIcon>

SongList::SongList(QObject *parent) : QAbstractTableModel(parent) {
}

QVariant SongList::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            switch (section) {
            case Title:
                return QString("Title");
            case Artist:
                return QString("Artist");
            case Album:
                return QString("Album");
            case Duration:
                return QString("Duration");
            }
        }
    }
    return QVariant();
}

int SongList::rowCount(const QModelIndex &) const {
    return static_cast<int>(songs.size());
}

int SongList::columnCount(const QModelIndex &) const {
    return 4;
}

QVariant SongList::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }

    int row = index.row();
    int col = index.column();

    switch (role) {
    case Qt::DisplayRole:
        switch (col) {
        case Title:
            return songs[row].get_title();
        case Artist:
            return songs[row].get_artist();
        case Album:
            return songs[row].get_album();
        case Duration:
            return songs[row].get_duration();
        }

        break;

    case Qt::FontRole: {
        if (songs[row].get_filename() == playing) {
            QFont font;
            font.setBold(true);
            return font;
        }
    } break;

    case Qt::DecorationRole:
        if (col == Title && songs[row].get_filename() == playing) {
            return QIcon::fromTheme("media-playback-start");
        }
        break;
    }

    return QVariant();
}

bool SongList::add_song(QString filename) {
    // Check if already added
    auto index = songs.indexOf(filename);
    if (index != -1) {
        return false;
    }

    Song song(filename);
    if (song.is_valid()) {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        songs.push_back(Song(filename));
        endInsertRows();
        return true;
    }

    return false;
}

void SongList::remove_song(int pos) {
    beginRemoveRows(QModelIndex(), pos, pos);
    songs.remove(pos);
    endRemoveRows();
}

Song &SongList::get_song(int pos) {
    return songs[pos];
}

void SongList::setPlaying(QString playing) {
    this->playing = playing;
	refresh();
}

QString SongList::getPlaying() const {
    return playing;
}

void SongList::sort(int column, Qt::SortOrder order) {
    switch (column) {
    case Title:
        if (order == Qt::DescendingOrder) {
            std::sort(songs.begin(), songs.end(),
                      [=](auto &a, auto &b) { return a.get_title().compare(b.get_title()); });
        } else {
            std::sort(songs.begin(), songs.end(),
                      [=](auto &a, auto &b) { return b.get_title().compare(a.get_title()); });
        }
    }
	
	refresh();
}

void SongList::refresh() {
    auto top = createIndex(0, 0);
    auto bottom = createIndex(rowCount() - 1, 3);

    emit dataChanged(top, bottom, {Qt::DecorationRole, Qt::FontRole});
}