TEMPLATE = app
CONFIG += debug_and_release qt

DESTDIR = ./bin
OBJECTS_DIR = ./obj

RESOURCES = tileset_manager.qrc

HEADERS +=	MainWindow.hpp \
            TileHeadModel.hpp



SOURCES +=	main.cpp \
			MainWindow.cpp \
			TileHeadModel.cpp
