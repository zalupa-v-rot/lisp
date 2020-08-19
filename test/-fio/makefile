SRC0=./main.c ./chekers.c ./string.c ./assert.c ./lisp/fio.c ./lisp/fio_file.c ./lisp/fio_part.c ./lisp/bits.c ./lib/list.c ./lib/rand.c
SRC1=./main.stress-byte.c ./chekers.c ./string.c ./assert.c ./lisp/fio.c ./lisp/fio_file.c ./lisp/fio_part.c ./lisp/bits.c ./lib/list.c ./lib/rand.c
SRC2=./main.stress-bit.c ./chekers.c ./string.c ./assert.c ./lisp/fio.c ./lisp/fio_file.c ./lisp/fio_part.c ./lisp/bits.c ./lib/list.c ./lib/rand.c

DIR?=../tmp
OUT0=test.fio
OUT1=test.stress-byte.fio
OUT2=test.stress-bit.fio

build: tmp-struct
	# $(CC) $(CFLAGS) $(SRC0) -DFIO_PART_SIZE=2048 -DTMP_DIR=$(DIR)/test.fio.files $(LDLIBS) -o $(DIR)/$(OUT0)
	# $(CC) $(CFLAGS) $(SRC1) -DFIO_PART_SIZE=2048 -DTMP_DIR=$(DIR)/test.fio.files $(LDLIBS) -o $(DIR)/$(OUT1)
	$(CC) $(CFLAGS) $(SRC2) -DFIO_PART_SIZE=2048 -DTMP_DIR=$(DIR)/test.fio.files $(LDLIBS) -o $(DIR)/$(OUT2)

run:
	rm -rf $(DIR)/test.fio.files
	mkdir $(DIR)/test.fio.files
	# $(DIR)/$(OUT0)
	# $(DIR)/$(OUT1)
	$(DIR)/$(OUT2)

tmp-struct: _FORCE
	mkdir -p $(DIR)

_FORCE:
