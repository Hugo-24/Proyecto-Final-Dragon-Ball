#ifndef NIVEL1_H
#define NIVEL1_H

#include "nivel.h"
#include "roshi.h"
#include <QTimer>
#include <QSet>

class Nivel1 : public Nivel {
    Q_OBJECT

public:
    Nivel1(QWidget* parent = nullptr);
    void cargarNivel() override;

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

private:
    Personaje* jugador;
    QTimer* timer;
    QSet<int> teclasPresionadas;
};

#endif // NIVEL1_H
