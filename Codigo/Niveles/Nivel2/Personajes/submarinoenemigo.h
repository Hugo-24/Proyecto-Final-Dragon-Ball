#pragma once
#include "entidad.h"
#include <QVector2D>
#include <QTimer>
#include <QObject>


class SubmarinoEnemigo : public QObject, public Entidad {
    Q_OBJECT

public:
    SubmarinoEnemigo(QWidget* parent, const QVector2D& posicion);
    void actualizar() override;
    void interactuar(Entidad* otra) override;
    void recibirDaño(int cantidad);
    bool estaDestruido() const;

    void verificarFoco(const Entidad* jugador);  // Verifica si el jugador está en rango
    Entidad* objetivo; // Referencia al jugador para seguimiento

signals:
    void torpedoDisparado(const QVector2D& pos, const QVector2D& dir);

private slots:
    void dispararTorpedo();
    void salirDeModoAtaque();

private:
    QVector2D direccion;
    float velocidad;
    int vida;

    // Patrullaje
    float tiempoTotal;
    float amplitudVertical;
    float frecuencia;
    int pasosDesdeUltimoAvance;
    int pasosPorAvance;

    // Modo ataque
    bool enModoAtaque;
    float tiempoEnAtaque;
    QTimer* temporizadorAtaque;
    QTimer* temporizadorDisparo;
};
