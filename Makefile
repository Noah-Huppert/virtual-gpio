SRC=src/main.c

OUT_DIR=build
OUT=${OUT_DIR}/virtual-gpio

virtual-gpio: ${SRC}
	mkdir -p ${OUT_DIR}
	gcc -o ${OUT} ${SRC}
