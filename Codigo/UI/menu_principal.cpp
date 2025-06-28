// menuprincipal.cpp
#include "menu_principal.h"
#include <QVBoxLayout>

MenuPrincipal::MenuPrincipal(QWidget* parent) : QWidget(parent) {
    resize(400, 400);
    QVBoxLayout* layout = new QVBoxLayout(this);

    QPushButton* btn1 = new QPushButton("Jugar Nivel 1");
    QPushButton* btn2 = new QPushButton("Jugar Nivel 2");

    layout->addWidget(btn1);
    layout->addWidget(btn2);

    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene, this);
    layout->addWidget(view);

    juego = new Juego(this);

    connect(btn1, &QPushButton::clicked, this, &MenuPrincipal::jugarNivel1);
    connect(btn2, &QPushButton::clicked, this, &MenuPrincipal::jugarNivel2);
}

void MenuPrincipal::jugarNivel1() {
    juego->cambiarNivel("nivel1");
}

void MenuPrincipal::jugarNivel2() {
    juego->cambiarNivel("nivel2");
}
