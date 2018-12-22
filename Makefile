OUT_DIR=build
OUT=${OUT_DIR}/virtual-gpio

virtual-gpio: $(shell find src -type f)
	mkdir -p ${OUT_DIR}
	gcc -Wall -g -o ${OUT} $^
