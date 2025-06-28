#include "objeto.h"

Objeto::Objeto(QWidget* parent, const QVector2D& posicion)
    : Entidad(parent) {
    setPosicion(posicion);
}

Objeto::~Objeto() {
    // Nada especial que liberar: Entidad ya gestiona sprite
}

void Objeto::actualizar() {
    // Implementación vacía por defecto
}
