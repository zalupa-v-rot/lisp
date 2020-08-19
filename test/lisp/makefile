build: _FORCE
	make -C ../ MODULES=$(shell basename $(CURDIR))

hidden_build: _FORCE
	$(MAKE) -C ../ build_me_please DIR='$(DIR)/lisp-common0' LISP_DEFINES='OPTION_NUMBERS OPTION_MATH OPTION_HOOK_INTER_RET' NOT_OVERRIDE_SRC='main.c guts.c'
	# $(MAKE) -C ../ build_me_please DIR='$(DIR)/lisp-common1' LISP_DEFINES='OPTION_HOOK_INTER_RET' NOT_OVERRIDE_SRC='main.c guts.c'


_FORCE:
