#pragma once
#include "objeto.h"
#include <QVector2D>

class Torpedo : public Objeto {
public:
    Torpedo(QWidget* parent, const QVector2D& posicion, const QVector2D& direccion);

    void actualizar() override;
    void interactuar(Entidad* entidad) override;

private:
    QVector2D direccion;
    float velocidad;

    float tiempoTotal;           // para simular la progresión
    float amplitud;              // qué tan alto es el "oleaje"
    float frecuencia;            // qué tan rápido oscila
    float posicionInicialY;      // punto de partida en Y

};
