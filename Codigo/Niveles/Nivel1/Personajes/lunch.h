#ifndef LUNCH_H
#define LUNCH_H

#include "personaje.h"

// Clase Launch: personaje con transformación y disparo
class Lunch : public Personaje {
public:
    Lunch(QWidget* parent);

    void atacar();       // Golpe cuerpo a cuerpo (como Roshi)
    void disparar();     // Disparo con animación de bazuca (si está transformada)
    void transformar();  // Alterna entre modo tranquila y agresiva

protected:
    void actualizarSprite() override;

private:
    bool transformada; // true si es Launch agresiva (puede atacar y disparar)
};

#endif // LUNCH_H
