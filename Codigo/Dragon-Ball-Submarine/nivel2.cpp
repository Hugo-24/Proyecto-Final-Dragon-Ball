#include "Nivel2.h"
#include <QLabel>

Nivel2::Nivel2(QWidget* parent) : Nivel(parent) {
    QLabel *label = new QLabel("Nivel 2 en construcción", this);
    label->setAlignment(Qt::AlignCenter);
}

void Nivel2::cargarNivel() {
    // lógica de nivel 2
}
