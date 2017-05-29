/** @file ksamp.c
 *  @brief Informacion del sistema
 * Se muestra informacion relevante al sistema de manera abreviada y detallada
 * Procesador
 * Memoria RAM
 * Uptime
 * Fecha y hora del sistema
 *
 */
#include "include/ksamp.h"


int main (int argc, char* argv[]){
    printf("Content-Type: text/html\n\n");
    struct Datos datos;
    struct Interval interval;
    
    getData(&datos);
    calculateInterval(&interval);
    printPage(datos, interval);
	return 0;
}

/**
* @brief Imprime el código HTML con la información solicitada.
*.
*
* @param datos Tiene informacion sobre CPU,uptime y hora
* @param data Tiene informacion sobre la RAM
*/
void printPage(struct Datos datos, struct Interval data){
    long uptime = datos.uptime;
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
               " <li class=\"active\"><a href=\"ksamp.cgi\">Informacion del sistema</a></li>"
                "<li><a href=\"../consultas.html\">Consultas</a></li>"
                "<li><a href=\"modulos.cgi\">Modulos del sistema</a></li>"
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
              "<th>Categoria</th>"
              "<th>Resumen</th>"
              "<th>Datos completos</th>"
            "</tr>"
          "</thead>"
          "<tbody>"
            "<tr>"
              "<td>Procesador</td>"
              "<td>%s</td>"
              "<td></td>"
            "</tr>"
            "<tr>"
              "<td>Memoria</td>"
              "<td>%i MB</td>"
              "<td>Total: %i MB<br> Disponible: %i MB</td>"
            "</tr>"
            "<tr>"
              "<td>Uptime</td>"
              "<td>%ld</td>"
              "<td>%ld Dias %ld Horas %ld Minutos %ld segundos</td>"
            "</tr>"
            "<tr>"
              "<td>Fecha y Hora</td>"
              "<td>%d-%d-%d <br> %d:%d:%d</td>"
              "<td></td>"
            "</tr>"
          "</tbody>"
        "</table>"
      "</div>"
    "</div>" ,
    datos.cpu,
    data.memTot/MBYTE,
    data.memTot/MBYTE,
    data.memDisp/MBYTE,
    datos.uptime,
    uptime/day, (uptime%day)/hour,(uptime%hour)/minute,uptime%minute,
    datos.time.tm_mday, datos.time.tm_mon + 1,datos.time.tm_year + 1900,datos.time.tm_hour,datos.time.tm_min,datos.time.tm_sec
    );

     printf(""
            "</body>"
        "</html>"
     );
}

/**
* @brief Mediante el directorio proc llena los datos necesarios.
*
*
* @param *datos Puntero a estructura para modificarla directamente con los datos obtenidos.
*/
void getData(struct Datos *datos){

    char buffer[256];

    fp = fopen("/proc/sys/kernel/hostname","r");
    if (fp == NULL){
        return;
    } 
    if(fgets(buffer, 256, fp) == NULL){
        printf("Error fgets\n");
    }
    fclose(fp);
    strcpy(datos->hostname,buffer);

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    datos->time = tm;

    strcpy(path,"/proc/cpuinfo");
    strcpy(before,"model name");
    strcpy(after,"");
    parseFile(fp,path,before,after,&buff);
    strcpy(datos->cpu,buff+2);

    strcpy(path,"/proc/version");
    strcpy(before,"version ");
    strcpy(after,"(");
    parseFile(fp,path,before,after,&buff);
    strcpy(datos->kernel,buff);

    strcpy(path,"/proc/uptime");
    strcpy(before,"");
    strcpy(after," ");
    parseFile(fp,path,before,after,&buff);
    datos->uptime = atol(buff);
}

/**
* @brief Llena la estructura interval la cual contiene informacion sobre la RAM.
*
*
* @param *interval Puntero a estructura para modificarla directamente con los datos obtenidos.
*/
void calculateInterval(struct Interval *interval){

    strcpy(path,"/proc/meminfo");
    strcpy(before,"MemTotal:");
    strcpy(after,"");
    parseFile(fp,path,before,after,&buff);
    
    interval->memTot = atoi(buff);

    strcpy(before,"MemAvailable:");
    parseFile(fp,path,before,after,&buff);
    
    interval->memDisp = atoi(buff);

    strcpy(path,"/proc/loadavg");
    strcpy(before,"");
    strcpy(after,"");
    parseFile(fp,path,before,after,&buff);
    if(sscanf( buff, "%s %s %s", interval->load1Min, interval->load5Min, interval->load15Min) != 3){
        printf("Error sscanf\n");
        exit(EXIT_FAILURE);
    }
}

    /**
    * @brief Parsea un archivo.
    *
    */
int parseFile(FILE* fp, char path[], char before[], char after[], char** buff){

    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    
    fp = fopen(path,"r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    while ((read = getline(&line, &len, fp)) != -1) {
        if(strstr(line,before)!=NULL){
            *buff = strstr(line,before) + strlen(before);
            *buff = strtok(*buff,after);
            break;
        }
    }
    fclose(fp);
    return 0;
}