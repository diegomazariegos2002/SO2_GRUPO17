#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include <time.h>
#include <sys/sysinfo.h>

#define MAX_BUFFER_SIZE 1024

int get_month_number(char *month);
unsigned long get_total_system_memory();

int get_month_number(char *month) {
    if (strcmp(month, "Jan") == 0) return 1;
    if (strcmp(month, "Feb") == 0) return 2;
    if (strcmp(month, "Mar") == 0) return 3;
    if (strcmp(month, "Apr") == 0) return 4;
    if (strcmp(month, "May") == 0) return 5;
    if (strcmp(month, "Jun") == 0) return 6;
    if (strcmp(month, "Jul") == 0) return 7;
    if (strcmp(month, "Aug") == 0) return 8;
    if (strcmp(month, "Sep") == 0) return 9;
    if (strcmp(month, "Oct") == 0) return 10;
    if (strcmp(month, "Nov") == 0) return 11;
    if (strcmp(month, "Dec") == 0) return 12;
    return -1;
}

unsigned long get_total_system_memory() {
    struct sysinfo info;
    sysinfo(&info);
    return info.totalram / 1024 / 1024; // Convert to MB
}

int main() {
    // MySQL connection variables
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    
    char* server = "localhost";
    char* user = "root";
    char* password = "";
    char* database = "monitor";

    // Initialize MySQL connection
    conn = mysql_init(NULL);
    if (conn == NULL) {
        fprintf(stderr, "mysql_init() failed\n");
        exit(EXIT_FAILURE);
    }

    if (mysql_real_connect(conn, server, user, password, database, 0, NULL, 0) == NULL) {
        fprintf(stderr, "mysql_real_connect() failed\n");
        mysql_close(conn);
        exit(EXIT_FAILURE);
    }

    unsigned long total_memory = get_total_system_memory();
    printf("Total System Memory: %lu MB\n", total_memory);

    FILE *pipe = popen("sudo stap -v ./memory_monitor.stp", "r");
    if (!pipe) {
        perror("Error al abrir la tubería");
        return 1;
    }

    char buffer[MAX_BUFFER_SIZE];
    while (fgets(buffer, MAX_BUFFER_SIZE, pipe) != NULL) {
        char *pid_str, *execname, *size_str, *name, *timestamp;
        unsigned long size;
        pid_str = strtok(buffer, ",");
        execname = strtok(NULL, ",");
        size_str = strtok(NULL, ",");
        name = strtok(NULL, ",");
        timestamp = strtok(NULL, ",");

        // Convertir size a unsigned long
        size = strtoul(size_str, NULL, 10);

        // Convertir size de bytes a megabytes
        double size_mb = (double)size / 1024 / 1024;

        // Parsear el timestamp
        char month[4];
        struct tm tm_time;
        memset(&tm_time, 0, sizeof(struct tm));
        sscanf(timestamp, "%*s %s %d %d:%d:%d %d", month, &tm_time.tm_mday, &tm_time.tm_hour, &tm_time.tm_min, &tm_time.tm_sec, &tm_time.tm_year);
        tm_time.tm_year -= 1900;
        tm_time.tm_mon = get_month_number(month) - 1;
        char formatted_timestamp[50];
        strftime(formatted_timestamp, sizeof(formatted_timestamp), "%Y-%m-%d %H:%M:%S", &tm_time);

        // Insert data into LLAMADA table
        char query[512];
        //fprintf(stdout, "INSERT INTO LLAMADA (pid, nombre_proceso, llamada, size, fecha) VALUES (%s, '%s', '%s', %.2f, '%s');", pid_str, execname, name, size_mb, formatted_timestamp);
        snprintf(query, sizeof(query), "INSERT INTO LLAMADA (pid, nombre_proceso, llamada, size, fecha) VALUES (%s, '%s', '%s', %.2f, '%s');", pid_str, execname, name, size_mb, formatted_timestamp);
        if (mysql_query(conn, query)) {
            fprintf(stderr, "INSERT error: %s\n", mysql_error(conn));
        }

        // Insert data into PROCESO table
        double percentage = (size_mb / total_memory) * 100; // Asumiendo que 'size_mb' aquí es el cambio en memoria total usado
        snprintf(query, sizeof(query), "INSERT INTO PROCESO (pid, nombre, size, porcentaje) VALUES (%s, '%s', %.2f, %.2f);", pid_str, execname, size_mb, percentage);
        if (mysql_query(conn, query)) {
            fprintf(stderr, "INSERT error: %s\n", mysql_error(conn));
        }
    }

    pclose(pipe);
    mysql_close(conn);

    return 0;
}
