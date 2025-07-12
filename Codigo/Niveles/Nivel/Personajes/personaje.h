#ifndef PERSONAJE_H
#define PERSONAJE_H

#include "entidad.h"  // Hereda comportamiento base de Entidad (posición, velocidad, etc.)
#include <QTimer>     // Usado para controlar la animación de caminar

// Clase base para personajes jugables como Roshi, Lunch, Goku
class Personaje : public Entidad, public QObject {
public:
    // Constructor base. Se inicializa con el widget padre.
    Personaje(QWidget* parent);

    // Movimiento horizontal a la izquierda
    virtual void moverIzquierda();

    // Movimiento horizontal a la derecha
    virtual void moverDerecha();

    // Salto vertical con impulso hacia arriba
    virtual void saltar();

    // Ataque cuerpo a cuerpo (se redefine en subclases como Roshi o Goku)
    virtual void atacar() {}

    // Ataque de energía o disparo (se redefine en subclases como Lunch o Roshi)
    virtual void lanzarEnergia() {}

    // Indica si el personaje mira hacia la derecha
    bool estaMirandoDerecha() const;

    // Métodos para controlar el cooldown de disparo (tiempo entre disparos)
    bool estaListoParaDisparar() const { return puedeDisparar; }
    void setPuedeDisparar(bool estado) { puedeDisparar = estado; }

    // Control de animación de caminar
    void iniciarAnimacionCaminar();
    void detenerAnimacionCaminar();

    // Cambia el estado de "muerto" del personaje
    void setMuerto(bool muerto) {
        estaMuerto = muerto;
    }
    //destructor
    virtual ~Personaje() {}

protected:
    bool mirandoDerecha;         // true = el sprite mira a la derecha
    virtual void actualizarSprite(); // Función virtual para actualizar el sprite (según paso o dirección)

    bool puedeDisparar = true;       // Controla si el personaje puede lanzar un ataque especial

    // Temporizador para animar el movimiento (cambio entre sprite1 y sprite2)
    QTimer* animTimer = nullptr;

    // Alterna entre dos sprites para simular caminar
    bool pasoAlternado = false;

    bool estaMuerto = false;         // Indica si el personaje fue derrotado
};

#endif // PERSONAJE_H
