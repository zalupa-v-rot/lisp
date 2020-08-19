#CC
#CFLAGS
#LDFLAGS
#LDLIBS

#CDEFINES
#SRC source files
#FO  obj postfix
#DIR build directory
#OUT final binary code
CDEFINESD=$(addprefix -D, $(CDEFINES))
SRC_CC=$(filter %.c, $(SRC))
OBJ_CC=$(addprefix $(DIR)/src/, $(SRC:.c=$(FO))) $(addprefix $(DIR)/lisp/, $(LISP_SRC:.c=$(FO)))


build: $(DIR)/$(OUT)

# линковка
$(DIR)/$(OUT): $(OBJ_CC)
	$(CC) $(CFLAGS) $(CDEFINESD) $(LDFLAGS) $(OBJ_CC) $(LDLIBS) -o $(DIR)/$(OUT)

# компиляция
$(DIR)/src/%$(FO): ./$(NAME)/%.c
	$(CC) $(CFLAGS) $(CDEFINESD) -c -o $@ $<

$(DIR)/lisp/%$(FO): ./$(NAME)/lisp/%.c
	$(CC) $(CFLAGS) $(CDEFINESD) -c -o $@ $<

_FORCE:
