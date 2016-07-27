/*
 * song.h
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

#ifndef SONG_H
#define SONG_H

#include <QString>

class Song {
public:
	Song() = default;
    Song(QString filename);

    QString get_artist() const;
    QString get_album() const;
    QString get_title() const;
    QString get_duration() const;
	QString get_filename() const;

	QString toString() const;
	bool is_valid() const { return valid; }
	
	bool operator == (const Song &other);
	

private:
	QString artist;
	QString album;
	QString title;
	int duration;
	QString duration_str;
	QString filename;
	
	QString seconds_to_str(int duration);
	
	bool valid;
};

#endif // SONG_H