#include "Juego.h"
#include <QVBoxLayout>
Juego::Juego(QWidget *parent) : QWidget(parent), nivelActual(nullptr) {
    // Crear los niveles y almacenarlos en el mapa
    niveles["nivel1"] = new Nivel1(this);
    niveles["nivel2"] = new Nivel2(this);
}

void Juego::cambiarNivel(const std::string& id) {
    // Ocultar y eliminar nivel anterior si existe
    if (nivelActual) {
        layout()->removeWidget(nivelActual);
        nivelActual->hide();
        nivelActual->deleteLater();
    }

    // Crear nuevo nivel
    if (id == "nivel1") {
        nivelActual = new Nivel1(this);
    } else if (id == "nivel2") {
        nivelActual = new Nivel2(this);
    } else {
        nivelActual = nullptr;
        return;
    }

    // Conectar señal para regresar al menú
    connect(nivelActual, &Nivel::regresarAlMenu, this, [=]() {
        emit regresarAlMenu();
    });

    nivelActual->cargarNivel();

    if (!layout()) {
        QVBoxLayout* l = new QVBoxLayout(this);
        l->setContentsMargins(0, 0, 0, 0);
        setLayout(l);
    }

    layout()->addWidget(nivelActual);
    nivelActual->show();
}
