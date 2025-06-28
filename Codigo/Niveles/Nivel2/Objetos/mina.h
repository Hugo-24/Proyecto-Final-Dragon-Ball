#pragma once
#include "objeto.h"

class Mina : public Objeto {
public:
    Mina(QWidget* parent, const QVector2D& posicion);
    void interactuar(Entidad* entidad) override;
    void actualizar() override;
};
