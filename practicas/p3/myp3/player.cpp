#include <iostream>
#include <cstdlib>
#include <vector>
#include <queue>
#include <stack>

#include "player.h"
#include "environment.h"

/* --- MODIFICADO POR:  Víctor García Carrera, victorgarcia@correo.ugr.es --- */

using namespace std;

const double masinf=9999999999.0, menosinf=-9999999999.0;
//const int MAX = 1; MIN = 0;


// Constructor
Player::Player(int jug){
	jugador_=jug;
}

// Actualiza el estado del juego para el jugador
void Player::Perceive(const Environment & env){
	actual_=env;
}

double Puntuacion(int jugador, const Environment &estado){
	double suma=0;

	for (int i=0; i<7; i++)
	  for (int j=0; j<7; j++){
		 if (estado.See_Casilla(i,j)==jugador){
			if (j<3)
			   suma += j;
			else
			   suma += (6-j);
		 }
	  }

	return suma;
}


// Funcion de valoracion para testear Poda Alfabeta
double ValoracionTest(const Environment &estado, int jugador){
	int ganador = estado.RevisarTablero();

	if (ganador==jugador)
	   return 99999999.0; // Gana el jugador que pide la valoracion
	else if (ganador!=0)
			return -99999999.0; // Pierde el jugador que pide la valoracion
	else if (estado.Get_Casillas_Libres()==0)
			return 0;  // Hay un empate global y se ha rellenado completamente el tablero
	else
		  return Puntuacion(jugador,estado);
}

// ------------------- Los tres metodos anteriores no se pueden modificar



// MI Funcion heuristica
// Valores elevados indican estados más favorables para ganar, y negativos lo contrario
double Valoracion(const Environment &estado, int jugador){

	// Version PRO incorpora mejoras sustanciales como diferenciar EARLY, MID, LATE(COMBOS)

	// IDEA: GANAR LAS ESQUINAS, GENERARÁ MENOS CONEXIONES QUE EL CENTRO
	//double esquinas = valorarEsquinas(estado);

	// double PELIGRO_LINEA
	double valor = 0;

	// ROLLO EARLY
	// Guardamos la ocupacion de cada columna (valores de 0:vacía a 7:llena)
	for (int j=0; j < 7; j++){
		
		int ocup = estado.Get_Ocupacion_Columna(j);
		if (ocup == 7){
			valor = -1;		// Si está llena, valoración negativa
		}

		// LAS 3 SIGUIENTES FAVORECEN HUECOS VACIOS
		if (ocup == 0){
			valor += 5;
		}
		if (ocup == 1){
			valor += 4;
		}
		if (ocup == 2){
			valor += 3;
		}

		// FAVORECEMOS LAS ESQUINAS
		if (j < 2 or j > 4){
			if (ocup < 3 or ocup > 5){		// ### Ver que justo las de debajo NO sean nuestras ni a los lados
				valor += 5;
			}
		}

	}
	/*
	int casillasLibres = Get_Casillas_Libres();
	// MINIMIZAR NUESTRO NUM FICHAS (BOMBAS)
	casillasLibres*/

	return valor;

}

/*
// ESCALA DE 0 A 4
// Devuelve un valor alto si representa un movimiento a una esquina, bajo en caso contrario
double valorarEsquinas(const Environment &estado, int jugador){
	ocupacion = [];
	double valor = 0;

	// Guardamos la ocupacion de cada columna (valores de 0:vacía a 7:llena)
	for (int j=0; j < 7; j++){
		
		int ocup = Get_Ocupacion_Columna(j);
		ocupacion.append(ocup);

		if (j < 2 or j > 4){
			if (ocup < 3 or ocup > 5){		// ### Ver que justo las de debajo NO sean nuestras ni a los lados
				valor = 5;
			}
		}

	}

	if ()
}*/


// Esta funcion no se puede usar en la version entregable
// Aparece aqui solo para ILUSTRAR el comportamiento del juego
// ESTO NO IMPLEMENTA NI MINIMAX, NI PODA ALFABETA
void JuegoAleatorio(bool aplicables[], int opciones[], int &j){
	j=0;
	for (int i=0; i<8; i++){
		if (aplicables[i]){
		   opciones[j]=i;
		   j++;
		}
	}
}


struct nodo{
  Environment estado;   // Representa el estado del juego asociado a ese nodo
  double evaluacion;   //  Valoración de la heurística del estado que representa el nodo, se actualiza según Minimax
  int jugador;      //  Quien juega desde este estado, MAX=1 o MIN=0
  bool VICTORIA;       //  Indica que la jugada es una victoria para uno de los jugadores
  int prof;         //  Profundidad en el árbol de juego
};


// Función que Genera el árbol de juego y lo evalúa según Minimax de forma recursiva para evaluar el nodo current y devolver la acción elegida
double EvaluaMinimax(nodo &current, int jugador, int MAXPROF, int &accionElegida){     // ##### MEJOR OPCION??

	// accionElegida: Indica el hijo responsable de tal peso (SIGUIENTE JUGADA)
	int MAX = jugador;		// MAX es nuestro jugador
	int MIN = (MAX+1)%2;	// MIN es el otro
	double average;			// Representa el peso de la heurística escogido según Minimax en función de sus hijos para el nodo current
	//int hijoAverage;			// accionElegida: Indica el hijo responsable de tal peso (SIGUIENTE JUGADA)
	//set<estado,ComparaEstados> visitados;	// Cerrados (inicialmente vacio): Conjunto de estados explorados.
	//arbol = [];						// Árbol Minimax de posibles estados desde el actual
	Environment *posiblesEstados;

	cout << "Generando sucesores\n" << endl;

  	nodo next;					// Nodo que representa a un sucesor de current
	/*next.estado = actual;
	next.jugador = MAX;
	next.prof = prof;*/
	int n_act = current.estado.GenerateAllMoves(posiblesEstados); // Obtengo las acciones aplicables al estado actual en "aplicables"*/

  	cout << " NEXT" << endl;
	if (current.prof == MAXPROF or n_act == 0){		// Es un NODO TERMINAL, no hay acciones posibles a desarrollar desde el estado actual
		(current.jugador==MAX) ? cout << " MAX ha alcanzado un nodo terminal " : cout << " MIN ha alcanzado un nodo terminal ";
		//accion = Environment::actIDLE;
		current.evaluacion = Valoracion(current.estado, current.jugador);
		accionElegida = -1;
		return current.evaluacion;
	}
	/*
	else if (n_act==1){
		(current.jugador==1) ? cout << " MAX: " : cout << " MIN: ";
		cout << " Solo se puede realizar la accion "
				 << current.estado.ActionStr( static_cast< Environment::ActionType > (posiblesEstados[0])  ) << endl;
		
		//accion = static_cast< Environment::ActionType > (posiblesEstados[0]);
		average = EvaluaMinimax(posiblesEstados[0])
		accionElegida = 0;
		return average;		// Devolvemos la valoracion de este único hijo (el único estado posible)
	}*/

	else { // Hay varios estados posibles desde el actual

		if (current.jugador == MAX){						// Si nos encontramos en un nodo MAX, los hijos serán MIN
			next.jugador = MIN;
			average = menosinf;

			cout << "Nodo MIN creado de prof " << next.prof << endl;

			for (int i=0; i<n_act; i++){	// Para cada posible sucesor, generamos su nodo en el árbol
				next.estado = posiblesEstados[i];	// Generamos el sucesor
				next.prof = current.prof+1;
				int valorHijo = EvaluaMinimax(next, jugador, MAXPROF, accionElegida);
		
				if (average < valorHijo){	// Al estar en un nodo MAX, su evaluación es el máximo de las evals de los hijos
					average = valorHijo;				// Actualizamos el peso minimax del nodo
					accionElegida = i;					// Guardamos el posibleEstado que representa 
				}
			}

		}
		else{												// Si nos encontramos en un nodo MIN, los hijos serán MAX
			next.jugador = MAX;
			average = masinf;

			cout << "Nodo MAX creado de prof " << next.prof << endl;

			for (int i=0; i<n_act; i++){	// Para cada posible sucesor, generamos su nodo en el árbol
				next.estado = posiblesEstados[i];	// Generamos el sucesor
				next.prof = current.prof+1;
				int valorHijo = EvaluaMinimax(next, jugador, MAXPROF, accionElegida);

				if (average > valorHijo){	// Al estar en un nodo MAX, su evaluación es el máximo de las evals de los hijos
					average = valorHijo;				// Actualizamos el peso minimax del nodo
					accionElegida = i;					// Guardamos el posibleEstado que representa 
				}
			}

		}

		//accion = static_cast< Environment::ActionType > (posiblesEstados[hijoAverage]);		// Guardamos la accion elegida por Minimax
		current.evaluacion = average;														// Guardamos su peso
		return average;

		//current.evaluacion;

	}
	

}


// Implementa el Algoritmo Minimax para jugar la partida
// En Teoría de Juegos, este aloritmo representa un método de decisión de juegos con adversario
//  para MINIMIZAR la PÉRDIDA MÁXIMA esperada por el contrincante 
// Devuelve el valor obtenido tras la búsqueda minimax desde el estado actual
double Player::JuegoMinimax(const Environment & actual, int jugador, int MAXPROF, Environment::ActionType &accion){     // ##### MEJOR OPCION??
	
	// PASOS DEL ALGORITMO MINIMAX
	//  Generar árbol de juego
	//  Evaluar (siempre con la heurística) los nodos terminales del árbol
	//  Evaluar sucesivamente los nodos padres a partir de los terminales (hacia arriba) hasta la raiz
	//  Escoger la jugada de profundidad 1 (directamente realizable) que optimiza la heurística (VALOR MAS ALTO)
	  //     Escoge la siguiente acción siguiendo el mejor camino que puede presuponer

	/*j=0;

	for (int i=0; i<8; i++){	// Para cada una de las 8 posibles jugadas en ese estado
		if (aplicables[i]){
			opciones[j]=i;
			j++;
		}
	}*/

	int prof = 0;
	int accionElegida;
	int n_act;		// Número de acciones posibles a partir de ese estado
	Environment *posiblesEstados;

	//set<estado,ComparaEstados> visitados;	// Cerrados (inicialmente vacio): Conjunto de estados explorados.
	//arbol = [];						// Árbol Minimax de posibles estados desde el actual

	cout << "Generando arbol de juego\n" << endl;

  	nodo current;					// Definimos el Nodo Raiz del arbol de búsqueda con el estado actual de la partida
	current.estado = Environment(actual);
	current.jugador = jugador;
	current.prof = prof;

	cout << "NODO CREADO de prof " << current.prof << " , jugador " << current.jugador << endl;

	// Evaluamos por 
	double evalRaiz = EvaluaMinimax(current, jugador, MAXPROF, accionElegida);

	// Actualizamos la acción elegida por Minimax a realizar
	if (accionElegida >= 0){
		accion = static_cast< Environment::ActionType > (accionElegida);		// Guardamos la accion elegida por Minimax

		cout << " ACCIÓN DECIDIDA CON HEURISTICA " << evalRaiz << " :  " 
				<< current.estado.ActionStr( static_cast< Environment::ActionType > (accionElegida)  ) << endl;

	}
	else{			// No hay acciones posibles a desarrollar desde el estado actual, es un NODO TERMINAL
		(jugador==1) ? cout << "Verde: " : cout << "Azul: ";
		cout << " No puede realizar ninguna accion!!!\n";

	}

	return evalRaiz;
	
}



// Invoca el siguiente movimiento del jugador
Environment::ActionType Player::Think(){
	const int PROFUNDIDAD_MINIMAX = 4;  // Umbral maximo de profundidad para el metodo MiniMax    Quizas 6??
	//const int PROFUNDIDAD_ALFABETA = 8; // Umbral maximo de profundidad para la poda Alfa_Beta

	Environment::ActionType accion; // acción que se va a devolver
	bool aplicables[8]; // Vector bool usado para obtener las acciones que son aplicables en el estado actual. La interpretacion es
						// aplicables[0]==true si PUT1 es aplicable
						// aplicables[1]==true si PUT2 es aplicable
						// aplicables[2]==true si PUT3 es aplicable
						// aplicables[3]==true si PUT4 es aplicable
						// aplicables[4]==true si PUT5 es aplicable
						// aplicables[5]==true si PUT6 es aplicable
						// aplicables[6]==true si PUT7 es aplicable
						// aplicables[7]==true si BOOM es aplicable



	double valor; // Almacena el valor con el que se etiqueta el estado tras el proceso de busqueda.
	//double alpha, beta; // Cotas de la poda AlfaBeta

	int n_act; //Acciones posibles en el estado actual


	n_act = actual_.possible_actions(aplicables); // Obtengo las acciones aplicables al estado actual en "aplicables"
	//int opciones[10];

	// Muestra por la consola las acciones aplicable para el jugador activo
	//actual_.PintaTablero();
	cout << " Acciones aplicables ";
	(jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
	for (int t=0; t<8; t++)
	  if (aplicables[t])
		 cout << " " << actual_.ActionStr( static_cast< Environment::ActionType > (t)  );
	cout << endl;

	/*
	//--------------------- COMENTAR Desde aqui
	cout << "\n\t";
	int n_opciones=0;
	JuegoAleatorio(aplicables, opciones, n_opciones);   // #### LLAMO ALGORITMO Y CONSTRUYO ARBOL

	if (n_act==0){		// COMPROBAR RAPIDO ANTES DE ALGORITMO MINIMAX
	  (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
	  cout << " No puede realizar ninguna accion!!!\n";
	  //accion = Environment::actIDLE;
	}
	else if (n_act==1){
		   (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
			cout << " Solo se puede realizar la accion "
				 << actual_.ActionStr( static_cast< Environment::ActionType > (opciones[0])  ) << endl;
			accion = static_cast< Environment::ActionType > (opciones[0]);

		 }
		 else { // Hay que elegir entre varias posibles acciones      #### METERIA ELEGIR LA OPCION CON MAYOR VALOR
			int aleatorio = rand()%n_opciones;
			cout << " -> " << actual_.ActionStr( static_cast< Environment::ActionType > (opciones[aleatorio])  ) << endl;
			accion = static_cast< Environment::ActionType > (opciones[aleatorio]);
		 }

	//--------------------- COMENTAR Hasta aqui */


	//--------------------- AQUI EMPIEZA LA PARTE A REALIZAR POR EL ALUMNO ------------------------------------------------


	// Opcion: Poda AlfaBeta
	// NOTA: La parametrizacion es solo orientativa
	valor = JuegoMinimax(actual_, jugador_, PROFUNDIDAD_MINIMAX, accion);
	cout << "Valor MiniMax: " << valor << "  Accion: " << actual_.ActionStr(accion) << endl;

	return accion;
}

