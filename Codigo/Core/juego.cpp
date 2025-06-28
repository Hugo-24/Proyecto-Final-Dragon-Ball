#include "Juego.h"
#include <QVBoxLayout>

Juego::Juego(QWidget *parent) : QWidget(parent), nivelActual(nullptr) {
    // Crear los niveles y almacenarlos en el mapa
    niveles["nivel1"] = new Nivel1(this);
    niveles["nivel2"] = new Nivel2(this);
}

void Juego::cambiarNivel(const std::string& id) {
    // Si hay un nivel actualmente activo, ocultarlo y quitarlo del layout
    if (nivelActual) {
        layout()->removeWidget(nivelActual);
        nivelActual->hide();
    }

    // Buscar el nuevo nivel
    nivelActual = niveles[id];
    if (!nivelActual) return;

    // Llamar a cargarNivel() para inicializar su contenido
    nivelActual->cargarNivel();

    // Crear el layout si no existe aún
    if (!layout()) {
        QVBoxLayout* l = new QVBoxLayout(this);
        l->setContentsMargins(0, 0, 0, 0);
        setLayout(l);
    }

    layout()->addWidget(nivelActual);
    nivelActual->show();
}
