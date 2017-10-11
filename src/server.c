/** @file server.c
 *  @brief Archivo principal del servidor. Maneja hasta 5 clientes conectados
 *
 * Este es el codigo encargado de correr el servidor.
 * Autentica a los usuarios e intercambia informacion con el cliente.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h> 
#include <unistd.h>

#include "estructuraDeDatos.h"
#include "estructuraPromedios.h"
#include "serverHelpers.h"
#define TAM 10000
#define puerto 6020
#define TAM_FILE 18304 //Tamano de la estructura que guarda las lineas del archivo
#define ESTACIONES 5 //Cantidad de estaciones en el archivo CSV

void printPage(char *metodo,char *valor,struct Datos *datos,struct Promedio *promedio_estaciones);

int main( int argc, char *argv[] ) {

	printf("Content-Type: text/html\n\n");
	//Declaracion de variables
	char *input_browser;
  char metodo[100];
  char *valor;
	char buffer[TAM];
	char promedio[80];
	struct Datos *datos;
	char *estaciones[ESTACIONES]; //Arreglo con las estaciones no repetidas
	struct Promedio promedio_estaciones[ESTACIONES]; //Arreglo para promedio de las estaciones
	
	//Inicializaciones
	datos = csv_parser();  //Lleno con los datos del archivo
  listar_estaciones(datos, promedio_estaciones,TAM_FILE,ESTACIONES);
	input_browser = getenv("QUERY_STRING");
  strcpy(metodo,input_browser);
  valor = strstr(input_browser,"=")+1;
  strtok(metodo,"=");
  printPage(metodo,valor,datos,promedio_estaciones);

	return 0; 
} 

void printPage(char *metodo,char *valor,struct Datos *datos,struct Promedio *promedio_estaciones){
    float precipitacion_mensual[12]; //Uno para cada mes del ano
    float precipitacion_diaria[365]; //Uno para cada dia del ano
    int numero_estacion = atoi(valor);
    //Imports
    printf(""
       "<!DOCTYPE html>"
            "<html>"
               "<head>"
                "<title>Mi estacion</title>"
               " <link rel=\"stylesheet\""
                "href=\"http://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/css/bootstrap.min.css\">"
                "<script src\"http://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/js/bootstrap.min.js\"></script>"
                "</head>");

    //<!-- Navbar -->
    printf(""  
   " <body>"
        "<nav class=\"navbar navbar-inverse\">"
         " <div class=\"container-fluid\">"
           " <!--Barra colapsada -->"
            "<div class=\"navbar-header\">"
              "<button type=\"button\" class=\"navbar-toggle\" data-toggle=\"collapse\" data-target=\"#myNavbar\">"
                "<span class=\"icon-bar\"></span>"
               " <span class=\"icon-bar\"></span>"
                "<span class=\"icon-bar\"></span>"
             " </button>"
             " <a class=\"navbar-brand\" href=\"../index.html\">Estacion Meteorologica</a>"
           " </div>"
            "<!--Barra no colapsada -->"
            "<div class=\"collapse navbar-collapse\" id=\"myNavbar\">"
              "<ul class=\"nav navbar-nav\">"
               " <li><a href=\"ksamp.cgi\">Informacion del sistema</a></li>"
                "<li><a href=\"../consultas.html\">Consultas Actualizadas</a></li>"
                "<li><a href=\"modulos.cgi\">Modulos del sistema</a></li>"
              "</ul>"
           " </div>"
          "</div>"
        "</nav>");

/*     printf("<h1>%s</h1>",metodo );
     printf("<h1>%s</h1>",valor );*/

     if(strcmp(metodo,"descargar") == 0){
        float temperatura = -9999;
        float humedad = -9999;
        char *nombre_estacion;
        mensual_precipitacion(numero_estacion,precipitacion_mensual,datos,TAM_FILE);
        calcular_promedio("temperatura",promedio_estaciones,datos,TAM_FILE,ESTACIONES);
        for (int i = 0; i < ESTACIONES; ++i)
        {
          if (promedio_estaciones[i].numero == numero_estacion){
              temperatura = promedio_estaciones[i].variable;

          }
        }
        calcular_promedio("humedad",promedio_estaciones,datos,TAM_FILE,ESTACIONES);
        for (int i = 0; i < ESTACIONES; ++i)
        {
          if (promedio_estaciones[i].numero == numero_estacion){
              humedad = promedio_estaciones[i].variable;
              nombre_estacion = promedio_estaciones[i].nombre_estacion;
          }
        }
        escribir_archivo(numero_estacion,precipitacion_mensual,temperatura,humedad,nombre_estacion);
        printf("Datos de la estacion %i </br>",numero_estacion);
        printf("<a download href=\"../text_files/datos_estacion.txt\">Descargar archivo</a>");
     }
     else if(strcmp(metodo,"diario_precipitacion") == 0){
        diario_precipitacion(numero_estacion,precipitacion_diaria,datos,TAM_FILE);
        for (int i = 0; i < 365; ++i)
        {
          printf("Dia %i %f <br>",i,precipitacion_diaria[i]);
        }
     }
     else if(strcmp(metodo,"mensual_precipitacion") == 0){
        mensual_precipitacion(numero_estacion,precipitacion_mensual,datos,TAM_FILE);
        char *meses[] = {"Enero","Febrero","Marzo","Abril","Mayo","Junio","Julio","Agosto","Septiembre","octubre","Noviembre","Diciembre"};
        for (int i = 0; i < 12; ++i)
        {
          printf("<h4>%s: %f</h4> <br>",meses[i],precipitacion_mensual[i]);
        }
     }
     else if(strcmp(metodo,"promedio_variable") == 0){
        calcular_promedio(valor,promedio_estaciones,datos,TAM_FILE,ESTACIONES);
        for (int i = 0; i < ESTACIONES; ++i)
        {
          printf("Estacion %s: %f <br>",promedio_estaciones[i].nombre_estacion,promedio_estaciones[i].variable);        
        }
      }
     else{
        printf("Metodo no permitido");
     }
     printf(""
        "</body>"
    "</html>"
 	  );
   }
