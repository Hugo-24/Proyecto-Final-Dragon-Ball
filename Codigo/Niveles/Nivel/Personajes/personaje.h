#ifndef PERSONAJE_H
#define PERSONAJE_H

#include "entidad.h"

// Clase base para personajes jugables como Roshi, Lunch, Goku
class Personaje : public Entidad, public QObject {
public:
    Personaje(QWidget* parent);

    // Movimiento horizontal
    virtual void moverIzquierda();
    virtual void moverDerecha();

    // Salto vertical
    virtual void saltar();

protected:
    bool mirandoDerecha;  // true = derecha, false = izquierda
    virtual void actualizarSprite(); // Se redefine en subclases
};

#endif // PERSONAJE_H
