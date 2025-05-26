cmake_minimum_required(VERSION 3.10)
project(cpu_daemon C)

set(CMAKE_C_STANDARD 99)

add_executable(cpu_daemon
        main.c
        daemon.c daemon.h
        temp_monitor.c temp_monitor.h
        notifier.c notifier.h
)

add_executable(cpu_stressor
        cpu_stressor.c
)

