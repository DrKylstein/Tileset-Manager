/*
 *  Tileset Manager, an editor for TILEHEAD.CK? files in the V2 patch for
 *  Commander Keen: Invasion of the Vorticons.
 *  Copyright (C) 2012  Kyle Delaney
 *
 *  This file is a part of Tileset Manager.
 *
 *  Tileset Manager is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Tileset Manager is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  You may contact the author at <dr.kylstein@gmail.com>
 */

#include <QtGui>
#include "MainWindow.hpp"

int main(int argc, char* argv[]) {
	QCoreApplication::setOrganizationName("Kyle Delaney");
	QCoreApplication::setApplicationName("Tileset Manager");
	QApplication app(argc, argv);
	MainWindow window;
	window.show();
	return app.exec();
}

