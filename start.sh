#!/bin/bash

DAEMON="./cmake-build-debug/cpu_daemon"

if pgrep -x "cpu_daemon" > /dev/null
then
    echo "🟡 El daemon ya está corriendo."
else
    echo "🟢 Iniciando daemon..."
    $DAEMON &
    echo "✅ Daemon iniciado."
fi
