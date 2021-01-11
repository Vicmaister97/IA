#ifndef PLAYER_H
#define PLAYER_H

#include "environment.h"

/*
struct nodo{
  const Environment &estado;   // Representa el estado del juego asociado a ese nodo
  double evaluacion;   //  Valoración de la heurística del estado que representa el nodo, se actualiza según Minimax
  int jugador;      //  Quien juega desde este estado, MAX=1 o MIN=0
  //bool VICTORIA;       //  Indica que la jugada es una victoria para uno de los jugadores
  int prof;         //  Profundidad en el árbol de juego
};*/

class Player{
    public:
      Player(int jug);
      Environment::ActionType Think();
      void Perceive(const Environment &env);

    private:
      int jugador_;		//	1 Verde		2 Azul
      Environment actual_;

      // Métodos privados
      double JuegoMinimax(const Environment &current, int jugador, int MAXPROF, Environment::ActionType &accion);
      //double EvaluaMinimax(nodo &current, int jugador, int MAXPROF, int &accionElegida);

};
#endif
