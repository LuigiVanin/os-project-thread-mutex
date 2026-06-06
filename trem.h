#ifndef TREM_H
#define TREM_H

#include <QThread>

// #include "ui_mainwindow.h"
#include "utils.h"
#include "track.h"


/*
 * Classe Trem herda QThread
 * Classe Trem passa a ser uma thread.
 * A função START inicializa a thread. Após inicializada, a thread irá executar a função RUN.
 * Para parar a execução da função RUN da thread, basta executar a função TERMINATE.
 *
*/
class Trem: public QThread{
 Q_OBJECT
public:
    Trem(int, QLabel* trem_entity, std::vector<Track *> tracks);  //construtor
    ~Trem(); // destructor

    void run();         //função a ser executada pela thread

    int getID();    
    QLabel* getEntity();




//Cria um sinal
signals:
    void updateGUI(int,int,int);

public slots:
    void updateVelocity(int);

private:
   int x;           //posição X do trem na tela
   int y;           //posição Y do trem na tela
   int ID;          //ID do trem
   int velocidade;  //Velocidade. É o tempo de dormir em milisegundos entre a mudança de posição do trem
   // diferencial de espaço - 10 pixeis por ciclo
   int dv = 15;

   QLabel *entity;
   std::vector<Track *> tracks;


   Vector2D calculatePathStart(Track* track, Vector2D orientation);
   Vector2D calculatePathEnd(Track* track, Vector2D orientation);
   Vector2D calculateOrientation(Track* track);
};

#endif // TREM_H
