#pragma once
#include "entidad.h"

// Clase base abstracta para todos los objetos interactuables del juego
class Objeto : public Entidad {
public:
    Objeto(QWidget* parent = nullptr, const QVector2D& posicion = QVector2D());
    virtual ~Objeto();

    virtual void interactuar(Entidad* otra) = 0; // Acción al colisionar con una entidad
    virtual void actualizar(); // Por defecto no hace nada (puede sobrescribirse)
};
