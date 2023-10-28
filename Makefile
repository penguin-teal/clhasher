MKDIR 			:= mkdir -p --
RMDIR   		:= rm -Rf --
RM				:= rm --

EXTRA_CFLAGS	:=
WARNINGS		:= -Wall -Wextra -Wfloat-equal -Wundef
ERRORS			:= -Werror=implicit-int -Werror=implicit-function-declaration
CFLAGS 		    := $(WARNINGS) $(ERRORS) -std=c99 $(EXTRA_CFLAGS)
CURRENT_CFLAGS	:=

BIN   		    := ./bin
INCLUDE		    := ./include
SRC 		    := ./src
SRCS  		    := $(wildcard $(SRC)/*.c)
OUT				:= $(BIN)/clhasher
INCLUDEFLAGS	:= -I$(INCLUDE)
TESTSRC			:= ./tests
TESTSRCS		:= $(wildcard $(TESTSRC)/*.c)
TESTBIN			:= ./testbin
TESTOUTS		:= $(patsubst $(TESTSRC)/%.c,$(TESTBIN)/%,$(TESTSRCS))

CHECK_TEST_FLAGS:= -pthread -lcheck -lrt -lm

.PHONY: release debug tests clean

release: CURRENT_CFLAGS += -O3 -DNDEBUG
release: $(OUT) tests

debug: CURRENT_CFLAGS += -g3 -O0 -DDEBUG
debug: $(OUT) tests

$(OUT): $(SRCS)
	$(MKDIR) $(BIN)
	$(CC) $(CFLAGS) $(CURRENT_CFLAGS) $(INCLUDEFLAGS) $^ -o $(OUT)

tests: $(TESTOUTS)
	$(RM) $^

$(TESTBIN)/%: $(TESTSRC)/%.c
	$(MKDIR) $(TESTBIN)
	$(CC) $(CFLAGS) $(INCLUDEFLAGS) $(CHECK_TEST_FLAGS) -o $@ $<
	./$@

clean:
	$(RMDIR) $(TESTBIN) $(BIN)