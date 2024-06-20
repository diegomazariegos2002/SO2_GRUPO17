#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_BUFFER_SIZE 1024

int get_month_number(const char *month_name) {
    const char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    for (int i = 0; i < 12; ++i) {
        if (strcmp(month_name, months[i]) == 0) {
            return i;
        }
    }
    return -1; // Mes no encontrado
}

int main() {
    // Abrir pipe para leer la salida del script de SystemTap
    FILE *pipe = popen("stap -v memory_monitor.stp", "r");
    if (!pipe) {
        perror("Error al abrir la tubería");
        return 1;
    }

    char buffer[MAX_BUFFER_SIZE];
    while (fgets(buffer, MAX_BUFFER_SIZE, pipe) != NULL) {
        char *pid, *execname, *length, *name, *timestamp;
        pid = strtok(buffer, ",");
        execname = strtok(NULL, ",");
        length = strtok(NULL, ",");
        name = strtok(NULL, ",");
        timestamp = strtok(NULL, ",");

        char month[4];

        struct tm tm_time;
        memset(&tm_time, 0, sizeof(struct tm));
        sscanf(timestamp, "%*s %s %d %d:%d:%d %d", month, &tm_time.tm_mday, &tm_time.tm_hour, &tm_time.tm_min, &tm_time.tm_sec, &tm_time.tm_year);

        tm_time.tm_year -= 1900;
        tm_time.tm_mon = get_month_number(month);

        char formatted_timestamp[50];
        strftime(formatted_timestamp, sizeof(formatted_timestamp), "%Y-%m-%d %H:%M:%S", &tm_time);
        
        printf("PID: %s, ExecName: %s, Length: %s, Name: %s, Timestamp: %s\n", pid, execname, length, name, formatted_timestamp);
    }

    pclose(pipe);

    return 0;
}
