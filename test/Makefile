CC = clang
LD = clang

TEST_SRC_DIR := src
BUILD_DIR := build

APP_SRC := \
	mbadu_ascii.c \
	mbadu_tcp.c \
	mbadu.c \
	mbcoil.c \
	mbcrc.c \
	mbfn_coils.c \
	mbfn_digs.c \
	mbfn_regs.c \
	mbfn_serial.c \
	mbinst.c \
	mbpdu.c \
	mbreg.c \
	mbsupp.c \
	mbtest.c \
	utils/endian.c

TEST_SRC := ${sort ${wildcard ${TEST_SRC_DIR}/*.c}}
TESTS := ${patsubst ${TEST_SRC_DIR}/%.c,${BUILD_DIR}/${TEST_SRC_DIR}/%,${TEST_SRC}}

TEST_OBJECTS := ${addprefix ${BUILD_DIR}/, ${TEST_SRC:.c=.o}}
APP_OBJECTS := ${addprefix ${BUILD_DIR}/app/, ${APP_SRC:.c=.o}}

DEPENDENCY_OBJECTS := ${TEST_OBJECTS:.o=.d} ${APP_OBJECTS:.o=.d}

CFLAGS = -std=c11 -I../src
# Generate dependency information
CFLAGS += -MMD -MP -MF"${@:%.o=%.d}"

# Clang Undefined Behavior Sanitizer
ifeq (${CC}, clang)
CFLAGS += \
	-fsanitize=undefined \
	-fsanitize-trap=all \
	-fsanitize=implicit-conversion \
	-fsanitize=integer \
	-fsanitize=nullability
endif

LDFLAGS =

.PHONY: test clean

test: ${TESTS}
	${subst ${} ${}, && ,${^:%=./%}}

clean:
	@rm -rf ${BUILD_DIR}/

${TESTS}: ${APP_OBJECTS} ${TEST_OBJECTS}
	@mkdir -p ${dir $@}
	${LD} -o $@ $@.o ${APP_OBJECTS} ${LDFLAGS}

${BUILD_DIR}/%.o: %.c Makefile | ${BUILD_DIR}
	@mkdir -p ${dir $@}
	${CC} ${CFLAGS} -o $@ -c $<

${BUILD_DIR}/app/%.o: ../src/%.c Makefile | ${BUILD_DIR}
	@mkdir -p ${dir $@}
	${CC} ${CFLAGS} -o $@ -c $<

${BUILD_DIR}:
	@mkdir $@

-include ${DEPENDENCY_OBJECTS}
