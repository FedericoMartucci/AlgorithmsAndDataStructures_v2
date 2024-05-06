#include "juego.h"
#include "archivos.h"
#include "curlReducido.h"

char mostrarMenu()
{
    char opcion;

    do
    {
        puts("Bienvenido al juego:");
        puts("[A] Jugar");
        puts("[B] Salir");
        printf("Ingrese su opción: ");
        fflush(stdin);
        scanf("%c", &opcion);
        system("cls");
        printf("Opción no válida. Por favor, ingrese A o B.\n");
    }
    while(toupper(opcion) != 'B' && toupper(opcion) != 'A');

    system("cls");

    return toupper(opcion);
}

int iniciarJuego()
{
    tJuego juego;
    char opcion;
    int estado;

    opcion = mostrarMenu();

    switch(opcion)
    {
    case 'A':
        estado = configurarJuego(&juego);
        mostrarOrdenJuego(&juego);
        mostrarInformacionJuego(&juego);
        iniciarTrivia(&juego);
        determinarPuntos(&juego);
        estado = imprimirResultados(&juego);
        break;
    case 'B':
        printf("¡Hasta luego!\n");
        estado = OK;
        break;
    }

    return estado;
}

int configurarJuego(tJuego* juego)
{
    CURL* cURL;
    int estado;

    if((estado = leerArchivoConfig(juego)) != OK)
        return estado;

    cargarJugadores(juego);
    mezclar(juego, juego->cantJugadores, mezclarJugadores);
    elegirDificultad(juego);

    if ((estado = cargarCURL(&cURL)) != OK)
        return estado;

    ///TODO: chequear las siguientes 3 lineas
    //    if((estado = verificarConectividad(&cURL)) != OK ){
    //        return estado;
    //    }

    if (cURL)
        estado = obtenerPreguntas(&cURL, juego->preguntas, juego->dificultad, juego->cantRondas);

    liberarCurl(&cURL);

    return estado;
}

void elegirDificultad(tJuego* juego)
{
    char dificultad[8];

    fprintf(stdout, "Ingrese la dificultad deseada (FACIL - MEDIA - DIFICIL): ");
    fflush(stdin);
    scanf("%7s", dificultad);

    if (strcmpi(dificultad, "FACIL") == 0)
    {
        juego->dificultad = FACIL;
    }
    else if (strcmpi(dificultad, "MEDIO") == 0)
    {
        juego->dificultad = MEDIO;
    }
    else if (strcmpi(dificultad, "DIFICIL") == 0)
    {
        juego->dificultad = DIFICIL;
    }
    else
    {
        juego->dificultad = DIFICIL;
    }

    system("cls");
}

void mezclar(void* item, int cantElementos, void(*mezclarImpl)(void*, int))
{
    mezclarImpl(item, cantElementos);
}

void mostrarOrdenJuego(const tJuego* juego)
{
    int i;

    puts("Orden de juego:");
    for (i = 0; i < juego->cantJugadores; i++)
        printf("Turno %d: %s\n", i + 1, juego->jugadores[i].nombre);
}

void mostrarInformacionJuego(const tJuego* juego)
{
    printf("\nCantidad de rondas: %d\n", juego->cantRondas);
    printf("Tiempo por ronda: %d\n\n", juego->tiempoRonda);
}

void iniciarTrivia(tJuego* juego)
{
    int i;
    int rondaActual;
    int menorTiempo;
    int tiempoDeRespuesta;
    char respuesta;
    char opciones[CANT_OPCIONES][TAM_OPCION];

    menorTiempo = juego->tiempoRonda;
    respuesta = '\0';

    for(i = 0; i < juego->cantJugadores; i++)
    {
        printf("%s, ¿estás listo para iniciar el juego?\n", juego->jugadores[i].nombre);
        printf("Si esta listo, ingrese una tecla para comenzar...\n");
        getch();
        fflush(stdin);
        system("cls");
        printf("Jugador actual: %s", juego->jugadores[i].nombre);

        for(rondaActual = 0; rondaActual < juego->cantRondas; rondaActual++)
        {
            puts("");
            cargarYMezclarOpciones(opciones, &juego->preguntas[rondaActual]);
            printf("\nPregunta %d: %s\n", rondaActual + 1, juego->preguntas[rondaActual].pregunta);
            for (int i = 0; i < CANT_OPCIONES; i++)
                printf("%c- %s\n", 'A' + i, opciones[i]);
            printf("Respuesta: ");
            tiempoDeRespuesta = iniciarTemporizador(&respuesta, juego->tiempoRonda);
            juego->jugadores[i].respuestas[rondaActual].esCorrecta = strcmp(respuesta == '\0'? "" : opciones[toupper(respuesta) - 'A'], juego->preguntas[rondaActual].resp_correcta) == 0;
            if(menorTiempo > tiempoDeRespuesta && juego->jugadores[i].respuestas[rondaActual].esCorrecta)
                menorTiempo = tiempoDeRespuesta;
            juego->menorTiempoRespuesta[rondaActual] = menorTiempo;
            juego->jugadores[i].respuestas[rondaActual].tiempoDeRespuesta = tiempoDeRespuesta;
            strcpy(juego->jugadores[i].respuestas[rondaActual].opcion, respuesta == '\0'? "" : opciones[toupper(respuesta) - 'A']);
            juego->jugadores[i].respuestas[rondaActual].puntaje = 0;
        }
        printf("\n\nSu turno ha finalizado, ingrese una tecla para continuar");
        getch();
        system("cls");
    }
    puts("Juego terminado, ingrese cualquier tecla para continuar");
    getch();
    system("cls");
}

int iniciarTemporizador(char* respuesta, int tiempoLimite)
{
    char key;
    time_t startTime;
    int cursorPosition ;  // Posición del cursor en el buffer

    *respuesta = '\0';
    startTime = time(NULL);
    cursorPosition = 0;

    while (difftime(time(NULL), startTime) < tiempoLimite)
    {
        usleep(1000);  // duerme la entrada cada 1 milisegundos, importante asi para no cargar al procesador en un bucle hiper rapido
        if (_kbhit())
        {
            // Si el usuario presiona una tecla, la almacenamos
            key = _getch();
            if (key == RETORNO_DE_CARRO)    // Tecla Enter
            {
                break;        // Salir si se presiona Enter
            }
            else if (key == BACKSPACE)      // Retroceso (Backspace)
            {
                if (cursorPosition > 0)
                {
                    // Solo retroceder si no estamos al principio del buffer
                    cursorPosition--;
                    printf("\b \b");  // Retrocede y borra un carácter en la pantalla
                    *respuesta = '\0';  // Borra el carácter retrocedido en el buffer
                }
            }
            else if (esLetraValida(toupper(key)) && cursorPosition == 0)
            {
                // Almacena el carácter en el buffer y muestra en pantalla
                *respuesta = key;
                printf("%c", toupper(key));
                cursorPosition++;
            }
        }
    }

    if(difftime(time(NULL), startTime) >= tiempoLimite)
        printf("- No puede contestar, el tiempo ha finalizado");
    fflush(stdin);
    return (int)difftime(time(NULL), startTime);
}

int esLetraValida(char key)
{
    return key == 'A' ||
           key == 'B' ||
           key == 'C' ||
           key == 'D';
}

void determinarPuntos(tJuego* juego)
{
    int ronda;
    int jugador;
    int correctasEnMenorTiempoPorRonda;

    for (ronda = 0; ronda < juego->cantRondas; ronda++)
    {
        correctasEnMenorTiempoPorRonda = obtenerCorrectasEnMenorTiempo(juego->jugadores, juego->cantJugadores, ronda, juego->menorTiempoRespuesta[ronda]);
        for (jugador = 0; jugador < juego->cantJugadores; jugador++)
        {
            calcularPuntajePorJugador(&juego->jugadores[jugador], ronda,
                                      juego->menorTiempoRespuesta[ronda],
                                      correctasEnMenorTiempoPorRonda);
        }
    }
}

int obtenerCorrectasEnMenorTiempo(const tJugador* jugadores, int cantJugadores,
                                  int nroRonda, int menorTiempo)
{
    int jugador;
    int correctasEnMenorTiempo;

    correctasEnMenorTiempo = 0;

    for (jugador = 0; jugador < cantJugadores; jugador++)
        if(jugadores[jugador].respuestas[nroRonda].esCorrecta &&
                jugadores[jugador].respuestas[nroRonda].tiempoDeRespuesta ==  menorTiempo)
            correctasEnMenorTiempo++;

    return correctasEnMenorTiempo;
}
