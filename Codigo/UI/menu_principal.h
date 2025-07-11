// menu_principal.h
#pragma once
#include "juego.h"
#include <QWidget>
#include <QPushButton>
#include <QGraphicsView>
#include <QGraphicsScene>

class MenuPrincipal : public QWidget {
    Q_OBJECT
public:
    MenuPrincipal(QWidget* parent = nullptr);

private slots:
    void jugarNivel1();
    void jugarNivel2();
    void jugarNivel3();

private:
    QGraphicsView* view;
    QGraphicsScene* scene;
    Juego* juego;
};
