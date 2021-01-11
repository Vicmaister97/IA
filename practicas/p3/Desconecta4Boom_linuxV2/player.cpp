#include <iostream>
#include <cstdlib>
#include <vector>
#include <queue>
#include <stack>

#include "player.h"
#include "environment.h"

/* --- MODIFICADO POR:  V�ctor Garc�a Carrera, victorgarcia@correo.ugr.es --- */

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

// Dada una posici�n(fil,col), y un jugador, devuelve el n�mero de 2 en l�nea que tiene ese jugador
int EnLinea2(const Environment &estado, int fila, int columna, int jugador){
	int ret = 0;
	int ganador=0;

	if (estado.See_Casilla(fila,columna)==0){
		return ret;
	}

	ganador= (estado.See_Casilla(fila,columna)%3);
	if (ganador == jugador){

	
		//En la misma Fila
		if (columna<6){
			if ( (estado.See_Casilla(fila,columna)%3)   == (estado.See_Casilla(fila,columna+1)%3)){
				//cout << "En la misma fila\n";
				ret+=1;
			
			}
		}
		//En la misma columna
		if (fila<6){
			if ((estado.See_Casilla(fila,columna)%3)   == (estado.See_Casilla(fila+1,columna)%3) ){
				//cout << "En la misma columna\n";
				ret+=1;	
				
			}
		}
		//En la diagonal hacia arriba
		if (fila<6 and columna<6){
			if ((estado.See_Casilla(fila,columna)%3)     == (estado.See_Casilla(fila+1,columna+1)%3)){
			  	//cout << "En la diagonal principal\n";
				ret+=1;

			}
		}

		//En la diagonal hacia abajo
		if (fila>0 and columna<6){
			if ((estado.See_Casilla(fila,columna)%3)     == (estado.See_Casilla(fila-1,columna+1)%3)){
				//cout << "En la diagonal secundaria\n";

				ret+=1;

			}
		}
	}

	return ret;
}


// Dada una posici�n(fil,col), y un jugador, devuelve el n�mero de 3 en l�nea que tiene ese jugador
int EnLinea3(const Environment &estado, int fila, int columna, int jugador){
	int ret = 0;
	int ganador=0;

	if (estado.See_Casilla(fila,columna)==0){
		return ret;
	}

	ganador= (estado.See_Casilla(fila,columna)%3);
	if (ganador == jugador){

	
		//En la misma Fila
		if (columna<5){
			if ( (estado.See_Casilla(fila,columna)%3)   == (estado.See_Casilla(fila,columna+1)%3) and
				(estado.See_Casilla(fila,columna+1)%3) == (estado.See_Casilla(fila,columna+2)%3) ){
				//cout << "En la misma fila\n";
				ret+=1;

			}
		}
		//En la misma columna
		if (fila<5){
			if ((estado.See_Casilla(fila,columna)%3)   == (estado.See_Casilla(fila+1,columna)%3) and
				(estado.See_Casilla(fila+1,columna)%3) == (estado.See_Casilla(fila+2,columna)%3) ){
				//cout << "En la misma columna\n";

				ret+=1;	
				
			}
		}
		//En la diagonal hacia arriba
		if (fila<5 and columna<5){
			if ((estado.See_Casilla(fila,columna)%3)     == (estado.See_Casilla(fila+1,columna+1)%3) and
				(estado.See_Casilla(fila+1,columna+1)%3) == (estado.See_Casilla(fila+2,columna+2)%3) ){
			  	//cout << "En la diagonal principal\n";

				ret+=1;

			}
		}

		//En la diagonal hacia abajo
		if (fila>1 and columna<5){
			if ((estado.See_Casilla(fila,columna)%3)     == (estado.See_Casilla(fila-1,columna+1)%3) and
				(estado.See_Casilla(fila-1,columna+1)%3) == (estado.See_Casilla(fila-2,columna+2)%3) ){
				//cout << "En la diagonal secundaria\n";

				ret+=1;

			}
		}
	}

	//cout << "3LINEA ENCONTRADAS de " << jugador << ": " << ret;
	return ret;

}

// 0 si no hay, 1 si hay 4 en linea (da igual saber n�mero de 4 en l�nea)
int EnLinea4(const Environment &estado, int fila, int columna, int jugador){
	//int ret = 0;
	int ganador=0;

	if (estado.See_Casilla(fila,columna)==0){
		return 0;
	}

	ganador= (estado.See_Casilla(fila,columna)%3);
	if (ganador == jugador){

		//cout << "CASILLA " << fila << ", " << columna << " ANALIZADA DEL JUGADOR " << jugador << endl;

	
		//En la misma Fila
		if (columna<4){
			if ( (estado.See_Casilla(fila,columna)%3)   == (estado.See_Casilla(fila,columna+1)%3) and
				(estado.See_Casilla(fila,columna+1)%3) == (estado.See_Casilla(fila,columna+2)%3)  and
				(estado.See_Casilla(fila,columna+2)%3) == (estado.See_Casilla(fila,columna+3)%3) ){
				//cout << "En la misma fila\n";
				return 1;
			
			}
		}
		//En la misma columna
		if (fila<4){
			if ( (estado.See_Casilla(fila,columna)%3)   == (estado.See_Casilla(fila+1,columna)%3) and
				(estado.See_Casilla(fila+1,columna)%3) == (estado.See_Casilla(fila+2,columna)%3) and
				(estado.See_Casilla(fila+2,columna)%3) == (estado.See_Casilla(fila+3,columna)%3)){
				//cout << "En la misma columna\n";

				return 1;	
				
				/*if (columna == 6){
					cout << "RET: " << ret << " que es 4RAYA en ESQUINA DER(columna): " << endl;
				}*/
			}
		}
		//En la diagonal hacia arriba
		if (fila<4 and columna<4){
			if ((estado.See_Casilla(fila,columna)%3) == (estado.See_Casilla(fila+1,columna+1)%3) and
				(estado.See_Casilla(fila+1,columna+1)%3) == (estado.See_Casilla(fila+2,columna+2)%3) and
				(estado.See_Casilla(fila+2,columna+2)%3) == (estado.See_Casilla(fila+3,columna+3)%3) ) {
			  	//cout << "En la diagonal principal\n";

				return 1;
				
			}
		}

		//En la diagonal hacia abajo
		if (fila>2 and columna<4){
			if ((estado.See_Casilla(fila,columna)%3)     == (estado.See_Casilla(fila-1,columna+1)%3) and
				(estado.See_Casilla(fila-1,columna+1)%3) == (estado.See_Casilla(fila-2,columna+2)%3) and
				(estado.See_Casilla(fila-2,columna+2)%3) == (estado.See_Casilla(fila-3,columna+3)%3)){
				//cout << "En la diagonal secundaria\n";

				return 1;
				
			}
		}
	}

	//cout << "3LINEA ENCONTRADAS de " << jugador << ": " << ret;
	return 0;

}




// MI Funcion heuristica
// Valores elevados indican estados m�s favorables para ganar, y negativos lo contrario
double Valoracion(const Environment &estado){
	/*int rival;
	if (jugador==1){
		rival=2;
	}
	else{
		rival=1;
	}*/

	// Version PRO incorpora mejoras sustanciales como diferenciar EARLY, MID, LATE(COMBOS)

	// IDEA: GANAR LAS ESQUINAS, GENERAR� MENOS CONEXIONES QUE EL CENTRO
	//double esquinas = valorarEsquinas(estado);

	// double PELIGRO_LINEA
	double valor = 0;
	int var3 = 0;
	//int num2Linea = 0;
	int num3Linea = 0;
	int num4Linea = 0;		// 4 en linea nuestras (QUEREMOS MINIMIZAR)

	// POSIBLE EARLY
	// Guardamos la ocupacion de cada columna (valores de 0:vac�a a 7:llena)
	
	/*
	int casillasLibres = Get_Casillas_Libres();*/
	// VER NUMERO DE 3 EN RAYA DEL OTRO JUGADOR
	for (int i=0; i<7; i++){
		for (int j=0; j<7; j++){
			//num2Linea -= EnLinea2(estado,i,j,jugador);
			//var3= EnLinea3(estado,i,j,jugador);
			//num3Linea = num3Linea-var3;
			var3 = EnLinea4(estado,i,j,estado.JugadorActivo());
			num4Linea = num4Linea - var3;
		}
	}
	// Devolvemos un peso que ajuste las conexiones de 2,3 y 4 en raya encontradas
	// 2 en raya: Peso de 1
	// 3 en raya: Peso de 10
	// 4 en raya: Peso MAX 1000

	//valor =10*num3Linea + 1000*num4Linea;
	return num4Linea;

}




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


// Funci�n que Genera el �rbol de juego y lo eval�a seg�n Minimax de forma recursiva para evaluar el nodo current y devolver la acci�n elegida
double EvaluaMinimax(int iter, const Environment &current, int prof, int jugador, int MAX, int MAXPROF, int &accionElegida){     // ##### MEJOR OPCION??

	// accionElegida: Indica el hijo responsable de tal peso (SIGUIENTE JUGADA)
	//int MAX = jugador;		// MAX es nuestro jugador
	int MIN;	// MIN es el otro
	if (MAX == 1){	// Si MAX es el jugador VERDE
		MIN = 2;
	}
	else{
		MIN = 1;
	}
	double average;			// Representa el peso de la heur�stica escogido seg�n Minimax en funci�n de sus hijos para el nodo current
	
	Environment posiblesEstados[8];
	bool posibles[8];
	int n_posibles = current.possible_actions(posibles);

	//int desfase = 1;		// Mide el desfase entre los 8 posibles movimientos y las acciones posibles para actualizar correctamente la acci�n
	int j = 0;
	int t=0;
	int act = 0;				// Guarda la accion asociada al sucesor escogido

	//cout << "Generando sucesores\n" << endl;
	
	int n_act = current.GenerateAllMoves(posiblesEstados); // Obtengo las acciones aplicables al estado actual en "aplicables"*/

	
	//cout << " GENERADAS " << n_act << " y POSIBLES " << n_posibles << endl;
	if (prof == MAXPROF or n_act == 0){		// Es un NODO TERMINAL, no hay acciones posibles a desarrollar desde el estado actual
		//(jugador==MAX) ? cout << " MAX ha alcanzado un nodo terminal " : cout << " MIN ha alcanzado un nodo terminal ";
		//accion = Environment::actIDLE;

		//accionElegida = -1;
		return Valoracion(current);
	}

	// A PARTIR DE LA FICHA BOMBA, UN JUGADOR PIERDE
	

	else { // Hay varios estados posibles desde el actual

		if (jugador == MAX){						// Si nos encontramos en un nodo MAX, los hijos ser�n MIN
			//next.jugador = MIN;
			average = menosinf;

			for (int i=0; i<8; i++){	// Para cada posible sucesor, generamos su nodo en el �rbol
				
				if (posibles[i]==true and j<n_act){	// Si se trata de un movimiento posible, generamos y evaluamos el sucesor

					double valorHijo = EvaluaMinimax(iter+1, posiblesEstados[j], prof+1, MIN, MAX, MAXPROF, accionElegida);

					if (prof == 0){
						cout << "valor: " << valorHijo << endl;
					}
			
					if (average < valorHijo){	// Al estar en un nodo MAX, su evaluaci�n es el m�ximo de las evals de los hijos
						average = valorHijo;				// Actualizamos el peso minimax del nodo
						//accionElegida = posiblesEstados[i].Last_Action(MAX);					// Guardamos el posibleEstado que representa 
						act = i;
						
					}

					j++;
				}
			}

		}

		else{												// Si nos encontramos en un nodo MIN, los hijos ser�n MAX
			//next.jugador = MAX;
			average = masinf;

			//cout << "NODO MIN: " << jugador << endl;

			for (int i=0; i<8; i++){	// Para cada posible sucesor, generamos su nodo en el �rbol
				
				if (posibles[i]==true and j < n_act){	// Si se trata de un movimiento posible, generamos y evaluamos el sucesor
					int valorHijo = EvaluaMinimax(iter+1, posiblesEstados[j], prof+1, MAX, MAX, MAXPROF, accionElegida);

					if (average > valorHijo){	// Al estar en un nodo MAX, su evaluaci�n es el m�ximo de las evals de los hijos
						average = valorHijo;				// Actualizamos el peso minimax del nodo
						//accionElegida = posiblesEstados[i].Last_Action(MIN);					// Guardamos el posibleEstado que representa 
						act = i;
					}

					j++;
				}
			}
		}

		//accion = static_cast< Environment::ActionType > (posiblesEstados[hijoAverage]);		// Guardamos la accion elegida por Minimax
		
		accionElegida = act;
		return average;

		//current.evaluacion;

	}
	

}


// Implementa el Algoritmo Minimax para jugar la partida
// En Teor�a de Juegos, este aloritmo representa un m�todo de decisi�n de juegos con adversario
//  para MINIMIZAR la P�RDIDA M�XIMA esperada por el contrincante 
// Devuelve el valor obtenido tras la b�squeda minimax desde el estado actual
double Player::JuegoMinimax(const Environment & actual, int jugador, int MAXPROF, Environment::ActionType &accion){     // ##### MEJOR OPCION??
	
	// PASOS DEL ALGORITMO MINIMAX
	//  Generar �rbol de juego
	//  Evaluar (siempre con la heur�stica) los nodos terminales del �rbol
	//  Evaluar sucesivamente los nodos padres a partir de los terminales (hacia arriba) hasta la raiz
	//  Escoger la jugada de profundidad 1 (directamente realizable) que optimiza la heur�stica (VALOR MAS ALTO)
	  //     Escoge la siguiente acci�n siguiendo el mejor camino que puede presuponer

	/*j=0;

	for (int i=0; i<8; i++){	// Para cada una de las 8 posibles jugadas en ese estado
		if (aplicables[i]){
			opciones[j]=i;
			j++;
		}
	}*/

	int prof = 0;
	int accionElegida;
	int n_act;		// N�mero de acciones posibles a partir de ese estado
	Environment *posiblesEstados;

	//set<estado,ComparaEstados> visitados;	// Cerrados (inicialmente vacio): Conjunto de estados explorados.
	//arbol = [];						// �rbol Minimax de posibles estados desde el actual

	cout << "Generando y evaluando arbol de juego\n" << endl;

	/*nodo current;					// Definimos el Nodo Raiz del arbol de b�squeda con el estado actual de la partida
	current.estado = actual;
	current.jugador = jugador;
	current.prof = prof;*/

	(jugador==1) ? cout << "Nodo RAIZ con jugador Verde: " << endl : cout << "Nodo RAIZ con jugador Azul: " << endl;

	// Evaluamos desde el nodo raiz para obtener su peso y decisi�n a tomar
	double evalRaiz = EvaluaMinimax(0, actual, prof, jugador, jugador, MAXPROF, accionElegida);

	// Actualizamos la acci�n elegida por Minimax a realizar
	if (accionElegida >= 0){
		accion = static_cast< Environment::ActionType > (accionElegida);		// Guardamos la accion elegida por Minimax

		/*cout << " ACCI�N DECIDIDA CON HEURISTICA " << evalRaiz << " :  " 
				<< actual.ActionStr(  static_cast< Environment::ActionType > (accionElegida)  ) << endl;*/

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

	Environment::ActionType accion; // acci�n que se va a devolver
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
	/*Environment *posiblesEstados;
	int numPos = actual_.GenerateAllMoves(posiblesEstados);*/

	valor = JuegoMinimax(actual_, jugador_, PROFUNDIDAD_MINIMAX, accion);
	cout << "Valor MiniMax: " << valor << "  Accion: " << actual_.ActionStr(accion) << endl;

	return accion;
}

