#ifndef PROYECTIL_H
#define PROYECTIL_H

#include "objeto.h"

/**
 * Clase que representa un proyectil lanzado por un personaje (Lunch o Roshi).
 * Hereda de Objeto porque puede colisionar con otras entidades.
 */
class Proyectil : public Objeto {
public:
    Proyectil(QWidget* parent,
              const QVector2D& posicion,
              const QVector2D& direccion,
              const QString& tipo, // "roshi" o "lunch"
              float velocidad = 10.0f);

    void actualizar() override;
    void interactuar(Entidad* otra) override;

private:
    QVector2D direccion;  // Dirección de movimiento (1,0) o (-1,0)
    float velocidad;      // Velocidad del proyectil
};

#endif // PROYECTIL_H
