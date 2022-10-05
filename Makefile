#------------------------------------------------------------------------------
# File:   Makefile
# Author: Miguel Aréjula Aisa y Pablo Angusto Delgado
# Date:   octubre 2022
# Coms:   Makefile para los ejercicios 1 y 2 de la práctica 2 de PSCD
#        
#------------------------------------------------------------------------------

#---------------------------------------------------------
#definición de variables
CC=g++
CPPFLAGS=-I. -g -std=c++11      #opciones de compilación
LDFLAGS=-pthread             #opciones de enlazado (linkado)
RM=rm -f

all: main_p2_e1 main_p2_e2
#-----------------------------------------------------------
# Compilacion
main_p2_e1.o: main_p2_e1.cpp
	${CC} -c main_p2_e1.cpp ${CPPFLAGS}
#linkado
main_p2_e1: main_p2_e1.o
	${CC} main_p2_e1.o -o main_p2_e1 ${LDFLAGS}
#-----------------------------------------------------------
# Compilacion
main_p2_e2.o: main_p2_e2.cpp
	${CC} -c main_p2_e2.cpp ${CPPFLAGS}
#linkado
main_p2_e2: main_p2_e2.o
	${CC} main_p2_e2.o -o main_p2_e2 ${LDFLAGS}
#-----------------------------------------------------------

#-----------------------------------------------------------	
# LIMPIEZA de todo lo generado. OjO: se borrarán sin preguntar
# RM es una de las variables predefinidas en make: invoca borrado de fichero del SO
clean:
	${RM} main_p2_ej1.o main_p2_ej1
	${RM} main_p2_ej2.o main_p2_ej2
	