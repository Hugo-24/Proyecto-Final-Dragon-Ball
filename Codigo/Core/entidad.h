#ifndef ENTIDAD_H
#define ENTIDAD_H

#include <QVector2D>
#include <QLabel>
#include <QWidget>

// Clase base para cualquier objeto con posición, velocidad y sprite
class Entidad {
public:
    Entidad(QWidget* parent = nullptr);
    virtual ~Entidad();

    virtual void mover();         // Lógica de desplazamiento
    virtual void actualizar();    // Actualiza sprite según posición
    virtual void aplicarFisica(); // Física simple (gravedad)
    bool enElSuelo; // ¿Está tocando el suelo?

    void setPosicion(const QVector2D& pos);
    QVector2D getPosicion() const;

    void setVelocidad(const QVector2D& vel);
    QVector2D getVelocidad() const;

    QLabel* getSprite();              // Acceso al sprite

    void setSprite(const QString& ruta); // Cambiar imagen del sprite

protected:
    QVector2D posicion;
    QVector2D velocidad;
    QLabel* sprite;
};

#endif // ENTIDAD_H
