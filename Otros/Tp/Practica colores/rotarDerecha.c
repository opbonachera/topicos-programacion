#include <stdlib.h>
#include <stdio.h>

typedef struct {
    unsigned int tamArchivo;
    unsigned int tamEncabezado;
    unsigned int comienzoImagen;
    unsigned int ancho;
    unsigned int alto;
    unsigned short profundidad;
} t_metadata;

typedef struct {
    unsigned char pixel[3];
} t_pixel;

int leerCabecera(char [], t_metadata *);
int escribirCabecera(FILE *archivoNuevo);
int escribirDatos(FILE *archivoNuevo);
int modificarDimensiones(FILE *archivoNuevo, unsigned int ww, unsigned int hh);
int crearImagen();

int main()
{
    crearImagen();

    return 0;
}

int crearImagen()
{
    t_metadata cabeceraNuevo, cabeceraOriginal;
    t_pixel px;

    FILE *archivoNuevo = fopen("nueva.bmp", "wb");

    if (archivoNuevo == NULL) {
        perror("Error opening file");
        return 1;
    }

    leerCabecera("kitty.bmp", &cabeceraOriginal);

    escribirCabecera(archivoNuevo);

    modificarDimensiones(archivoNuevo, cabeceraOriginal.alto, cabeceraOriginal.ancho);

    escribirDatos(archivoNuevo);

    leerCabecera("nueva.bmp", &cabeceraNuevo);

    fclose(archivoNuevo); // Close the file at the end

    return 0;
}


int escribirCabecera(FILE *archivoNuevo)
{
    // Copia TODOS los datos de la cabecera del archivo original al nuevo
    FILE *archivoOriginal;

    archivoOriginal = fopen("kitty.bmp","rb");

    if (archivoOriginal == NULL || archivoNuevo == NULL) {
        perror("Error opening file");
        return 1;
    }

    unsigned char byte[3];

    for (int i=0; i<54;i++){
        fread(&byte, sizeof(unsigned char), 1, archivoOriginal);
        fwrite(&byte, sizeof(unsigned char), 1, archivoNuevo);
    }

    fclose(archivoOriginal);

    return 0;
}

int escribirDatos(FILE *archivoNuevo)
{
    // Escribe los pixeles que componen la imagen
    t_pixel px;

    FILE *archivoOriginal;

    archivoOriginal = fopen("kitty.bmp","rb");

    if (archivoOriginal == NULL || archivoNuevo == NULL) {
        perror("Error opening file");
        return 1;
    }

    fseek(archivoOriginal, 54, SEEK_SET);
    fseek(archivoNuevo, 54, SEEK_SET);

    unsigned int alto = 300;
    unsigned int ancho = 270;

    t_pixel imagen[300 * 270];
    t_pixel copia[270 * 300];

    for(int i=0; i<300*270; i++)
    {
        fread(&imagen[i], sizeof(unsigned char), 3, archivoOriginal);
    }

    for(int y=0; y = alto; y++)
    {
        for(int x=0; x<ancho; x++)
        {
            copia[y * ancho + x] = imagen[x * alto + (alto - y - 1)];
        }
    }

    for(int i=0; i<300*270; i++)
    {
        fwrite(&copia[i], sizeof(unsigned char), 3, archivoNuevo);
    }

    fclose(archivoOriginal);
    return 0;
}

int modificarDimensiones(FILE *file, unsigned int w, unsigned int h)
{
    fseek(file, 18, SEEK_SET);
    fwrite(&w, sizeof(unsigned int), 1, file);

    fseek(file, 22, SEEK_SET);
    fwrite(&h, sizeof(unsigned int), 1, file);

    return 0;
}

int leerCabecera(char file[], t_metadata *cabecera)
{
    FILE *img;

    img = fopen(file,"rb");

    printf("\n\n--------");
    printf("Filename: %s\n",file);

    fseek(img, 2, SEEK_SET);
    fread(&cabecera->tamArchivo, sizeof(unsigned int), 1, img);

    fseek(img, 14, SEEK_SET);
    fread(&cabecera->tamEncabezado, sizeof(unsigned int), 1, img);

    fseek(img, 10, SEEK_SET);
    fread(&cabecera->comienzoImagen, sizeof(unsigned int), 1, img);

    fseek(img, 18, SEEK_SET);
    fread(&cabecera->ancho, sizeof(unsigned int), 1, img);

    fseek(img, 22, SEEK_SET);
    fread(&cabecera->alto, sizeof(unsigned int), 1, img);

    fseek(img, 28, SEEK_SET);
    fread(&cabecera->profundidad, sizeof(unsigned short), 1, img);

    printf("Tama�o de archivo: %u bytes\n", cabecera->tamArchivo);
    printf("Tama�o de cabecera: %u bytes\n", cabecera->tamEncabezado);
    printf("Alto: %u bytes\n", cabecera->alto);
    printf("Ancho: %u bytes\n", cabecera->ancho);
    printf("Comienzo de imagen: byte %u\n", cabecera->comienzoImagen);
    printf("Profundidad: %u bits\n", cabecera->profundidad);

    fclose(img);
    return 0;
}
