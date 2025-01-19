SRC = main.c \
	  task_a.c \
	  task_b.c \
	  task_c.c \
	  task_i2c.c \
	  driver_i2c.c \
	  queue_lib.c \
	  timing.c \

#take obj from .c files
OBJ = $(SRC:.c=.o)
CC		= gcc
CFLAGS	= -Wall -g
LDFLAGS = -lm

BIN = q4.out	

all:
	$(CC) -o $(BIN) $(SRC) $(LDFLAGS) $(CFLAGS)

clean:
	rm  -f *.out $(OBJ)

