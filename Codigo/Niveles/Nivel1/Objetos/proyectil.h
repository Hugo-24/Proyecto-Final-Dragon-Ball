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
              const QString& tipo, // "roshi", "lunch", "subfusil"
              float velocidad = 10.0f);

    void actualizar() override;
    void interactuar(Entidad* otra) override;

    int getDanio() const; // Devuelve el daño del proyectil
    void setEsDelJugador(bool valor);
    bool esDelJugadorFunc() const;
    QString getTipo() const; // Devuelve el tipo para validación
    ~Proyectil();

private:
    QVector2D direccion;  // Dirección de movimiento (1,0) o (-1,0)
    float velocidad;      // Velocidad del proyectil
    int danio = 10;        // Nuevo: daño que causa este proyectil
    bool esDelJugador = true; // Nuevo: indica si el proyectil fue disparado por el jugador
    QString tipo; // Identificador del tipo de proyectil: "lunch", "roshi", "subfusil", etc.
};

#endif // PROYECTIL_H
