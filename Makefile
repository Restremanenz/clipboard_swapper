# tool macros
CC ?= gcc
CCFLAGS := -Wall -O2 -pedantic
DBGFLAGS := -g
CCOBJFLAGS := $(CCFLAGS) -c

# path macros
BIN_PATH := bin
OBJ_PATH := obj
SRC_PATH := src
DBG_PATH := debug

LIB_DIR=./libclipboard
LIBS=-L$(LIB_DIR)/lib/ -lclipboard -lpthread -lxcb
INCLUDES=-I./libclipboard/include/

# compile macros
TARGET_NAME := clipboard_swapper
ifeq ($(OS),Windows_NT)
	TARGET_NAME := $(addsuffix .exe,$(TARGET_NAME))
endif
TARGET := $(BIN_PATH)/$(TARGET_NAME)
TARGET_DEBUG := $(DBG_PATH)/$(TARGET_NAME)

# src files & obj files
SRC := $(foreach x, $(SRC_PATH), $(wildcard $(addprefix $(x)/*,.c*)))
OBJ := $(addprefix $(OBJ_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))
OBJ_DEBUG := $(addprefix $(DBG_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))

# clean files list
DISTCLEAN_LIST := $(OBJ) \
                  $(OBJ_DEBUG)
CLEAN_LIST := $(TARGET) \
			  $(TARGET_DEBUG) \
			  $(DISTCLEAN_LIST)

# default rule
default: makedir all

# non-phony targets
$(TARGET): $(OBJ)
	$(CC) $(CCFLAGS) -o $@ $(OBJ) $(LIB)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c*
	$(CC) $(CCOBJFLAGS) -o $@ $< $(INCLUDES)

$(DBG_PATH)/%.o: $(SRC_PATH)/%.c*
	$(CC) $(CCOBJFLAGS) $(DBGFLAGS) -o $@ $<

$(TARGET_DEBUG): $(OBJ_DEBUG)
	$(CC) $(CCFLAGS) $(DBGFLAGS) $(OBJ_DEBUG) -o $@

$(LIB_DIR):
	git clone https://github.com/jtanx/libclipboard.git
	cmake -S $(LIB_DIR) -B $(LIB_DIR)
	make -j4 -C ./libclipboard/

# phony rules
.PHONY:install
install:$(LIB_DIR) makedir all

.PHONY: makedir
makedir:
	@mkdir -p $(BIN_PATH) $(OBJ_PATH) $(DBG_PATH)

.PHONY: all
all: $(TARGET)

.PHONY: debug
debug: $(TARGET_DEBUG)

.PHONY: clean
clean:
	@echo CLEAN $(CLEAN_LIST)
	@rm -f $(CLEAN_LIST)

.PHONY: distclean
distclean:
	@echo CLEAN $(DISTCLEAN_LIST)
	@rm -f $(DISTCLEAN_LIST)