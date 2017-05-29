/**
 * @file modulos.h
 * @brief Header con declaracion de variables y prototipos de funciones.
 *
 * Este archivo contiene la estructura modulo con los campos necesarios para llenar la
 * informacion sobre los modulos y el prototipo de las funciones para contar,listar e imprimir
 * informacion sobre los modulos.
 */


#include <stdio.h>
#include <stdlib.h>

#define KBYTE 1024

struct Modulo{
	char nombre_modulo[50];
	int tamano;
	int veces_cargado;
	char dependencias[20];
	char estado[50];
	float memory_offset;
};

int count_modules(FILE *fp);
void list_modules(FILE *fp, int cantidad_modulos, struct Modulo modulos[cantidad_modulos]);
void printPage(int cantidad_modulos,struct Modulo modulos_instalados[cantidad_modulos]);