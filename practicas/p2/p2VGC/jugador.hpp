#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"

#include <list>

/* --- MODIFICADO POR: 	Víctor García Carrera, victorgarcia@correo.ugr.es --- */


struct estado {
  int fila;
  int columna;
  int orientacion;
};

class ComportamientoJugador : public Comportamiento {
  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size) {
      // Inicializar Variables de Estado
      fil = col = 99;
      brujula = 0; // 0: Norte, 1:Este, 2:Sur, 3:Oeste
      destino.fila = -1;
      destino.columna = -1;
      destino.orientacion = -1;
      ultimaAccion = actIDLE;
      hayPlan = false;
      bienSituado = false;
      primero = false;
      reto = false;
    }
    ComportamientoJugador(std::vector< std::vector< unsigned char> > mapaR) : Comportamiento(mapaR) {
      // Inicializar Variables de Estado
      fil = col = 99;
      brujula = 0; // 0: Norte, 1:Este, 2:Sur, 3:Oeste
      destino.fila = -1;
      destino.columna = -1;
      destino.orientacion = -1;
      ultimaAccion = actIDLE;
      hayPlan = false;
      bienSituado = false;
      primero = false;
      reto = false;
    }
    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    void actualizaPosicion(Sensores sensores);
    void actualizaOrientacion(Sensores sensores);
    void actualizaMapa(Sensores sensores);

    void planPK(Sensores sensores, int posPK, list<Action> &plan);
    int costeAvanzar(int fila, int columna);			// Coste de avanzar por esa casilla
    int interact(Action accion, int valor);
    void VisualizaPlan(const estado &st, const list<Action> &plan);
    ComportamientoJugador * clone(){return new ComportamientoJugador(*this);}

  private:
    // Declarar Variables de Estado
    int fil, col, brujula;  //fil y col donde esta el jugador, brujula orientacion 0:Norte, 1:Este, 2:Sur, 3:Oeste
    estado actual, destino;
    list<Action> plan;			// lista de acciones que describen el plan desarrollado por el agente para llegar al destino

    // Nuevas Variables de Estado
    Action ultimaAccion;  // Almacena la ultima accion realizada por el agente para actualizar sus efectos en la siguiente iteracion
    bool hayPlan;				// true si el agente ha deliberado un plan viable, false si no
    bool bienSituado;		// true cuando el agente ha encontrado un PK (punto referencia) y puede situarse correctamente en el mapa real
    bool primero;
    bool reto;
		list<Action> miniPlan;		// Plan para llegar a un PK y situarse


    // Métodos privados de la clase
    bool pathFinding(int level, const estado &origen, const estado &destino, list<Action> &plan);
    bool pathFinding_Profundidad(const estado &origen, const estado &destino, list<Action> &plan);
    bool pathFinding_Anchura(const estado &origen, const estado &destino, list<Action> &plan);
    bool pathFinding_Uniforme(const estado &origen, const estado &destino, list<Action> &plan);
   // bool pathFinding_Reto(const estado &origen, const estado &destino, list<Action> &plan);


    void PintaPlan(list<Action> plan);
    bool HayObstaculoDelante(estado &st);

};

#endif
