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

#include "MainWindow.hpp"

MainWindow::MainWindow(QWidget * parent, Qt::WindowFlags flags): QMainWindow(parent, flags) {
	QTableView* view = new QTableView();
	view->setModel(&_model);
	view->setAlternatingRowColors(true);
	view->horizontalHeader()->setVisible(false);
	view->horizontalHeader()->setStretchLastSection(true);
	view->verticalHeader()->setVisible(true);
	view->setEditTriggers(QAbstractItemView::NoEditTriggers);
	setCentralWidget(view);
	connect(view, SIGNAL(activated(const QModelIndex&)), this, SLOT(_edit(const QModelIndex&)));

	QAction* newAction = new QAction(QIcon(":/images/new.png"), tr("&New"), this);
	newAction->setShortcuts(QKeySequence::New);
	connect(newAction, SIGNAL(triggered()), this, SLOT(_newFile()));

	QAction* openAction = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
	openAction->setShortcuts(QKeySequence::Open);
	connect(openAction, SIGNAL(triggered()), this, SLOT(_openFile()));

	QAction* saveAction = new QAction(QIcon(":/images/save.png"), tr("&Save"), this);
	saveAction->setShortcuts(QKeySequence::Save);
	connect(saveAction, SIGNAL(triggered()), this, SLOT(_saveFile()));

	QAction* saveAsAction = new QAction(QIcon(":/images/save-as.png"), tr("Save &As..."), this);
	saveAsAction->setShortcuts(QKeySequence::SaveAs);
	connect(saveAsAction, SIGNAL(triggered()), this, SLOT(_saveFileAs()));

	QAction* quitAction = new QAction(QIcon(":/images/exit.png"), tr("&Quit"), this);
	quitAction->setShortcut(QKeySequence(tr("Ctrl+Q")));
	connect(quitAction, SIGNAL(triggered()), this, SLOT(close()));

	QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(newAction);
	fileMenu->addAction(openAction);
	fileMenu->addAction(saveAction);
	fileMenu->addAction(saveAsAction);
	fileMenu->addSeparator();
	fileMenu->addAction(quitAction);
}
/*~MainWindow::MainWindow(void) {

}*/

void MainWindow::_newFile(void) {
	_model.reset();
}
void MainWindow::_openFile(void) {
	QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), _directory,
		tr("V2 tilehead file (tilehead.ck? TILEHEAD.CK? tilehead.CK?)"));
	if(filename.isEmpty()) {
		return;
	}
	QFile file(filename);
	if(!file.open(QIODevice::ReadOnly)) {
		QMessageBox::critical(this, tr("File Error"),
			tr("The file could not be loaded."));
		return;
	}
	QDataStream stream(&file);
	if(!_model.load(stream)) {
		QMessageBox::critical(this, tr("File Error"), tr("The specified file could not be opened."));
	}
	_filename = filename;
	_directory = QFileInfo(filename).canonicalPath();
}
void MainWindow::_saveFile(void) {
	if(_filename.isEmpty()) {
		_saveFileAs();
		return;
	}
	QFile file(_filename);
	if(!file.open(QIODevice::WriteOnly)) {
		QMessageBox::critical(this, tr("File Error"), tr("The file could not be saved to."));
		return;
	}
	QDataStream stream(&file);
	_model.dump(stream);
}
void MainWindow::_saveFileAs(void) {
	QString filename = QFileDialog::getSaveFileName(this, tr("Save File"), _directory,
		tr("V2 tilehead file (tilehead.ck? TILEHEAD.CK? tilehead.CK?)"));
	if(filename.isEmpty()) {
		return;
	}
	QFile file(filename);
	if(!file.open(QIODevice::WriteOnly)) {
		QMessageBox::critical(this, tr("File Error"), tr("The specified file could not be saved to."));
		return;
	}
	QDataStream stream(&file);
	_model.dump(stream);
	_filename = filename;
	_directory = QFileInfo(filename).canonicalPath();
}

void MainWindow::_edit(const QModelIndex& index) {
	QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), _directory,
		tr("V2 tileset file (*.tls *.TLS)"));
	if(filename.isEmpty()) {
		return;
	}
	filename = QFileInfo(filename).fileName();
	QStringList split = filename.split(".");
	if(split.size() != 2 || split[0].size() > 8 || split[1].size() != 3) {
		QMessageBox::critical(this, tr("Filename Error"),
			tr("Files must have names no more than eight letters long, with a 3 letter extension."));
		return;
	}
	_model.setData(index, filename);
}
