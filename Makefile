CC = gcc

EFLAGS = -Wall
EFLAGS += -Werror

OFLAGS = -Ofast

SRC_F = src

SRCS = $(SRC_F)/main.c
SRCS += $(SRC_F)/cpu/stack-manip.c
SRCS += $(SRC_F)/cpu/instructions/arith.c
SRCS += $(SRC_F)/cpu/instructions/bra.c
SRCS += $(SRC_F)/cpu/instructions/ctrl.c
SRCS += $(SRC_F)/cpu/instructions/flags.c
SRCS += $(SRC_F)/cpu/instructions/inc.c
SRCS += $(SRC_F)/cpu/instructions/kil.c
SRCS += $(SRC_F)/cpu/instructions/load.c
SRCS += $(SRC_F)/cpu/instructions/logic.c
SRCS += $(SRC_F)/cpu/instructions/nop.c
SRCS += $(SRC_F)/cpu/instructions/shift.c
SRCS += $(SRC_F)/cpu/instructions/stack.c
SRCS += $(SRC_F)/cpu/instructions/trans.c
SRCS += $(SRC_F)/cpu/flags.c
SRCS += $(SRC_F)/cpu/init.c
SRCS += $(SRC_F)/cpu/mem.c
SRCS += $(SRC_F)/cpu/rotate.c
SRCS += $(SRC_F)/cpu/split.c
SRCS += $(SRC_F)/cpu/thread.c
SRCS += $(SRC_F)/cli/menu.c

build:
$(CC) main.c -o 6502 $(EFLAGS) $(OFLAGS)