
SRC_DIR  := ./
BIN_DIR  := ../out
OBJ_DIR  := ../obj
BIN := ../out/game.exe


CC := c:/djgpp/bin/i586-pc-msdosdjgpp-gcc.exe
DC := objdump
CFLAGS	:= -I. -IC:\djgpp\include -c -std=gnu99 -Wall -Werror -pedantic -O3 -fomit-frame-pointer -ffast-math -march=i386
LDFLAGS := -LC:\djgpp\lib -s
LIBS := -lmikmod

SRCS	:= $(shell find $(SRC_DIR) -name '*.c')
OBJS	:= $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRCS))

DEBUG ?=
ifeq ($(DEBUG),collisions)
    CFLAGS += -DDEBUG_COLLISIONS -DDEBUG
else ifeq ($(DEBUG),tiles)
    CFLAGS += -DDEBUG_TILES -DDEBUG
else ifeq ($(DEBUG), entities)
	CFLAGS += -DDEBUG_ENTITIES -DDEBUG
else ifeq ($(DEBUG),all)
    CFLAGS += -DDEBUG_COLLISIONS -DDEBUG_TILES -DDEBUG
endif


all: $(BIN)
	cp ../vendor/CWSDPMI.exe ../out/CWSDPMI.exe

clean: 
	rm -rf $(BIN_DIR)/* $(OBJ_DIR)/*

$(BIN): $(OBJS)
	@mkdir -p $(@D)
	$(CC) $(LDFLAGS) $(OBJS) -o $@ $(LIBS)

$(OBJ_DIR)/%.o:%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $< -o $@
	$(CC) $(CFLAGS) -S $< -o $(OBJ_DIR)/$*.lst

.PHONY: all clean