#ifndef PLAYER_H
#define PLAYER_H

#include "environment.h"

/* --- MODIFICADO POR:  Víctor García Carrera, victorgarcia@correo.ugr.es --- */

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
