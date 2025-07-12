#ifndef ROSHI_H
#define ROSHI_H

#include "personaje.h"

/**
 * Clase Roshi: personaje jugable.
 * Puede atacar cuerpo a cuerpo y lanzar energía.
 */
class Roshi : public Personaje {
public:
    Roshi(QWidget* parent);
    ~Roshi();

    void atacar() override;          // Ataque cuerpo a cuerpo
    void lanzarEnergia() override;  // Disparo de energía

protected:
    void actualizarSprite() override; // Cambia sprite según dirección y movimiento
};

#endif // ROSHI_H
