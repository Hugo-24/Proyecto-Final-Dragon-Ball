#ifndef ROSHI_H
#define ROSHI_H

#include "personaje.h"

// Clase concreta: Maestro Roshi jugable
class Roshi : public Personaje {
public:
    Roshi(QWidget* parent);
    void atacar();
    void lanzarEnergia();

private:
    void volverAIdle();

protected:
    void actualizarSprite() override; // Cambia sprite según dirección
};

#endif // ROSHI_H
