#include <stdlib.h>
#include <stdio.h>
#include "notifier.h"

void send_notification(float temp) {
    char command[128];
    snprintf(command, sizeof(command),
             "notify-send '⚠️ CPU ALERT' 'Temp: %.1f°C exceeds safe limit!'", temp);
    system(command);
}
