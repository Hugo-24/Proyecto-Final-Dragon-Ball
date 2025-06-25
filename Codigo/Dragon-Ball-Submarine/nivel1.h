#pragma once
#include "Nivel.h"

class Nivel1 : public Nivel {
    Q_OBJECT
public:
    explicit Nivel1(QWidget* parent = nullptr);
    void cargarNivel() override;
};
