CC?=gcc
CFLAGS?=-Wall -Wextra -pedantic -Wno-parentheses -std=c90 -g
#LDLIBS=

SRC=./lib/argparser.c ./main.c

DIR?=./tmp
OUT?=a.out

ifeq ($(DIR),)
$(error root has been removed cusscecfully)
endif


build: _FORCE tmp-struct
	$(CC) $(CFLAGS) $(SRC) $(LDLIBS) -o $(DIR)/$(OUT)

run: _FORCE
	$(DIR)/$(OUT)

clear: _FORCE
	rm -rf $(DIR)

tmp-struct: _FORCE
	mkdir -p $(DIR)

_FORCE:
