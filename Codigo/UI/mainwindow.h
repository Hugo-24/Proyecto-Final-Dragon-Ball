#pragma once
#include <QMainWindow>
#include "juego.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void iniciarNivel1();
    void iniciarNivel2();

private:
    Juego* juego;
};
