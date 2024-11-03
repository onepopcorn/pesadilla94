SRC_DIR := ./src
BIN_DIR := ./bin
CC := gcc
CCFLAGS := -Wall

SRCS := $(wildcard ./src/*.c)
BINS := $(patsubst ./src/%.c, ./bin/%.exe, $(SRCS))

all: $(BINS)

$(BINS):
	$(CC) $(CCFLAGS) ./src/bmp2data.c -o $(BIN_DIR)/bmp2data.exe
	$(CC) $(CCFLAGS) ./src/tiled2map.c ../vendor/c-simple-json-parser/json.c -DJSON_SKIP_WHITESPACE -o $(BIN_DIR)/tiled2map.exe

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -f $(BIN_DIR)/*

