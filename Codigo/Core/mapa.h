#pragma once
#include <QWidget>
#include <QPixmap>
#include <QLabel>
#include <vector>


class Mapa : public QWidget {
    Q_OBJECT

public:
    explicit Mapa(const QString& rutaFondo, QWidget* parent = nullptr);
    void cargarDesdeArchivo(const QString& archivo);
    void detectarColisiones();

private:
    QLabel* fondoLabel;
    //std::vector<Objeto*> elementos;
};
