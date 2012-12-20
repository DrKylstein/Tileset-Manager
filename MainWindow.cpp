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
	setWindowTitle(tr("Tileset Manager"));
	setWindowIcon(QIcon(":/images/icon.svg"));

	QWidget* mainWidget = new QWidget();
	QVBoxLayout* mainLayout = new QVBoxLayout();
	mainWidget->setLayout(mainLayout);

	QTableView* view = new QTableView();
	view->setModel(&_model);
	view->setAlternatingRowColors(true);
	view->horizontalHeader()->setVisible(false);
	view->horizontalHeader()->setStretchLastSection(true);
	view->verticalHeader()->setVisible(true);
	view->setEditTriggers(QAbstractItemView::NoEditTriggers);
	mainLayout->addWidget(view);
	connect(view, SIGNAL(activated(const QModelIndex&)), this, SLOT(_edit(const QModelIndex&)));

	QCheckBox* extended = new QCheckBox(tr("Using 32-level patch"));
	connect(extended, SIGNAL(toggled(bool)), &_model, SLOT(setExtendedLevels(bool)));
	mainLayout->addWidget(extended);

	setCentralWidget(mainWidget);


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

	QAction* aboutAction = new QAction(QIcon(":/images/about.png"), tr("&About..."), this);
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(_about()));

	_helpViewer.setSource(QUrl("qrc:/docs/index.html"));
	_helpViewer.setWindowTitle("Tileset Manager Help");

	QAction* helpAction = new QAction(QIcon(":/images/help.png"), tr("&Contents"), this);
	helpAction->setShortcuts(QKeySequence::HelpContents);
	connect(helpAction, SIGNAL(triggered()), &_helpViewer, SLOT(show()));


	QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(newAction);
	fileMenu->addAction(openAction);
	fileMenu->addAction(saveAction);
	fileMenu->addAction(saveAsAction);
	fileMenu->addSeparator();
	fileMenu->addAction(quitAction);

	menuBar()->addSeparator();

	QMenu* helpMenu = menuBar()->addMenu(tr("&Help"));
	helpMenu->addAction(aboutAction);
	helpMenu->addAction(helpAction);

	QSettings settings(tr("Tileset Manager"), QString(), this);
	settings.beginGroup("Window");
		resize(settings.value("Size", QSize(976, 506)).toSize());
		move(settings.value("Position", QPoint(200, 200)).toPoint());
		restoreState(settings.value("State").toByteArray());
		settings.endGroup();
	settings.beginGroup("Help_Window");
		_helpViewer.resize(settings.value("Size", QSize(500, 600)).toSize());
		_helpViewer.move(settings.value("Position", QPoint(200, 200)).toPoint());
		settings.endGroup();
	_directory = settings.value("Working_Directory", QString()).toString();

}
MainWindow::~MainWindow(void) {
	QSettings settings(tr("Tileset Manager"), QString(), this);
	settings.beginGroup("Window");
		settings.setValue("Size", size());
		settings.setValue("Position", pos());
		settings.setValue("State", saveState());
		settings.endGroup();
	settings.beginGroup("Help_Window");
		settings.setValue("Size", _helpViewer.size());
		settings.setValue("Position", _helpViewer.pos());
		settings.endGroup();
	settings.setValue("Working_Directory", _directory);
}

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

void MainWindow::_about(void) {
	QMessageBox::about(this, tr("About Tileset Manager"), tr(
		"<p><b>Tileset Manager</b>, an editor for TILHEAD.CK1 files in the V2 patch for Commander Keen: Invasion of the Vorticons.<br />"
		"Copyright &copy; 2012 Kyle Delaney.</p>"

		"<p>Tileset Manager is free software: you can redistribute it and/or modify "
		"it under the terms of the GNU General Public License as published by "
		"the Free Software Foundation, either version 3 of the License, or "
		"(at your option) any later version.</p>"

		"<p>Tileset Manager is distributed in the hope that it will be useful, "
		"but WITHOUT ANY WARRANTY; without even the implied warranty of "
		"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the "
		"GNU General Public License for more details.</p>"

		"<p>You should have received a copy of the GNU General Public License "
		"along with this program.  If not, see <a href='http://www.gnu.org/licenses/'>www.gnu.org/licenses/</a>.</p>"
	));
}
