.PHONY: run

OUT_DIR=build
OUT=${OUT_DIR}/virtual-gpio

TEST_CONTROL_FILE_DIR=vgpio

# run binary with test directory
run: virtual-gpio
	${OUT} ${TEST_CONTROL_FILE_DIR}

# build virtual gpio binary
virtual-gpio: $(shell find src -type f)
	mkdir -p ${OUT_DIR}
	gcc -Wall -g -o ${OUT} $^
