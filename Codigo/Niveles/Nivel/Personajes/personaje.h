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

    // Ataque cuerpo a cuerpo (opcional en subclases)
    virtual void atacar() {}

    // Ataque de energía (opcional en subclases)
    virtual void lanzarEnergia() {}

    bool estaMirandoDerecha() const;

    // Control de cooldown de disparo
    bool estaListoParaDisparar() const { return puedeDisparar; }
    void setPuedeDisparar(bool estado) { puedeDisparar = estado; }

protected:
    bool mirandoDerecha;         // true = derecha, false = izquierda
    virtual void actualizarSprite(); // Se redefine en subclases

    //Estado que controla si el personaje puede disparar o está en cooldown
    bool puedeDisparar = true;
};

#endif // PERSONAJE_H
