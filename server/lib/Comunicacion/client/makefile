#Compilación del cliente en c++:
GXX = g++
FLAGS = -O3
DIR = ./src
DIR_BIN = ./Bin/client
DEPENDENCIAS = ${DIR}/Client.cpp \
			   ${DIR}/main.cpp
				
run: compilar
	@./bin/client

compilar:
	${GXX} ${FLAGS} -o ${DIR_BIN} ${DEPENDENCIAS} -lwsock32 -municode
	