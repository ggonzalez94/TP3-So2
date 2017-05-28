#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/modulos.h"

#define SIZE_BUFFER 200

int count_modules(FILE *fp);
void list_modules(FILE *fp, int cantidad_modulos, struct Modulo modulos[cantidad_modulos]);

int main(void){
	FILE *archivo_modulos;
	int cantidad_modulos;
	archivo_modulos = fopen("/proc/modules","r");
	if (archivo_modulos == NULL){
		printf("Error abriendo archivo");
		exit(1);
	}
	cantidad_modulos = count_modules(archivo_modulos);
	struct Modulo modulos_instalados[cantidad_modulos];
	list_modules(archivo_modulos,cantidad_modulos,modulos_instalados);
	for (int i = 0; i < cantidad_modulos; ++i)
	{
		printf("%s\n", modulos_instalados[i].nombre_modulo);
	}
	return 0;
}

int count_modules(FILE *fp){
	int cantidad_modulos = 0;
	char buffer[SIZE_BUFFER];

	while(fgets(buffer,SIZE_BUFFER,fp) != NULL){
		cantidad_modulos++;
	}

	return cantidad_modulos;
}

void list_modules(FILE *fp, int cantidad_modulos, struct Modulo modulos[cantidad_modulos]){
	char buffer[SIZE_BUFFER];
	//Voy al inicio del archivo
	fseek(fp,0,SEEK_SET);
	for (int i = 0; i < cantidad_modulos; ++i)
    {
        fgets(buffer, SIZE_BUFFER, fp);
        //printf("%s\n", buffer);
        sscanf( buffer, "%s %d %d %s %s %f", modulos[i].nombre_modulo, &modulos[i].tamano, &modulos[i].veces_cargado, modulos[i].dependencias, modulos[i].estado, &modulos[i].memory_offset);
	}
	return;
}