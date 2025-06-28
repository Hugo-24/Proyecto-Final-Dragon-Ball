#include "nivel1.h"
#include <QLabel>

Nivel1::Nivel1(QWidget* parent) : Nivel(parent) {
    QLabel *label = new QLabel("Nivel 1 en construcción", this);
    label->setAlignment(Qt::AlignCenter);
}

void Nivel1::cargarNivel() {
    // lógica de nivel 1
}
