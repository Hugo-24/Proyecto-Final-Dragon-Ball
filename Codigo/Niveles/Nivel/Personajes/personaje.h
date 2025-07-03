#ifndef PERSONAJE_H
#define PERSONAJE_H

#include "entidad.h"
#include <QTimer>

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

    // Control de animación de caminar
    void iniciarAnimacionCaminar();
    void detenerAnimacionCaminar();

protected:
    bool mirandoDerecha;         // true = derecha, false = izquierda
    virtual void actualizarSprite(); // Se redefine en subclases

    bool puedeDisparar = true;

    // Animación de caminar
    QTimer* animTimer = nullptr;
    bool pasoAlternado = false;  // Para alternar entre Walk1 y Walk2
};

#endif // PERSONAJE_H
