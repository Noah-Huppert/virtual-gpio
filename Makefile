.PHONY: run build clean debug

OUT_DIR=build
OUT=${OUT_DIR}/virtual-gpio

TEST_CONTROL_FILE_DIR=vgpio

# run binary with test directory
run: build/virtual-gpio
	${OUT} ${TEST_CONTROL_FILE_DIR}

# shortcut
build: ${OUT}

# build virtual gpio binary
${OUT}: $(shell find src -type f)
	./scripts/check-misspelling.sh
	mkdir -p ${OUT_DIR}
	gcc -Wall -g -o ${OUT} $^

# clean build ouput
clean:
	rm -rf ${OUT_DIR}

# debug runs the program in GDB
debug: ${OUT}
	gdb \
		-tui \
		-x .debug.gdb \
		-iex "set args ${TEST_CONTROL_FILE_DIR}" \
		${OUT} 
