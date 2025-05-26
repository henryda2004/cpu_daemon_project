#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "daemon.h"
#include "temp_monitor.h"
#include "notifier.h"

#define INTERVAL 5
#define TEMP_THRESHOLD 65.0

int main() {
    create_daemon();

    FILE *log = fopen("/home/henry/CLionProjects/cpu_daemon/logs/cpu_temp_log.txt", "a");
    if (!log) return 1;

    while (1) {
        float temp = get_cpu_temp();
        time_t now = time(NULL);

        fprintf(log, "[%s] Temp: %.2f°C\n", ctime(&now), temp);
        fflush(log);

        if (temp >= TEMP_THRESHOLD) {
            send_notification(temp);
        }

        sleep(INTERVAL);
    }

    fclose(log);
    return 0;
}
