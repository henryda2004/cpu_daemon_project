#!/bin/bash

if pgrep -x "cpu_daemon" > /dev/null
then
    echo "🔴 Deteniendo daemon..."
    pkill -x "cpu_daemon"
    echo "✅ Daemon detenido."
else
    echo "🟡 El daemon no está corriendo."
fi
