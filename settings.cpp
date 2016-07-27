/*
 * settings.cpp
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

#include "settings.h"

Settings::Settings(QObject *parent)
    : QSettings(QSettings::IniFormat, QSettings::UserScope, "jpleau", "songpractice", parent) {
    paths = this->value("paths").toStringList();
	lastDir = this->value("lastDir").toString();
}

void Settings::addPath(QString path) {
    paths.push_back(path);
    this->setValue("paths", paths);
}

void Settings::removePath(QString path) {
    int pos = paths.indexOf(path);
    if (pos > -1) {
        paths.erase(paths.begin() + pos);
    }
    this->setValue("paths", paths);
}

QStringList Settings::getPaths() const {
    return paths;
}

QString Settings::getLastDir() const {
    return lastDir;
}

void Settings::setLastDir(QString dir) {
	lastDir = dir;
	this->setValue("lastDir", lastDir);
}
