# Compilateur
CC = g++
CFLAGS = -Wall -std=c++17

# Récupère tous les fichiers .cpp
SRCS := $(wildcard *.cpp)

# Transforme les fichiers .cpp en fichiers .o
OBJS := $(SRCS:.cpp=.o)

# Nom de l’exécutable
TARGET = main

# Règle principale
all: $(TARGET)

# Compilation de l’exécutable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

# Règle générique pour compiler .cpp en .o
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Dépendances sur les headers
main.o: board.h
board.o: board.h
cell.o: cell.h
piece.o: piece.h

# Nettoyage
clean:
	rm -f *.o $(TARGET)
