#include "../Comportamientos_Jugador/jugador.hpp"
#include "motorlib/util.h"

#include <iostream>
#include <limits.h>
#include <list>
#include <cmath>
#include <set>
#include <stack>
#include <queue>

/* --- MODIFICADO POR: 	Víctor García Carrera, victorgarcia@correo.ugr.es --- */


// Este es el método principal que implementa el comportamiento inteligente de nuestro agente reactivo/deliberativo.
// Contiene los 4 Comportamientos_Jugador que se piden en la práctica.

// Por un lado, controla la elaboracion de un plan para llegar al objetivo utilizando los 4 comportamientos
// de los que dispone dependiendo del caso en el que se encuentre.

// Por otro lado, controla la ejecución del plan elaborado, que se lleve a cabo y pueda reaccionar ante cambios imprevistos

// Tiene como entrada la información de los sensores y devuelve la acción a realizar.
Action ComportamientoJugador::think(Sensores sensores) {
	Action accion = actFORWARD;	// Accion que realizara el agente
	
	if (sensores.nivel != 4){
		// Estoy en el nivel 1 (LVL1)

		// Los sensores mensajeF y mensajeC se activan la primera vez que el jugador debe decidir
		// En la primera iteracion del juego, el sensor se activa con un valor != -1 para dar al jugador su posicion inicial (fila y columna)
		if (sensores.mensajeF != -1){
			fil = sensores.mensajeF;
			col = sensores.mensajeC;
			brujula = 0; // 0 corresponde con Norte

			/*actual.fila = fil;
			actual.columna = col;
			actual.orientacion = brujula;*/

			destino.fila = sensores.destinoF;
			destino.columna = sensores.destinoC;
		}

		// Actualizamos la posicion del jugador (fil, col) y su orientacion con el efecto de su ultima accion
		actualizaPosicion(sensores);

		// SE PUEDE OMITIR, o para DEST CAMBIANTES
		// Comprobamos si ha cambiado la casilla objetivo y hay que actualizar destino
		if (sensores.destinoF != destino.fila or sensores.destinoC != destino.columna){
			destino.fila = sensores.destinoF;
			destino.columna = sensores.destinoC;
			hayPlan = false;
		}

		/* -- DESARROLLO DEL PLAN --*/

		// GESTIONAR QUE CUANDO FALLA EL PLAN (OBSTACULO), SE RECALCULE !!!

		// Si el agente no ha deliberado un camino hasta el destino, lo calcula
		if (!hayPlan){
			actual.fila = fil;
			actual.columna = col;
			actual.orientacion = brujula;
			// (LVL2): ACTUALIZAR DESTINO !!!!
			//destino.fila = sensores.destinoF;
			//destino.columna = sensores.destinoC;

			hayPlan = pathFinding (sensores.nivel, actual, destino, plan);
		}

		/* -- CONTROL DEL PLAN -- */

		// Si hay un plan con una o mas acciones, controlamos que se ejecute el plan
		if (hayPlan and plan.size()>0){
			accion = plan.front();		// Obtenemos el primer elemento de la lista, es decir, la primera accion del camino planeado
			plan.erase(plan.begin());	// Borramos esta primera accion del plan para actualizar la siguiente accion (la 2da pasa a ser 1era)
		}
		else{		// NO hay plan que seguir
			// podemos valorar RECALCULAR el plan (tbien por obstaculos o cambios inesperados)
			
			// Version REACTIVA donde observa por los sensores y EVITA OBSTACULOS, resultado de NO TENER PLAN
			if (sensores.terreno[2]=='P' or sensores.terreno[2]=='M' or
				sensores.terreno[2]=='D' or sensores.superficie[2]=='a'){
			
				accion = actTURN_R;		// (LVL2) POR DEFECTO, CAMBIAR Y AJUSTAR A LO MAS CERCANO EN RUTA
			}
		}

		/* Sistema de movimiento, EVITA OBSTACULOS
		// Adaptado basicamente al LVL1 donde podemos leer siempre de mapaResultado la casilla delante del jugador
		unsigned char casillaDelante;
		switch(brujula){
			case 0: casillaDelante = mapaResultado[fil-1][col]; break;
			case 1: casillaDelante = mapaResultado[fil][col+1]; break;
			case 2: casillaDelante = mapaResultado[fil+1][col]; break;
			case 3: casillaDelante = mapaResultado[fil][col-1]; break;
		}

		if (casillaDelante=='P' or casillaDelante=='M' or
			casillaDelante=='D' or sensores.superficie[2]=='a'){

			accion = actTURN_R;		// (LVL2) POR DEFECTO, CAMBIAR Y AJUSTAR A LO MAS CERCANO EN RUTA
		} */

	}
	else {
		// Estoy en el nivel 2
		cout << "Aún no implementado el nivel 2" << endl;
	}

	// Recordamos para la siguiente iteracion la accion tomada
	ultimaAccion = accion;
	return accion;
}

// Actualiza la posicion del jugador (fil, col) y su orientacion con el efecto de su ultima accion
void ComportamientoJugador::actualizaPosicion(Sensores sensores){
	// Notacion brujula 0:N 1:E 2:S 3:O (4 sería N de nuevo) en sentido horario, +1 mod4 implica girar der(R), -1=+3 en mod4 girar izq(L)
	switch(ultimaAccion){
		case actTURN_R: brujula = (brujula+1)%4; break;
		case actTURN_L: brujula = (brujula+3)%4; break;
		case actFORWARD:
			switch(brujula){			// Si la accion fue caminar hacia delante, en función de su orientacion (que mantiene igual que antes)
				case 0: fil--; break;	// actualizamos correctamente la fila y columna de su posicion actual
				case 1: col++; break;
				case 2: fil++; break;
				case 3: col--; break;
			}
		break;
	}

	//cout << "Fila: " << fil << " Col: " << col << " Orientacion: " << brujula << endl;
	
}



// Llama al algoritmo de busqueda que se usará en cada comportamiento del agente
// Level representa el comportamiento en el que fue iniciado el agente.
bool ComportamientoJugador::pathFinding (int level, const estado &origen, const estado &destino, list<Action> &plan){
	switch (level){
		case 1: cout << "Busqueda en profundad\n";
			      		return pathFinding_Profundidad(origen,destino,plan);
						break;
		case 2: cout << "Busqueda en Anchura\n";
			     		return pathFinding_Anchura(origen,destino,plan);
						break;
		case 3: cout << "Busqueda Costo Uniforme\n";
						return pathFinding_Uniforme(origen,destino,plan);
						break;
		case 4: cout << "Busqueda para el reto\n";
						// Incluir aqui la llamada al algoritmo de búsqueda usado en el nivel 2
						break;
	}
	cout << "Comportamiento sin implementar\n";
	return false;
}


//---------------------- Implementación de la PLANIFICACION (busquedas en arbol de estados) ---------------------------

// TENER EN CUENTA TIPO DE TERRENO --> Quizas en coste uniforme????
// (LVL2): VALORAR OBSTACULO ALDEANO!!!!!!!!
// MIRAR A LOS LADOS PARA VER + MAPA
// HACER BUSQUEDA UNIFORME HASTA CASILLA DESCONOCIDA (si le damos coste 1 a casilla desconocida las explorará)

// PROBLEMA LVL2 CASILLAS DESCONOCIDAS (VALORAR ASIGNARLES PESO 1 A DESC) !!!!!!!!


/* ---RESUMEN--- Uniforme tiene en cuenta el terreno (NECESARIO), pero MUCHO TIEMPO
//				 MODIFICARLO V2.0 para --> va disminuyendo coste ciertas casillas para que las explore
//				 NO puede quedarse PARADO pensando planes, INUTIL
				// ANTES PK
//				 Mirar a los lados para + mapa, aprovechar camino hasta PK para matriz
				   DESPUES PK
				 Ir completando matriz al movernos al destino
				 DISTANCIAS CORTAS CON BUSQUEDA COSTE UNIFORME (MANTENDIENDO ORIENTACION DESTINO)
*/			


// Mirar Wikipedia & Otras fuentes de las busquedas



// Dado el código en carácter de una casilla del mapa dice si se puede
// pasar por ella sin riegos de morir o chocar.
bool EsObstaculo(unsigned char casilla){
	if (casilla=='P' or casilla=='M' or casilla =='D')
		return true;
	else
	  return false;
}


// Comprueba si la casilla que hay delante es un obstaculo. Si es un
// obstaculo devuelve true. Si no es un obstaculo, devuelve false y
// modifica st con la posición de la casilla del avance.
bool ComportamientoJugador::HayObstaculoDelante(estado &st){
	int fil=st.fila, col=st.columna;

  // calculo cual es la casilla de delante del agente
	switch (st.orientacion) {
		case 0: fil--; break;
		case 1: col++; break;
		case 2: fil++; break;
		case 3: col--; break;
	}

	// Compruebo que no me salgo fuera del rango del mapa
	if (fil<0 or fil>=mapaResultado.size()) return true;
	if (col<0 or col>=mapaResultado[0].size()) return true;

	// Miro si en esa casilla hay un obstaculo infranqueable
	if (!EsObstaculo(mapaResultado[fil][col])){
		// No hay obstaculo, actualizo el parámetro st poniendo la casilla de delante.
    	st.fila = fil;
		st.columna = col;
		return false;
	}
	else{
	  return true;
	}
}



// ---- Version valida para busqueda en anchura/profundidad ----
struct nodo{
	estado st;
	list<Action> secuencia;
};

// ---- Version valida para busqueda con coste Uniforme ----
struct nodoCoste{
	estado st;
	int coste;
	list<Action> secuencia;
};

struct ComparaEstados{
	bool operator()(const estado &a, const estado &n) const{
		if ((a.fila > n.fila) or (a.fila == n.fila and a.columna > n.columna) or
	      (a.fila == n.fila and a.columna == n.columna and a.orientacion > n.orientacion))
			return true;
		else
			return false;
	}
};


//---------------------- Implementación de la busqueda en Profundidad ---------------------------


// Implementación de la búsqueda en profundidad. BUSQUEDA SIN INFORMACION
// PILA LIFO
// Entran los puntos origen y destino y devuelve la
// secuencia de acciones en plan, una lista de acciones.
bool ComportamientoJugador::pathFinding_Profundidad(const estado &origen, const estado &destino, list<Action> &plan) {
	//Borro la lista
	cout << "Calculando plan\n";
	plan.clear();
	set<estado,ComparaEstados> generados;	// Cerrados: Conjunto de estados explorados. Inicialmente vacio
	stack<nodo> pila;						// Abiertos: PILA LIFO en el caso de la busqueda en profundidad, de nodos a explorar.

  	nodo current;
	current.st = origen;
	current.secuencia.empty();

	pila.push(current);		// El primer nodo a explorar es el nodo origen

  while (!pila.empty() and (current.st.fila!=destino.fila or current.st.columna != destino.columna)){

		pila.pop();
		generados.insert(current.st);		// Insertamos el estado actual en el conjunto de cerrados (estados visitados)

		// Generar descendiente de girar a la derecha
		nodo hijoTurnR = current;
		hijoTurnR.st.orientacion = (hijoTurnR.st.orientacion+1)%4;
		if (generados.find(hijoTurnR.st) == generados.end()){		// Si no habiamos visitado este nodo antes
			hijoTurnR.secuencia.push_back(actTURN_R);				// Actualizamos la secuencia de acciones hasta este nodo desde el nodo origen
			pila.push(hijoTurnR);									// lo insertamos en la pila de nodos a explorar

		}

		// Generar descendiente de girar a la izquierda
		nodo hijoTurnL = current;
		hijoTurnL.st.orientacion = (hijoTurnL.st.orientacion+3)%4;
		if (generados.find(hijoTurnL.st) == generados.end()){
			hijoTurnL.secuencia.push_back(actTURN_L);
			pila.push(hijoTurnL);
		}

		// Generar descendiente de avanzar
		nodo hijoForward = current;
		if (!HayObstaculoDelante(hijoForward.st)){
			if (generados.find(hijoForward.st) == generados.end()){
				hijoForward.secuencia.push_back(actFORWARD);
				pila.push(hijoForward);
			}
		}

		// Tomo el siguiente valor de la pila
		if (!pila.empty()){
			current = pila.top();
		}
	}

  cout << "Terminada la busqueda\n";

  	// Si ha encontrado el destino objetivo
	if (current.st.fila == destino.fila and current.st.columna == destino.columna){
		cout << "Cargando el plan\n";
		plan = current.secuencia;
		cout << "Longitud del plan: " << plan.size() << endl;
		PintaPlan(plan);
		// ver el plan en el mapa
		VisualizaPlan(origen, plan);
		return true;
	}
	else {
		cout << "No encontrado plan\n";
	}


	return false;
}



//---------------------- Implementación de la busqueda en Anchura ---------------------------

// Implementación de la búsqueda en anchura. BUSQUEDA SIN INFORMACION
// COLA FIFO
// Entran los puntos origen y destino y devuelve la
// secuencia de acciones en plan, una lista de acciones.
bool ComportamientoJugador::pathFinding_Anchura(const estado &origen, const estado &destino, list<Action> &plan) {
	//Borro la lista
	cout << "Calculando plan\n";
	plan.clear();
	set<estado,ComparaEstados> visitados;	// Cerrados (inicialmente vacio): Conjunto de estados explorados.
	queue<nodo> cola;						// Abiertos (inicialmente vacio): Cola de nodos a explorar

  	nodo current;					// Nodo con el que recorremos el arbol de estados posibles (arbol de búsqueda)
	current.st = origen;
	current.secuencia.empty();

	cola.push(current);				// El primer nodo a explorar es el nodo origen

  while (!cola.empty() and (current.st.fila!=destino.fila or current.st.columna != destino.columna)){

		cola.pop();
		visitados.insert(current.st);		// Insertamos el estado actual en el conjunto de cerrados (estados visitados)

		// Generar descendiente de girar a la derecha
		nodo hijoTurnR = current;
		hijoTurnR.st.orientacion = (hijoTurnR.st.orientacion+1)%4;
		if (visitados.find(hijoTurnR.st) == visitados.end()){		// Si no habiamos explorado este nodo antes (su estado no está en los cerrados)
			hijoTurnR.secuencia.push_back(actTURN_R);				// Actualizamos la secuencia de acciones hasta este nodo desde el nodo origen

			cola.push(hijoTurnR);									// Insertamos el nodo en la cola de nodos a explorar (los abiertos)

		}

		// Generar descendiente de girar a la izquierda
		nodo hijoTurnL = current;
		hijoTurnL.st.orientacion = (hijoTurnL.st.orientacion+3)%4;
		if (visitados.find(hijoTurnL.st) == visitados.end()){
			hijoTurnL.secuencia.push_back(actTURN_L);

			cola.push(hijoTurnL);
		}

		// Generar descendiente de avanzar
		nodo hijoForward = current;
		if (!HayObstaculoDelante(hijoForward.st)){
			if (visitados.find(hijoForward.st) == visitados.end()){
				hijoForward.secuencia.push_back(actFORWARD);
				if (hijoForward.st.fila == destino.fila and hijoForward.st.columna == destino.columna){
					current = hijoForward;		// Si el nuevo estado es el estado objetivo, lo guardamos en current y terminamos
					break;
				}
				cola.push(hijoForward);
			}
		}

		// Tomo el siguiente valor de la cola, el nodo a explorar (el mas antiguo de la cola)
		if (!cola.empty()){
			current = cola.front();		// Cola FIFO
		}

	}

  cout << "Terminada la busqueda\n";

	if (current.st.fila == destino.fila and current.st.columna == destino.columna){
		cout << "Cargando el plan\n";
		plan = current.secuencia;
		cout << "Longitud del plan: " << plan.size() << endl;
		PintaPlan(plan);
		// ver el plan en el mapa
		VisualizaPlan(origen, plan);
		return true;
	}
	else {
		cout << "No encontrado plan\n";
	}


	return false;
}


//---------------------- Implementación de la busqueda con coste Uniforme ---------------------------

// PROBLEMA LVL2 CASILLAS DESCONOCIDAS (VALORAR ASIGNARLES PESO 1 A DESC) !!!!!!!!

// Implementación de la búsqueda con coste uniforme. BUSQUEDA SIN INFORMACION
// Esta búsqueda tiene en cuenta el coste asociado a cada arco en el árbol de estados (árbol de búsqueda)
// es decir, tiene en cuenta el coste de pasar del estado i al j, el coste de avanzar de la casilla i a la j (coste del terreno)
// El valor de cada arco (i,j) depende del tipo de terreno en la casilla (estado) del nodo j

// Entran los puntos origen y destino y devuelve la
// secuencia de acciones en plan, una lista de acciones.
bool ComportamientoJugador::pathFinding_Uniforme(const estado &origen, const estado &destino, list<Action> &plan) {
	//Borro la lista
	int pos;
	int min = INT_MAX;
	int posMin = 0;
	cout << "Calculando plan costo UNIFORME\n";
	plan.clear();
	set<estado,ComparaEstados> visitados;	// Cerrados (inicialmente vacio): Conjunto de estados explorados.
	vector<nodoCoste> abiertos;				// Abiertos (inicialmente vacio): Lista de nodos a explorar
	// Utilizamos una nueva struct de nodo que almacena el coste de avanzar hasta esa casilla asociado al tipo de terreno
  	nodoCoste current;					// Nodo con el que recorremos el arbol de estados posibles (arbol de búsqueda)
	current.st = origen;
	current.coste = 0;
	current.secuencia.empty();

	abiertos.push_back(current);				// El primer nodo a explorar es el nodo origen

  while (!abiertos.empty() and (current.st.fila!=destino.fila or current.st.columna != destino.columna)){

		for (pos = 0; pos < abiertos.size(); pos++){	// Buscamos el nodo a explorar de Abiertos con menor coste
			if (abiertos[pos].coste < min){		// Si encontramos un nodo a explorar con menor coste
				min = abiertos[pos].coste;		// Actualizamos el camino minimo
				posMin = pos;					// Guardamos la posicion en la lista de abiertos de este estado con menor coste
			}
		}
		current = abiertos[posMin];	// Procedemos a explorar el nodo de menor coste encontrado
		cout << "NODO EXPLORADO: " << current.st.fila << " " << current.st.columna << " COSTE: " << current.coste << endl;

		// Sacamos de la lista de Abiertos (nodos a explorar) el nodo de menor coste
		vector<nodoCoste>::iterator listMin = abiertos.begin() + posMin;
		abiertos.erase(listMin);
		// Metemos el nodo en el conjunto de Cerrados (nodos explorados)
		visitados.insert(current.st);		// Insertamos el estado actual en el conjunto de cerrados (estados visitados)
		min = INT_MAX;

		// Generar descendiente de girar a la derecha
		nodoCoste hijoTurnR = current;
		hijoTurnR.st.orientacion = (hijoTurnR.st.orientacion+1)%4;
		if (visitados.find(hijoTurnR.st) == visitados.end()){		// Si no habiamos explorado este nodo antes (su estado no está en los cerrados)
			hijoTurnR.secuencia.push_back(actTURN_R);				// Actualizamos la secuencia de acciones hasta este nodo desde el nodo origen
			hijoTurnR.coste += 1;					// El coste aumenta en 1 si se gira

			abiertos.push_back(hijoTurnR);									// Insertamos el nodo en la lista de nodos a explorar (los abiertos)
		}

		// Generar descendiente de girar a la izquierda
		nodoCoste hijoTurnL = current;
		hijoTurnL.st.orientacion = (hijoTurnL.st.orientacion+3)%4;
		if (visitados.find(hijoTurnL.st) == visitados.end()){
			hijoTurnL.secuencia.push_back(actTURN_L);
			hijoTurnL.coste += 1;					// El coste aumenta en 1 si se gira

			abiertos.push_back(hijoTurnL);
		}

		// Generar descendiente de avanzar
		nodoCoste hijoForward = current;
		if (!HayObstaculoDelante(hijoForward.st)){
			if (visitados.find(hijoForward.st) == visitados.end()){
				hijoForward.secuencia.push_back(actFORWARD);
				hijoForward.coste += costeAvanzar(current.st.fila, current.st.columna);
				// Actualizamos el coste del nodo con el coste de avanzar por el terreno en el que se encuentra para llegar a él

				if (hijoForward.st.fila == destino.fila and hijoForward.st.columna == destino.columna){
					current = hijoForward;		// Si el nuevo estado es el estado objetivo, lo guardamos en current y terminamos
					break;
				}
				abiertos.push_back(hijoForward);
			}
		}

		

	}

  cout << "Terminada la busqueda\n";

	if (current.st.fila == destino.fila and current.st.columna == destino.columna){
		cout << "Cargando el plan\n";
		plan = current.secuencia;
		cout << "Longitud del plan: " << plan.size() << endl;
		PintaPlan(plan);
		// ver el plan en el mapa
		VisualizaPlan(origen, plan);
		cout << "COSTE del plan: " << current.coste << endl;
		return true;
	}
	else {
		cout << "No encontrado plan\n";
	}


	return false;
}

// Coste de avanzar por esa casilla. Consulta el tipo de terreno que es con mapaResultado
int ComportamientoJugador::costeAvanzar(int fila, int columna){
	unsigned char casilla = mapaResultado[fila][columna];
	if (casilla == 'T'){
		return 2;
	}
	else if (casilla == 'B'){
		return 5;
	}
	else if (casilla == 'A'){
		return 10;
	}
	else{
		return 1;
	}
}

// Sacar por la términal la secuencia del plan obtenido
void ComportamientoJugador::PintaPlan(list<Action> plan) {
	auto it = plan.begin();
	while (it!=plan.end()){
		if (*it == actFORWARD){
			cout << "A ";
		}
		else if (*it == actTURN_R){
			cout << "D ";
		}
		else if (*it == actTURN_L){
			cout << "I ";
		}
		else {
			cout << "- ";
		}
		it++;
	}
	cout << endl;
}



void AnularMatriz(vector<vector<unsigned char> > &m){
	for (int i=0; i<m[0].size(); i++){
		for (int j=0; j<m.size(); j++){
			m[i][j]=0;
		}
	}
}


// Pinta sobre el mapa del juego el plan obtenido
void ComportamientoJugador::VisualizaPlan(const estado &st, const list<Action> &plan){
  AnularMatriz(mapaConPlan);
	estado cst = st;

	auto it = plan.begin();
	while (it!=plan.end()){
		if (*it == actFORWARD){
			switch (cst.orientacion) {
				case 0: cst.fila--; break;
				case 1: cst.columna++; break;
				case 2: cst.fila++; break;
				case 3: cst.columna--; break;
			}
			mapaConPlan[cst.fila][cst.columna]=1;
		}
		else if (*it == actTURN_R){
			cst.orientacion = (cst.orientacion+1)%4;
		}
		else {
			cst.orientacion = (cst.orientacion+3)%4;
		}
		it++;
	}
}



int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}
