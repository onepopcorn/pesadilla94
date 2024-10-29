SRC_DIR := ./src
BIN_DIR := ./bin
CC := gcc
CCFLAGS := -Wall

SRCS := $(wildcard ./src/*.c)
BINS := $(patsubst ./src/%.c, ./bin/%.exe, $(SRCS))

all: $(BINS)

$(BIN_DIR)/%.exe: $(wildcard $(SRC_DIR)/*.c)
	$(CC) $(CCFLAGS) -o $@ $<


$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -f $(BIN_DIR)/*

