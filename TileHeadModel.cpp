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
#include "TileHeadModel.hpp"
#include <QDebug>

void TileHeadModel::reset(void) {
	_data.empty();
	for(int i = 0; i < ENTRY_COUNT; ++i) {
		_data << "";
	}
	for(int i = 1; i <= EXTENDED_LEVEL_COUNT; ++i) {
		_data[i] = "tileset1.tls";
	}
	_data[MAP_LEVEL] = "tileset1.tls";
	_data[END_LEVEL] = "tileset1.tls";
	_data[TITLE_LEVEL] = "tileset1.tls";
	emit dataChanged(index(0,0), index(rowCount(), 0));
}
bool TileHeadModel::load(QDataStream& stream) {
	quint8 buffer[16];
	for(int i = 0; i < ENTRY_COUNT; ++i) {
		for(int j = 0; j < ENTRY_SIZE; ++j) {
			stream >> buffer[j];
			//if(i == 1)
				//qDebug() << buffer[j];
		}
		_data[i] = QString::fromLatin1((char *)buffer);
	}
	emit dataChanged(index(0,0), index(rowCount(), 0));
	return true;
}
void TileHeadModel::dump(QDataStream& stream) const {
	for(int i = 0; i < ENTRY_COUNT; ++i) {
		stream.writeRawData(_data[i].toLatin1(), ENTRY_SIZE);
	}
}

int TileHeadModel::rowCount(const QModelIndex&) const {
	if(_extendedLevels) {
		return EXTENDED_LEVEL_COUNT+3;
	}
	return MAX_LEVEL_COUNT+3;
}
QVariant TileHeadModel::headerData ( int section, Qt::Orientation orientation, int role) const {
	if(orientation == Qt::Vertical && role == Qt::DisplayRole) {
		if(section == rowCount()-3) {
			return tr("Level 80 (Map)");
		}
		if(section == rowCount()-2) {
			return tr("Level 81 (BWB)");
		}
		if(section == rowCount()-1) {
			return tr("Level 90 (Title)");
		}
		return tr("Level %1").arg(section+1);
	}
	return QVariant();
}
QVariant TileHeadModel::data (const QModelIndex & index, int role) const {
	int section = index.row();
	if(section >= rowCount()) {
		return QVariant();
	}
	if(role != Qt::DisplayRole) {
		return QVariant();
	}
	if(section == rowCount()-3) {
		return _data[MAP_LEVEL];
	}
	if(section == rowCount()-2) {
		return _data[END_LEVEL];
	}
	if(section == rowCount()-1) {
		return _data[TITLE_LEVEL];
	}
	return _data[section+1];

}
Qt::ItemFlags TileHeadModel::flags (const QModelIndex & index) const {
	if(!index.isValid()) return Qt::ItemIsEnabled;
	return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
}
bool TileHeadModel::setData (const QModelIndex & index, const QVariant & value, int role) {
	int section = index.row();
	if(section == rowCount()-3) {
		_data[MAP_LEVEL] = value.toString();
	}
	if(section == rowCount()-2) {
		_data[END_LEVEL] = value.toString();
	}
	if(section == rowCount()-1) {
		_data[TITLE_LEVEL] = value.toString();
	}
	_data[section+1] = value.toString();
	emit dataChanged(index, index);
	return true;
}


TileHeadModel::TileHeadModel(QObject* parent): QAbstractListModel(parent), _extendedLevels(false) {
	reset();
}

void TileHeadModel::setExtendedLevels(bool state) {
	if(_extendedLevels && !state) {
		beginRemoveRows(QModelIndex(), 16, 31);
		_extendedLevels = state;
		endRemoveRows();
	} else if(!_extendedLevels && state) {
		beginInsertRows(QModelIndex(), 16, 31);
		_extendedLevels = state;
		endInsertRows();
	}
}
