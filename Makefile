CC = clang
CLANG = clang

BUILD_DIR := build

SRC := \
	src/endian.c \
	src/mbadu_ascii.c \
	src/mbadu_tcp.c \
	src/mbadu.c \
	src/mbcoil.c \
	src/mbcrc.c \
	src/mbfn_coils.c \
	src/mbfn_digs.c \
	src/mbfn_regs.c \
	src/mbfn_serial.c \
	src/mbinst.c \
	src/mbpdu.c \
	src/mbreg.c \
	src/mbsupp.c \
	src/mbtest.c

OBJECTS := ${addprefix ${BUILD_DIR}/, ${SRC:.c=.o}}
DEPENDENCY_OBJECTS := ${OBJECTS:.o=.d}

CFLAGS = \
	-std=c11 \
	-Isrc/ \
	-Wall -Wextra -Wmissing-include-dirs \
	-Wswitch-default -Wpedantic \
	-Wno-pointer-to-int-cast -Wno-int-to-pointer-cast

# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

.PHONY: all test clean analyze

all: ${OBJECTS}

test:
	${MAKE} -C test test

# Requires Clang/LLVM be installed
analyze:
	@echo "Running clang static analyzer..."
	@for src in ${SRC}; do \
		echo "Analyzing $$src..."; \
		output=$$(${CLANG} --analyze \
			-Xanalyzer -analyzer-output=text \
			$$src 2>&1); \
		if [ -n "$$output" ]; then \
			echo "$$output"; \
			exit 1; \
		fi; \
	done
	@echo "Static analysis complete - no issues found."

clean:
	@rm -rf ${BUILD_DIR}/
	${MAKE} -C test clean

${BUILD_DIR}/%.o: %.c Makefile | ${BUILD_DIR}
	@mkdir -p ${dir $@}
	${CC} ${CFLAGS} -o $@ -c $<

${BUILD_DIR}:
	@mkdir $@

-include ${DEPENDENCY_OBJECTS}
