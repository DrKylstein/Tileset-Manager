/*
 *  Tileset Manager, an editor for TILHEAD.CK1 files in the V2 patch for
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

#ifndef TILEHEADMODEL_HPP
#define TILEHEADMODEL_HPP

#include <QDataStream>
#include <QAbstractListModel>
#include <QStringList>

class TileHeadModel: public QAbstractListModel {

	Q_OBJECT

	public:
		void reset(void);
		bool load(QDataStream&);
		void dump(QDataStream&) const;
		void setLevelCount(int);

		int rowCount(const QModelIndex & parent = QModelIndex()) const;
		QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
		QVariant data (const QModelIndex & index, int role = Qt::DisplayRole) const;
		Qt::ItemFlags flags (const QModelIndex & index) const;
		bool setData (const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

		TileHeadModel(QObject* parent=0);

	private:
		static const int ENTRY_SIZE = 16;
		static const int ENTRY_COUNT = 91;
		static const int MAX_LEVEL_COUNT = 16;
		static const int EXTENDED_LEVEL_COUNT = 32;
		static const int TITLE_LEVEL = 90;
		static const int MAP_LEVEL = 80;
		static const int END_LEVEL = 81;
		QStringList _data;
		bool _extendedLevels;
};
#endif
