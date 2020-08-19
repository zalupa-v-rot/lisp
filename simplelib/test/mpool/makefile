#CC=gcc
NAME?=test
DIR?=./tmp
CFLAGS?=-g
CDEFINES=
LDLIBS=
OBJECTS=./lib/mpool.o ./lib/ofchk.o ./lib/assert.o ./tests.o ./main.o


build: dir-struct $(DIR)/$(NAME)
	$(DIR)/$(NAME)

dir-struct: _FORCE
	mkdir -p $(DIR)/lib

$(DIR)/$(NAME): $(OBJECTS)
	cd $(DIR); $(CC) $(CFLAGS) $(OBJECTS) $(LDLIBS) -o ./$(NAME)

%.o: ./%.c
	$(CC) $(CFLAGS) $(CDEFINES) -c -o $(DIR)/$@ $<

_FORCE:
