#include "arbol.h"

int main()
{

//    int vec[TAM_VEC];
    int vec[] = {6, 9, 7, 2, 4, 1, 3, 13, 11, 12};
//    int vec[] = {6, 9, 7, 2, 4, 1, 3, 13, 11, 12, 6, 7};
    int i;
//    int claveInfo = 2;
    int altura;
    tArbol pa;
    tArbol paRecuperadoBin;
    tArbol paRecuperadoTxt;

    srand(time(NULL));
    crearArbol(&pa);
    crearArbol(&paRecuperadoBin);
    crearArbol(&paRecuperadoTxt);
//    generarVectorEnteros(vec, TAM_VEC);

    for(i = 0; i < sizeof(vec)/sizeof(vec[i]); i++)
    {
//        printf("%d ", vec[i]);
        insertarEnArbol_R(&pa, &vec[i], sizeof(vec[i]), cmpEnteros, mostrarEnteroPorConsola);
    }
    puts("");
    grabarArbolEnArchivo(&pa, "archGrabadoBinario.bin", recorrerPreOrden2, grabarRegistroBin, grabarArbolEnArchivoBin);
    grabarArbolEnArchivo(&pa, "archGrabadoTextoPrO.txt", recorrerPreOrden2, grabarEnteroTxt, grabarArbolEnArchivoTxt);
//    grabarArbolEnArchivo(&pa, "archGrabadoTextoIO.txt", recorrerInOrden2, grabarEnteroTxt, grabarArbolEnArchivoTxt);
//    grabarArbolEnArchivo(&pa, "archGrabadoTextoPoO.txt", recorrerPosOrden2, grabarEnteroTxt, grabarArbolEnArchivoTxt);
    recuperarArbolDeArchivo(&paRecuperadoBin, "archGrabadoBinario.bin", sizeof(int), cmpEnteros, mostrarEnteroPorConsola, recuperarArbolDeArchivoBin);
    recuperarArbolDeArchivo(&paRecuperadoTxt, "archGrabadoTextoPrO.txt", sizeof(int), cmpEnteros, mostrarEnteroPorConsola, recuperarArbolDeArchivoTxt);
//    eliminarHoja(&pa, &claveInfo, sizeof(claveInfo), cmpEnteros);
    imprimirArbol(&pa, 1, mostrarEnteroPorConsola);
    puts("\n\nArbol Recuperado Bin: ");
    imprimirArbol(&paRecuperadoBin, 1, mostrarEnteroPorConsola);
    puts("\n\nArbol Recuperado Txt: ");
    imprimirArbol(&paRecuperadoTxt, 1, mostrarEnteroPorConsola);
    altura = alturaArbol(&pa);
    printf("\n\nAltura: %d\n", altura);
    printf("Nodos Sin hijos Izq: %d\n", contarNodosSinHijosIzq(&pa));
    printf("Nodos No Hoja: %d\n", contarNoHojas(&pa));
    printf("Nodos Hoja: %d\n", contarHojas(&pa));
    puts("\n\nPREORDEN: ");
    recorrerPreOrden(&pa, mostrarEnteroPorConsola);
    puts("\nINORDEN: ");
    recorrerInOrden(&pa, mostrarEnteroPorConsola);
    puts("\nPOSTORDEN: ");
    recorrerPosOrden(&pa, mostrarEnteroPorConsola);

    return 0;
}