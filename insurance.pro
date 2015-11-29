#-------------------------------------------------
#
# Project created by QtCreator 2015-11-29T14:32:02
#
#-------------------------------------------------

QT       += core gui sql

CONFIG += c++14

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = insurance
TEMPLATE = app


SOURCES +=  main.cpp\
            mainwindow.cpp \
            settingsdialog.cpp \
            clientedit.cpp \
            insurancetypeedit.cpp

HEADERS +=  mainwindow.h \
            settingsdialog.h \
            clientedit.h \
            insurancetypeedit.h

FORMS   +=  mainwindow.ui \
            settingsdialog.ui \
            clientedit.ui \
            insurancetypeedit.ui
