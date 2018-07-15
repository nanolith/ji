CBMC_DIR?=/opt/cbmc
CBMC?=$(CBMC_DIR)/bin/cbmc

ALL:
	$(CBMC) --bounds-check --pointer-check --memory-leak-check \
	--div-by-zero-check --signed-overflow-check --unsigned-overflow-check \
    --pointer-overflow-check --conversion-check \
	--conversion-check --trace --stop-on-fail -DCBMC \
    --object-bits 16 --drop-unused-functions \
    --unwind 1 \
    --unwindset list_dispose.0:4 \
    --unwinding-assertions \
	-I ../include \
    ../modelsrc/*c \
    ../src/list/*c \
    ../src/disposable/*c \
	list_push_front_main.c
