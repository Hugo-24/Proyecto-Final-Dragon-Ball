#include "Juego.h"
#include <QVBoxLayout>

Juego::Juego(QWidget *parent) : QWidget(parent), nivelActual(nullptr) {
    niveles["nivel1"] = new Nivel1(this);
    niveles["nivel2"] = new Nivel2(this);
}

void Juego::cambiarNivel(const std::string& id) {
    if (nivelActual) {
        layout()->removeWidget(nivelActual);
        nivelActual->hide();
    }

    nivelActual = niveles[id];
    QVBoxLayout* l = new QVBoxLayout(this);
    l->addWidget(nivelActual);
    setLayout(l);
    nivelActual->show();
}
