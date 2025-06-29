#pragma once
#include "objeto.h"
#include <QVector2D>
#include <QWidget>

class Mina : public Objeto {
public:
    Mina(QWidget* parent, const QVector2D& posicion);

    void interactuar(Entidad* entidad) override;
    void actualizar() override;

private:
    QWidget* contenedor; // Referencia al widget contenedor (el mapa o nivel)
};
