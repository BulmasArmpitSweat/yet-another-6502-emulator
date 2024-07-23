CC = gcc

EFLAGS = -Wall
EFLAGS += -Werror

OFLAGS = -Ofast

build:
$(CC) main.c -o 6502 $(EFLAGS) $(OFLAGS)