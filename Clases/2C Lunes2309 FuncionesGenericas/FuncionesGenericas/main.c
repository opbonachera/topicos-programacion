#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "genericas.h"

int cmpInt(const void* e1, const void* e2);

int main()
{
    Vector v;
    vectorCrear(&v, sizeof(int));

    // for(int i = 1; i <= 16; i++)
    // {
    //     vectorOrdInsertar(&v, i);
    // }

    // vectorMostrar(&v);

    // for(int i = 16; i > 0; i--)
    // {
    //     vectorOrdEliminarElem(&v, &i);
    // }

    cargarVectorRandom(&v, 100000);

    // puts("Antes de ordenar:");
    // vectorMostrar(&v);

    time_t iniT = time(NULL);
    vectorOrdenar(&v, cmpInt, SELECCION);
    time_t finT = time(NULL);

    // puts("\nDespues de ordenar:");
    // vectorMostrar(&v);

    printf("Tiempo de ejecucion: %ld\n", finT - iniT);

    vectorEliminar(&v);

    return 0;
}

int cmpInt(const void* e1, const void* e2)
{
    int* int1 = e1;
    int* int2 = e2;

    return int1 - int2;
    // r=0 si int1=int2
    // r<0 si int1<int1
    // r>0 si int1>int2
}
