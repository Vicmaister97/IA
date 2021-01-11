for (int j=0; j < 7; j++){
		
		int ocup = estado.Get_Ocupacion_Columna(j);
		if (ocup == 7){
			valor = menosinf;		// Si está llena, valoración negativa
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
			if (ocup < 2 or ocup > 5 ){		// ### Ver que justo las de debajo NO sean nuestras ni a los lados
				valor += 7;
			}
		}

	}



// Dada una posición(fil,col), y un jugador, devuelve el número de 3 en línea que tiene ese jugador
int EnLinea(const Environment &estado, int fila, int columna, int jugador){
	int ret = 0;
	int ganador=0;
	int linea2 = 0;
	int linea3 = 0;
	int linea4 = 0;

	if (estado.See_Casilla(fila,columna)==0){
		return ret;
	}

	
	//En la misma Fila
	if (columna<6){
		if ( (estado.See_Casilla(fila,columna)%3)   == (estado.See_Casilla(fila,columna+1)%3) ){
			ganador= (estado.See_Casilla(fila,columna)%3);

			if (ganador == jugador){

				linea2 +=1;
				if (columna<5){
					if ( (estado.See_Casilla(fila,columna+1)%3) == (estado.See_Casilla(fila,columna+2)%3) ){
						
						linea2 -=1;
						linea3 +=1;
						if (columna<4){
							if ( (estado.See_Casilla(fila,columna+2)%3) == (estado.See_Casilla(fila,columna+3)%3) ){
								if (ganador == jugador){
									linea3 -=1;
									linea4 +=1;
								}
							}
						}
						
					}
				}

			}
		}
		
	}

	//En la misma columna
	if (fila<6){

		if ( (estado.See_Casilla(fila,columna)%3)   == (estado.See_Casilla(fila+1,columna)%3) ){
			ganador= (estado.See_Casilla(fila,columna)%3);

			if (ganador == jugador){

				linea2 +=1;
				if (fila<5){
					if ( (estado.See_Casilla(fila+1,columna)%3) == (estado.See_Casilla(fila+2,columna)%3) ){
						
						linea2 -=1;
						linea3 +=1;
						if (fila<4){
							if ( (estado.See_Casilla(fila+2,columna)%3) == (estado.See_Casilla(fila+3,columna)%3) ) {
								if (ganador == jugador){
									linea3 -=1;
									linea4 +=1;
								}
							}
						}
						
					}
				}

			}
		}
		
	}
	//En la diagonal hacia arriba
	if (fila<6 and columna<6){

		if ( (estado.See_Casilla(fila,columna)%3)     == (estado.See_Casilla(fila+1,columna+1)%3) ){
			ganador= (estado.See_Casilla(fila,columna)%3);

			if (ganador == jugador){

				linea2 +=1;

				if (fila<5 and columna<5){
					if ( (estado.See_Casilla(fila+1,columna+1)%3) == (estado.See_Casilla(fila+2,columna+2)%3) ){
						
						linea2 -=1;
						linea3 +=1;
						if (fila<4 and columna<4){
							if ( (estado.See_Casilla(fila+2,columna+2)%3) == (estado.See_Casilla(fila+3,columna+3)%3) ){
								if (ganador == jugador){
									linea3 -=1;
									linea4 +=1;
								}
							}
						}
						
					}
				}

			}
		}

	}

	//En la diagonal hacia abajo
	if (fila>0 and columna<6){

		if ( (estado.See_Casilla(fila,columna)%3)     == (estado.See_Casilla(fila-1,columna+1)%3) ){
			ganador= (estado.See_Casilla(fila,columna)%3);

			if (ganador == jugador){

				linea2 +=1;
				if (fila>1 and columna<5){
					if ( (estado.See_Casilla(fila-1,columna+1)%3) == (estado.See_Casilla(fila-2,columna+2)%3) ) {
						
						linea2 -=1;
						linea3 +=1;
						if (fila>2 and columna<4){
							if ( (estado.See_Casilla(fila-2,columna+2)%3) == (estado.See_Casilla(fila-3,columna+3)%3) ){
								if (ganador == jugador){
									linea3 -=1;
									linea4 +=1;
								}
							}
						}
						
					}
				}

			}
		}

	}

	// Devolvemos un peso que ajuste las conexiones de 2,3 y 4 en raya encontradas
	// 2 en raya: Peso de 1
	// 3 en raya: Peso de 10
	// 4 en raya: Peso MAX 1000
	ret = linea2 + 10*(linea3) + 1000*(linea4);

	//cout << "3LINEA ENCONTRADAS de " << jugador << ": " << ret;
	return ret;

}







	// ############# VALORAR Last_Action() !!!

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















bool Environment::EnLinea(int fila, int columna, int &ganador) const{
    if (maze_[fila][columna]==0)
        return false;
    ganador=0;
    //En la misma Fila
    if (columna<4){
      if ((maze_[fila][columna]%3)   == (maze_[fila][columna+1]%3) and
          (maze_[fila][columna+1]%3) == (maze_[fila][columna+2]%3) and
          (maze_[fila][columna+2]%3) == (maze_[fila][columna+3]%3)){
          //cout << "En la misma fila\n";
          ganador= (maze_[fila][columna]%3);
          return true;
      }
    }
    //En la misma columna
    if (fila<4){
      if ((maze_[fila][columna]%3)   == (maze_[fila+1][columna]%3) and
          (maze_[fila+1][columna]%3) == (maze_[fila+2][columna]%3) and
          (maze_[fila+2][columna]%3) == (maze_[fila+3][columna]%3)){
          //cout << "En la misma columna\n";

          ganador= (maze_[fila][columna]%3);
          return true;
      }
    }
    //En la diagonal hacia arriba
    if (fila<4 and columna<4){
      if ((maze_[fila][columna]%3)     == (maze_[fila+1][columna+1]%3) and
          (maze_[fila+1][columna+1]%3) == (maze_[fila+2][columna+2]%3) and
          (maze_[fila+2][columna+2]%3) == (maze_[fila+3][columna+3]%3)){
          //cout << "En la diagonal principal\n";

          ganador= (maze_[fila][columna]%3);
          return true;
      }
    }

    //En la diagonal hacia abajo
    if (fila>2 and columna<4){
      if ((maze_[fila][columna]%3)   == (maze_[fila-1][columna+1]%3) and
          (maze_[fila-1][columna+1]%3) == (maze_[fila-2][columna+2]%3) and
          (maze_[fila-2][columna+2]%3) == (maze_[fila-3][columna+3]%3)){
          //cout << "En la diagonal secundaria\n";

          ganador= (maze_[fila][columna]%3);
          return true;
      }
    }

    return false;

}


// -----------------------------------------------------------
bool Environment::EnLinea2(int fila, int columna, int &ganador) const{
    if (maze_[fila][columna]==0)
        return false;
    ganador=0;
    //En la misma Fila
    if (columna<4){
      if ((maze_[fila][columna]%3)   == (maze_[fila][columna+1]%3) and
          (maze_[fila][columna+1]%3) == (maze_[fila][columna+2]%3) and
          (maze_[fila][columna+2]%3) == (maze_[fila][columna+3]%3)){
          //cout << "En la misma fila\n";
          ganador= (maze_[fila][columna]%3);
          if (ganador==1){
              maze_[fila][columna]=7;
              maze_[fila][columna+1]=7;
              maze_[fila][columna+2]=7;
              maze_[fila][columna+3]=7;
          }
          else{
              maze_[fila][columna]=8;
              maze_[fila][columna+1]=8;
              maze_[fila][columna+2]=8;
              maze_[fila][columna+3]=8;
          }
          return true;
      }
    }
    //En la misma columna
    if (fila<4){
      if ((maze_[fila][columna]%3)   == (maze_[fila+1][columna]%3) and
          (maze_[fila+1][columna]%3) == (maze_[fila+2][columna]%3) and
          (maze_[fila+2][columna]%3) == (maze_[fila+3][columna]%3)){
          //cout << "En la misma columna\n";

          ganador=(maze_[fila][columna]%3);
          if (ganador==1){
              maze_[fila][columna]=7;
              maze_[fila+1][columna]=7;
              maze_[fila+2][columna]=7;
              maze_[fila+3][columna]=7;
          }
          else{
              maze_[fila][columna]=8;
              maze_[fila+1][columna]=8;
              maze_[fila+2][columna]=8;
              maze_[fila+3][columna]=8;

          }
          return true;
      }
    }
    //En la diagonal hacia arriba
    if (fila<4 and columna<4){
      if ((maze_[fila][columna]%3)     == (maze_[fila+1][columna+1]%3) and
          (maze_[fila+1][columna+1]%3) == (maze_[fila+2][columna+2]%3) and
          (maze_[fila+2][columna+2]%3) == (maze_[fila+3][columna+3]%3)){
          //cout << "En la diagonal principal\n";

          ganador=(maze_[fila][columna]%3);
          if (ganador==1){
              maze_[fila][columna]=7;
              maze_[fila+1][columna+1]=7;
              maze_[fila+2][columna+2]=7;
              maze_[fila+3][columna+3]=7;
          }
          else{
              maze_[fila][columna]=8;
              maze_[fila+1][columna+1]=8;
              maze_[fila+2][columna+2]=8;
              maze_[fila+3][columna+3]=8;
          }
          return true;
      }
    }

    //En la diagonal hacia abajo
    if (fila>2 and columna<4){
      if ((maze_[fila][columna]%3)     == (maze_[fila-1][columna+1]%3) and
          (maze_[fila-1][columna+1]%3) == (maze_[fila-2][columna+2]%3) and
          (maze_[fila-2][columna+2]%3) == (maze_[fila-3][columna+3]%3)){
          //cout << "En la diagonal secundaria\n";

          ganador=(maze_[fila][columna]%3);
          if (ganador==1){
              maze_[fila][columna]=7;
              maze_[fila-1][columna+1]=7;
              maze_[fila-2][columna+2]=7;
              maze_[fila-3][columna+3]=7;
          }
          else{
              maze_[fila][columna]=8;
              maze_[fila-1][columna+1]=8;
              maze_[fila-2][columna+2]=8;
              maze_[fila-3][columna+3]=8;
          }
          return true;
      }
    }

    return false;

}