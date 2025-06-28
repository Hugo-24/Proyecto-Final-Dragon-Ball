#include "mapa.h"
#include <QVBoxLayout>
#include <QDebug>
#include <QPixmap>
#include <QLabel>

Mapa::Mapa(const QString& rutaFondo, QWidget* parent) : QWidget(parent) {
    QVBoxLayout* layout = new QVBoxLayout(this);
    fondoLabel = new QLabel(this);

    QPixmap fondo(rutaFondo);
    if (fondo.isNull()) {
        qDebug() << "No se pudo cargar el fondo desde:" << rutaFondo;
    }

    fondoLabel->setPixmap(fondo.scaled(800, 600));
    fondoLabel->setFixedSize(800, 600);
    fondoLabel->setScaledContents(true);

    layout->addWidget(fondoLabel);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);
}
void Mapa::cargarDesdeArchivo(const QString& archivo) {
    // Aquí cargarás los objetos del mapa a futuro
}

void Mapa::detectarColisiones() {
    // Aquí verificarás colisiones entre jugador y objetos
}
