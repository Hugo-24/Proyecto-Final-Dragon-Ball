#pragma once
#include <QMainWindow>
#include "juego.h"
#include "menu_principal.h"

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
