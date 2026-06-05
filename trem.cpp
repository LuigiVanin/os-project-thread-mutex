#include "trem.h"
#include <QtCore>
#include <iostream>
#include <algorithm>
#include "utils.h"

//Construtor
Trem::Trem(int ID, QLabel *trem_entity,std::vector<QLabel *> tracks){
    this->ID = ID;
    this->tracks = tracks;
    this->entity = trem_entity;

    this->x = tracks[0]->x();
    this->y = tracks[0]->y();
    // this->x = trem_entity->x();
    // this->y = trem_entity->x();

    velocidade = 100;
}

int Trem::getID() {
    return this->ID;
}

QLabel* Trem::getEntity() {
    return this->entity;
}

// NOTE: Isso é diferente de calcular aonde o trilho começa, isso indica o caminho que o trem atual
// vai terminar
Vector2D Trem::calculatePathEnd(QLabel* track, Vector2D orientation) {
    // auto axis = defineTrackAxis(track);
    Vector2D track_end = {
        .x = track->x() + track->width(),
        .y = track->y() + track->height()
    };
    Vector2D track_start = {
        .x = track->x(),
        .y = track->y()
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
Vector2D Trem::calculatePathStart(QLabel* track, Vector2D orientation) {
    // auto axis = defineTrackAxis(track);
    Vector2D track_end = {
        .x = track->x() + track->width(),
        .y = track->y() + track->height()
    };
    Vector2D track_start = {
        .x = track->x(),
        .y = track->y()
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


Vector2D Trem::calculateOrientation(QLabel* track) {
    Vector2D track_pos = {
        .x = track->x(),
        .y = track->y(),

    };

    Vector2D track_size = {
        .x = track->width(),
        .y = track->height(),
    };

    auto axis = defineTrackAxis(track);

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
    
    while(true){
        auto path_start = calculatePathStart(current_track, orientation);
        auto path_end = calculatePathEnd(current_track, orientation);
        auto current_track_axis = defineTrackAxis(current_track);

        this->x += orientation.x * this->dv;
        this->y += orientation.y * this->dv;

        bool is_horizontal_inbound = false;
        bool is_vertical_inbound = false;

        // Checagem de limites horizontais - prestando muito atenção para a direção para adicionar a largura do trem
        if (orientation.x > 0) {
            is_horizontal_inbound = this->x > path_start.x && this->x + trem_w < path_end.x;
        } else if(orientation.x < 0) {
            is_horizontal_inbound = this->x < path_start.x && this->x > path_end.x;
        }

        // Checagem de limites verticais - prestando muito atenção para a direção para adicionar a altura do trem
        if (orientation.y > 0) {
            is_vertical_inbound = this->y > path_start.y && this->y + trem_w < path_end.y;
        } else if(orientation.y < 0) {
            is_vertical_inbound = this->y < path_start.y && this->y > path_end.y;
        }

        if (
            (current_track_axis == Axis::HORIZONTAL && !is_horizontal_inbound) ||
            (current_track_axis == Axis::VERTICAL && !is_vertical_inbound)
           ) {
               
            // Só atualiza a coordenada relevante ao eixo do novo trilho
            if (current_track_axis == Axis::HORIZONTAL) {
                this->x = path_end.x - (trem_w * (orientation.x > 0));
            } else {
                this->y = path_end.y - (trem_h * (orientation.y > 0));
            }

            track_index = (track_index + 1) % this->tracks.size();
            current_track = this->tracks[track_index];
            orientation = calculateOrientation(current_track);


        }
   

        emit updateGUI(ID, x,y);    //Emite um sinal
        msleep(velocidade);
    }
}




