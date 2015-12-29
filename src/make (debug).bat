@echo off
gcc main.c main.h defs.h gui.c gui.h bots.c neural_network.c pellets.c render.c simulation.c world.c -lopengl32 -mwindows -g -gdwarf-2 -Wall -Wextra -o "main (debug).exe"
PAUSE