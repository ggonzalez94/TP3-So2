#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/modulos.h"

#define SIZE_BUFFER 200

/** @file list_modules.c
 *  @brief Muestra los modulos del sistema
 * Programa para listar los modulos del sistema ademas de la
 * informacion relevante(Tamano y estado)
 */

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
	printPage(cantidad_modulos,modulos_instalados);
	return 0;
}


/**
* @brief Devuelve la cantidad de modulos instalados en el sistema
*.
*
* @param fp File pointer al archivo /proc/modules
*/
int count_modules(FILE *fp){
	int cantidad_modulos = 0;
	char buffer[SIZE_BUFFER];

	while(fgets(buffer,SIZE_BUFFER,fp) != NULL){
		cantidad_modulos++;
	}

	return cantidad_modulos;
}

/**
* @brief Devuelve en el array modulos informacion sobre los modulos instalados
*.
*
* @param fp File pointer al archivo /proc/modules
* @param cantidad_modulos Cantidad de modulos actual del sistema
* @param modulos Arreglo a llenar de estructuras del tipo modulo
*/

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

/**
* @brief Imprime el código HTML con la información solicitada.
*.
*
* @param cantidad_modulos Cantidad de modulos actual del sistema
* @param modulos_instalados Arreglo de estructuras con la informacion de los modulos a imprimir
*/
void printPage(int cantidad_modulos,struct Modulo modulos_instalados[cantidad_modulos]){
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
                "<li><a href=\"../consultas.html\">Consultas</a></li>"
                "<li class=\"active\"><a href=\"modulos.cgi\">Modulos del sistema</a></li>"
              "</ul>"
           " </div>"
          "</div>"
        "</nav>");
        // //Lista
     printf(""
    "<div class=\"container\">" 
    "<div class=\"table-responsive\"> "         
        "<table class=\"table table-striped\">"
          "<thead>"
            "<tr>"
              "<th>Nombre</th>"
              "<th>Tamano</th>"
              "<th>estado</th>"
            "</tr>"
          "</thead>"
          "<tbody>"
    );

     for (int i = 0; i < cantidad_modulos; ++i)
     {
     	printf(""
            "<tr>"
              "<td>%s</td>"
              "<td>%i KBytes</td>"
              "<td>%s</td>"
            "</tr>",
            modulos_instalados[i].nombre_modulo,
            modulos_instalados[i].tamano/KBYTE,
            modulos_instalados[i].estado
     	);
     }
     printf(""
     		"</tbody>"
     		"</table>"
     		"</div>"
     		"</div>"
      );

     printf(""
        "</body>"
    "</html>"
 	  );
   }