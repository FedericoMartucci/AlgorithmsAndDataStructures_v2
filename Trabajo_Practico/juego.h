#ifndef JUEGO_H_INCLUDED
#define JUEGO_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
#include <time.h>
#include <unistd.h>

#include "curlReducido.h"
#include "pregunta.h"
#include "jugador.h"
#include "estructuras.h"

#define RETORNO_DE_CARRO 13
#define BACKSPACE 8

char mostrarMenu();
int iniciarJuego();
int configurarJuego(tJuego* juego);
void elegirDificultad(tJuego* juego);
void mezclar(void* item, int cantElementos, void(*mezclarImpl)(void*, int));
void mostrarOrdenJuego(const tJuego* juego);
void mostrarInformacionJuego(const tJuego* juego);
void iniciarTrivia(tJuego* juego);
void iniciarTemporizador(char respuesta, int tiempoLimite);

#endif // JUEGO_H_INCLUDED
