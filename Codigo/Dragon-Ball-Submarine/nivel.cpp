#include "nivel.h"

Nivel::Nivel(QWidget* parent)
    : QWidget(parent), mapa(nullptr) {
    // Inicialización básica del nivel
}

Nivel::~Nivel() {
    // Limpieza si es necesaria
    if (mapa) {
        delete mapa;
        mapa = nullptr;
    }
}

void Nivel::reiniciarNivel() {
    // Método vacío por defecto, se puede sobreescribir si se desea
}
