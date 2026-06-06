#include "trem.h"
#include <QtCore>
#include <iostream>
#include "utils.h"

//Construtor
Trem::Trem(int ID, QLabel *trem_entity,std::vector<Track *> tracks){
    this->ID = ID;
    this->tracks = tracks;
    this->entity = trem_entity;

    // Começa de onde o trem foi desenhado na interface
    this->x = trem_entity->x();
    this->y = trem_entity->y();

    QLabel* track_ui = tracks[0]->getUi();
    auto axis = defineTrackAxis(track_ui);

    bool respects_bounds;
    if (axis == Axis::HORIZONTAL) {
        respects_bounds = this->x >= track_ui->x() && this->x + TREM_SIZE.x <= track_ui->x() + track_ui->width();
        this->y = track_ui->y(); // alinha o eixo transversal ao trilho
    } else {
        respects_bounds = this->y >= track_ui->y() && this->y + TREM_SIZE.y <= track_ui->y() + track_ui->height();
        this->x = track_ui->x(); // alinha o eixo transversal ao trilho
    }

    // Não respeita os limites: inicializa no início do trilho atual
    if (!respects_bounds) {
        this->x = track_ui->x();
        this->y = track_ui->y();
    }

    this->updateVelocity(50);
}

Trem::~Trem() {
}

int Trem::getID() {
    return this->ID;
}

QLabel* Trem::getEntity() {
    return this->entity;
}

void Trem::updateVelocity(int value) {
    // this->velocidade = value;

    // O Slide vai de 1 até 99, precisamos transformar seus inputs para milissegundos
    // Transformação para que a velocidade vá de 1ms até 400ms, sendo 99 = 1ms e 1 = 400ms
    if (value >= 99) {
        this->velocidade = 1;
    }

    this->velocidade = (100 - value) * 4;
}


// NOTE: Isso é diferente de calcular aonde o trilho começa, isso indica o caminho que o trem atual
// vai terminar
Vector2D Trem::calculatePathEnd(Track* track, Vector2D orientation) {
    QLabel* track_ui = track->getUi();
    Vector2D track_end = {
        .x = track_ui->x() + track_ui->width(),
        .y = track_ui->y() + track_ui->height()
    };
    Vector2D track_start = {
        .x = track_ui->x(),
        .y = track_ui->y()
    };

    Vector2D path = {
        .x = 0,
        .y = 0,
    };

    if (orientation.x != 0) { // Orientação horizontal == Axis::HORIZONTAL
        if (orientation.x < 0) { // Movendo-se para a esquerda
            path.x = track_start.x; // O caminho do trem vai terminar no inicio do trilho(x = x)
        } else { // O caminho do trem é para a direita
            path.x = track_end.x; // O caminho do trem começa no fim do trilho (x = x + width)
        }
    } else if (orientation.y != 0) { // Orientação vertical == Axis::VERTICAL
        if (orientation.y < 0) { // Movendo-se para baixo
            path.y = track_start.y; // O caminho do trem vai começar no inicio do trilho(y = y)
        } else { // Movendo-se para cima
            path.y = track_end.y; // O caminho do trem começa no final do trilho (y = y + height)
        }
    }


    return path;
}

// NOTE: Isso é diferente de calcular aonde o trilho termina, isso indica o caminho que o trem atual
// vai começar
Vector2D Trem::calculatePathStart(Track* track, Vector2D orientation) {
    QLabel* track_ui = track->getUi();
    Vector2D track_end = {
        .x = track_ui->x() + track_ui->width(),
        .y = track_ui->y() + track_ui->height()
    };
    Vector2D track_start = {
        .x = track_ui->x(),
        .y = track_ui->y()
    };

    Vector2D path = {
        .x = 0,
        .y = 0,
    };

    if (orientation.x != 0) { // Orientação horizontal == Axis::HORIZONTAL
        if (orientation.x < 0) { // Movendo-se para a esquerda
            path.x = track_end.x; // O caminho do trem vai começar do final(x = x + width) do trilho
        } else { // O caminho do trem é para a direita
            path.x = track_start.x; // O caminho do trem começa no inicio(x = x) do trilho
        }
    } else if (orientation.y != 0) { // Orientação vertical == Axis::VERTICAL
        if (orientation.y < 0) { // Movendo-se para baixo
            path.y = track_end.y; // O caminho do trem vai começar do final(y = y + height) do trilho
        } else { // Movendo-se para cima
            path.y = track_start.y; // O caminho do trem começa no inicio(y = y) do trilho
        }
    }


    return path;
}


Vector2D Trem::calculateOrientation(Track* track) {
    QLabel* track_ui = track->getUi();
    Vector2D track_pos = {
        .x = track_ui->x(),
        .y = track_ui->y(),

    };

    Vector2D track_size = {
        .x = track_ui->width(),
        .y = track_ui->height(),
    };

    auto axis = defineTrackAxis(track_ui);

    Vector2D orientation = {
        .x = 0,
        .y = 0,
    };

    if (axis == Axis::HORIZONTAL) {
        auto dist_end = abs( ( track_pos.x + track_size.x ) - this->x );
        auto dist_start = abs( track_pos.x - this->x );

        orientation.x = dist_end < dist_start ? -1 : 1;
    } else if (axis == Axis::VERTICAL) {
        auto dist_end = abs( (track_pos.y + track_size.y) - this->y );
        auto dist_start = abs( track_pos.y - this->y );

        orientation.y = dist_end < dist_start ? -1 : 1;
    }

    return orientation;
}

//Função a ser executada após executar trem->START
void Trem::run(){
    uint track_index = 0;

    const int trem_h = TREM_SIZE.y;
    const int trem_w = TREM_SIZE.x;

    auto current_track = this->tracks[track_index];
    auto orientation = calculateOrientation(current_track);

    // Indica se o trem já travou o mutex do próximo trilho (segurando dois mutexes: atual + próximo)
    bool has_next_reservation = false;

    // Em trilhos não-críticos tryOccupy sempre sucede de primeira (só registra o ocupante)
    while(!current_track->tryOccupy(this->getID())) {
        msleep(velocidade);
    }

    while(true){
        uint next_track_index = (track_index + 1) % this->tracks.size();
        auto next_track = this->tracks[next_track_index];

        if (this->velocidade >= 400) {

            emit updateGUI(ID, x,y);    //Emite um sinal
            msleep(velocidade);
            continue;
        }

        auto path_start = calculatePathStart(current_track, orientation);
        auto path_end = calculatePathEnd(current_track, orientation);
        auto current_track_axis = defineTrackAxis(current_track->getUi());

        // Sem reserva do próximo trilho, o limite é o ponto de espera (recuado em uma
        // trem-length do fim do trilho); com reserva, o limite é o fim real do trilho
        Vector2D limit = path_end;
        if (!has_next_reservation) {
            limit.x -= orientation.x * trem_w;
            limit.y -= orientation.y * trem_h;
        }

        this->x += orientation.x * this->dv;
        this->y += orientation.y * this->dv;

        bool is_horizontal_inbound = false;
        bool is_vertical_inbound = false;

        // Checagem de limites horizontais - prestando muito atenção para a direção para adicionar a largura do trem
        if (orientation.x > 0) {
            is_horizontal_inbound = this->x > path_start.x && this->x + trem_w < limit.x;
        } else if(orientation.x < 0) {
            is_horizontal_inbound = this->x < path_start.x && this->x > limit.x;
        }

        // Checagem de limites verticais - prestando muito atenção para a direção para adicionar a altura do trem
        if (orientation.y > 0) {
            is_vertical_inbound = this->y > path_start.y && this->y + trem_h < limit.y;
        } else if(orientation.y < 0) {
            is_vertical_inbound = this->y < path_start.y && this->y > limit.y;
        }

        if (
            (current_track_axis == Axis::HORIZONTAL && !is_horizontal_inbound) ||
            (current_track_axis == Axis::VERTICAL && !is_vertical_inbound)
           ) {
               
            // Só atualiza a coordenada relevante ao eixo do trilho atual (snap no limite)
            if (current_track_axis == Axis::HORIZONTAL) {
                this->x = limit.x - (trem_w * (orientation.x > 0));
            } else {
                this->y = limit.y - (trem_h * (orientation.y > 0));
            }


            if (!has_next_reservation) {
                // Atingiu o PONTO DE ESPERA: tenta reservar o próximo trilho.
                // Em caso de sucesso o trem passa a segurar DOIS mutexes (atual + próximo)
                // até atingir o fim real do trilho atual
                if (next_track->tryOccupy(this->getID())) {
                    has_next_reservation = true;
                } else {
                    // Diagnóstico: ocupante -1 = mutex órfão; IDs em ciclo = espera circular (deadlock)
                    std::cout << "Trem " << ID << " bloqueado no trilho " << next_track->getID()
                              << " | ocupante atual: " << next_track->getCurrentOccupant() << std::endl;
                }
            } else {
                // Atingiu o FIM REAL do trilho com o próximo já reservado: migra
                current_track->release(this->getID());
                track_index = next_track_index;
                current_track = this->tracks[track_index];
                orientation = calculateOrientation(current_track);
                has_next_reservation = false;
            }


        }
   

        emit updateGUI(ID, x,y);    //Emite um sinal
        msleep(velocidade);
    }
}




