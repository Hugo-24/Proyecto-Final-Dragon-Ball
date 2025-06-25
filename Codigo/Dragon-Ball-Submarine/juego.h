#pragma once
#include <QWidget>
#include <map>
#include "Nivel.h"
#include "Nivel1.h"
#include "Nivel2.h"

class Juego : public QWidget {
    Q_OBJECT

public:
    Juego(QWidget *parent = nullptr);
    void cambiarNivel(const std::string& id);

private:
    std::map<std::string, Nivel*> niveles;
    Nivel* nivelActual;
};
