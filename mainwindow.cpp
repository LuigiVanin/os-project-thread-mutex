#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utils.h"

#define TREM_COUNT 6

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    std::vector<QLabel*> tracks[TREM_COUNT] = {
        {
            ui->label_trilho1,
            ui->label_trilho3,
            ui->label_trilho2,
            ui->label_trilho4
        },
        {
            ui->label_trilho6,
            ui->label_trilho12,
            ui->label_trilho13,
            ui->label_trilho7,
            ui->label_trilho8,
            ui->label_trilho3
        },
        {
            ui->label_trilho9,
            ui->label_trilho10,
            ui->label_trilho11,
            ui->label_trilho12
        },
        {
            ui->label_trilho15,
            ui->label_trilho16,
            ui->label_trilho13,
            ui->label_trilho11,
            ui->label_trilho14
        },
        {
            ui->label_trilho20,
            ui->label_trilho17,
            ui->label_trilho7,
            ui->label_trilho16
        },
        {
            ui->label_trilho18,
            ui->label_trilho17,
            ui->label_trilho8,
            ui->label_trilho2,
            ui->label_trilho19
        }
    };

    //Cria o trem com seu (ID, posição X, posição Y)
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

for (Trem* trem : trems) {
    connect(trem ,SIGNAL(updateGUI(int, int, int)), SLOT(updateInterface(int, int, int)));
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
    delete ui;
}

/*
 * Ao clicar, trens começam execução
 */
void MainWindow::on_pushButton_clicked()
{
    for (Trem* trem : trems) {
        trem->start();
    }
}

/*
 * Ao clicar, trens param execução
 */
void MainWindow::on_pushButton_2_clicked()
{
    for (Trem* trem : trems) {
        trem->terminate();
    }
}
