# caxaphone
CC = gcc
CC_FLAGS = -g -Wall

SRC_DIR = src
BIN_DIR = bin
BUILD_DIR = build
INSTALL_DIR = ~/.local/bin

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst %.c,$(BUILD_DIR)/%.o,$(notdir $(SRCS)))
TARGET = $(BIN_DIR)/cax

all: $(TARGET)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) -c $< -o $@ $(CC_FLAGS) #$(LD_FLAGS)

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $^ -o $@ $(CC_FLAGS) #$(LD_FLAGS)

install:
	cp $(TARGET) $(INSTALL_DIR)

uninstall:
	rm $(INSTALL_DIR)/$(TARGET)

clean:
	rm -rf $(BIN_DIR) $(BUILD_DIR)

.PHONY: install uninstall clean
