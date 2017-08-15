TEMPLATE = app
TARGET = name_of_the_app

QT = core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    main.cpp \
    game.cpp \
    location_piece_square.cpp \
    state.cpp \
    qsquare.cpp \
    promotion_button.cpp \
    qgame.cpp \
    buttons.cpp
HEADERS += \
    game.h \
    location_piece_square.h \
    state.h \
    qsquare.h \
    promotion_button.h \
    qgame.h \
    buttons.h
