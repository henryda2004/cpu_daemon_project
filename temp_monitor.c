#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "temp_monitor.h"

float get_cpu_temp() {
    FILE *fp = popen("sensors", "r");
    if (!fp) return -1;

    char line[256];
    float temp = 0.0;

    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, "Tctl:")) {
            // Ahora sí: reconoce el signo + y la unidad °C
            if (sscanf(line, "Tctl: +%f°C", &temp) == 1) {
                break;
            }
        }
    }

    pclose(fp);
    return temp;
}
