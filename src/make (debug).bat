@echo off
gcc main.c gui.c bots.c neural_network.c pellets.c render.c simulation.c world.c -lopengl32 -mwindows -g -gdwarf-2 -Wall -Wextra -o "main (debug).exe"
PAUSE