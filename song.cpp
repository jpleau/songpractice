/*
 * song.cpp
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

#include "song.h"

#include <taglib/fileref.h>

#include <QDebug>

Song::Song(QString filename) : filename(filename) {
    valid = true;
    TagLib::FileRef file(filename.toStdString().c_str());
    TagLib::Tag *tag = file.tag();

    if (tag) {
        artist = QString(file.tag()->artist().toCString());
        album = QString(file.tag()->album().toCString());
        title = QString(file.tag()->title().toCString());
        duration = file.audioProperties()->length();
        duration_str = seconds_to_str(duration);
    } else {
        valid = false;
    }
}

QString Song::get_artist() const {
    return artist;
}

QString Song::get_album() const {
    return album;
}

QString Song::get_title() const {
    return title;
}

QString Song::get_duration() const {
    return duration_str;
}

QString Song::get_filename() const {
    return filename;
}

QString Song::toString() const {
    return QString("{ artist: %1, album: %2, title: %3, length: %4}")
        .arg(artist)
        .arg(album)
        .arg(title)
        .arg(duration_str);
}

bool Song::operator==(const Song &other) {
	return filename == other.get_filename();
}

QString Song::seconds_to_str(int duration) {
    int minutes, seconds;
    seconds = duration % 60;
    duration -= seconds;
    minutes = duration / 60;
    return QString::asprintf("%02d:%02d", minutes, seconds);
}
