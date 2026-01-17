CC = g++
CFLAGS = -Wall -O3 --std=c++11

TARGET = MoRTo.exe

RANGEN_DIR = rangen
RANGEN_EXE = $(RANGEN_DIR)/main.exe
RANGEN_OBJ = $(RANGEN_DIR)/random.o

OBJS = main.o deck.o player.o table.o printing.o game.o theory.o config_reader.o $(RANGEN_OBJ)

.PHONY: all clean rangen

all: $(RANGEN_EXE) $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

# --- build della cartella interna (produce main.exe, random.o, main.o dentro rangen/) ---
$(RANGEN_EXE): $(RANGEN_DIR)/Makefile $(RANGEN_DIR)/main.cpp $(RANGEN_DIR)/random.cpp $(RANGEN_DIR)/random.h
	$(MAKE) -C $(RANGEN_DIR)

# --- oggetto usato dal progetto esterno: lo compilo tramite il make interno ---
$(RANGEN_OBJ): $(RANGEN_DIR)/random.cpp $(RANGEN_DIR)/random.h $(RANGEN_DIR)/Makefile
	$(MAKE) -C $(RANGEN_DIR) random.o

main.o: main.cpp config_reader.h card.h deck.h stats.h counting.h printing.h game.h theory.h table.h player.h rangen/random.h
	$(CC) -c main.cpp -o main.o $(CFLAGS)

config_reader.o: config_reader.cpp config_reader.h
	$(CC) -c config_reader.cpp -o config_reader.o $(CFLAGS)

deck.o: deck.cpp deck.h card.h rangen/random.h
	$(CC) -c deck.cpp -o deck.o $(CFLAGS)

player.o: player.cpp player.h card.h deck.h counting.h rangen/random.h
	$(CC) -c player.cpp -o player.o $(CFLAGS)

table.o: table.cpp table.h player.h
	$(CC) -c table.cpp -o table.o $(CFLAGS)

printing.o: printing.cpp printing.h stats.h table.h
	$(CC) -c printing.cpp -o printing.o $(CFLAGS)

game.o: game.cpp game.h table.h deck.h counting.h
	$(CC) -c game.cpp -o game.o $(CFLAGS)

theory.o: theory.cpp theory.h stats.h deck.h
	$(CC) -c theory.cpp -o theory.o $(CFLAGS)

clean:
	rm -f *.o $(TARGET)
	$(MAKE) -C $(RANGEN_DIR) clean
