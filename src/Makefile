
SRC_DIR  := ./
BIN_DIR  := ../out
OBJ_DIR  := ../obj

BIN := ../out/game.exe


CC := c:/djgpp/bin/i586-pc-msdosdjgpp-gcc.exe
DC := objdump
CFLAGS	:= -I. -c -Wall -Werror -pedantic -O3 -fomit-frame-pointer -ffast-math -march=i386
LDFLAGS := -s

SRCS	:= $(shell find $(SRC_DIR) -name '*.c')
OBJS	:= $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRCS))

all: $(BIN)
	cp ../vendor/CWSDPMI.exe ../out/CWSDPMI.exe

clean: 
	rm -rf $(BIN_DIR)/* $(OBJ_DIR)/*

$(BIN): $(OBJS)
	@mkdir -p $(@D)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

$(OBJ_DIR)/%.o:%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $< -o $@
	$(CC) $(CFLAGS) -S $< -o $(OBJ_DIR)/$*.lst

.PHONY: all clean