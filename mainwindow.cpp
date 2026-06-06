#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utils.h"

#define TREM_COUNT 6

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QSlider* sliders[TREM_COUNT] = {
        ui->t1_speed,
        ui->t2_speed,
        ui->t3_speed,
        ui->t4_speed,
        ui->t5_speed,
        ui->t6_speed

    };

    Track* track_list[19] = {
        new Track(1, ui->label_trilho1),
        new Track(2, ui->label_trilho2, true), // flag critical zone tracks with the last parameter to check for colisions
        new Track(3, ui->label_trilho3, true),
        new Track(4, ui->label_trilho4), // Yeah, I forgor the fifth track and I will not refactor anything anymore
        new Track(6, ui->label_trilho6),
        new Track(7, ui->label_trilho7, true),
        new Track(8, ui->label_trilho8, true),
        new Track(9, ui->label_trilho9),
        new Track(10, ui->label_trilho10),
        new Track(11, ui->label_trilho11, true),
        new Track(12, ui->label_trilho12, true),
        new Track(13, ui->label_trilho13, true),
        new Track(14, ui->label_trilho14),
        new Track(15, ui->label_trilho15),
        new Track(16, ui->label_trilho16, true),
        new Track(17, ui->label_trilho17, true),
        new Track(18, ui->label_trilho18),
        new Track(19, ui->label_trilho19),
        new Track(20, ui->label_trilho20),
    }; // NOTE: Marcadas 9 trilhos(tracks) como regiões críticas - elas terão seus próprios mutex para marcar
       //       qual trem está atualmente usando o trilhos

    // Busca um trilho pelo seu ID na track_list - lambda function fuck it
    auto trackByID = [&track_list](int id) -> Track* {
        for (Track* track : track_list) {
            if (track->getID() == id) return track;
        }
        return nullptr;
    };

    std::vector<Track*> tracks[TREM_COUNT] = {
        {
            trackByID(1),
            trackByID(3),
            trackByID(2),
            trackByID(4)
        },
        {
            trackByID(6),
            trackByID(12),
            trackByID(13),
            trackByID(7),
            trackByID(8),
            trackByID(3)
        },
        {
            trackByID(9),
            trackByID(10),
            trackByID(11),
            trackByID(12)
        },
        {
            trackByID(15),
            trackByID(16),
            trackByID(13),
            trackByID(11),
            trackByID(14)
        },
        {
            trackByID(20),
            trackByID(17),
            trackByID(7),
            trackByID(16)
        },
        {
            trackByID(18),
            trackByID(17),
            trackByID(8),
            trackByID(2),
            trackByID(19)
        }
    };

    // Cria o trem com seu (ID, posição X, posição Y) - nah, vamos pegar a posição a partir da track inicial
    QLabel* trems_entities[TREM_COUNT] = {
        ui->label_trem1,
        ui->label_trem2,
        ui->label_trem3,
        ui->label_trem4,
        ui->label_trem5,
        ui->label_trem6,
    };

    for (size_t i = 0; i < TREM_COUNT; i++) {
        this->trems.push_back(
            new Trem(
                    i + 1,
                    trems_entities[i],
                    tracks[i]
                )
            );

    }
/*
     * Conecta o sinal UPDATEGUI à função UPDATEINTERFACE.
     * Ou seja, sempre que o sinal UPDATEGUI foi chamado, será executada a função UPDATEINTERFACE.
     * Os 3 parâmetros INT do sinal serão utilizados na função.
     * Trem1 e Trem2 são os objetos que podem chamar o sinal. Se um outro objeto chamar o
     * sinal UPDATEGUI, não haverá execução da função UPDATEINTERFACE
     */

    for (size_t i = 0; i < TREM_COUNT; i++) {
        auto trem = trems[i];
        auto slider = sliders[i];
        connect(trem, SIGNAL(updateGUI(int, int, int)), SLOT(updateInterface(int, int, int)));
        connect(slider, &QSlider::valueChanged, trem, &Trem::updateVelocity);
    }

    for (Trem* trem : trems) {
        trem->start();
    }
}

//Função que será executada quando o sinal UPDATEGUI for emitido
void MainWindow::updateInterface(int id, int x, int y){

    for (Trem* trem : this->trems) {
        if (id == trem->getID()) {
            trem->getEntity()->setGeometry(x, y, TREM_SIZE.x, TREM_SIZE.y);
        }
    }
}

MainWindow::~MainWindow()
{

    for (Trem* trem : trems) {
        trem->terminate();
        delete trem;
    }

    delete ui;
}
