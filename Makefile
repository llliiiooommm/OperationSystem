CC = clang

# Стандарт C
CFLAGS = -std=c17 -Wall -Wextra -Wpedantic -Werror

# Режимы сборки
DEBUG_FLAGS = -g -O0 $(CFLAGS)
RELEASE_FLAGS = -O2 -DNDEBUG $(CFLAGS)

# Санитайзеры для debug
SANITIZE_FLAGS = -fsanitize=address,undefined -fno-omit-frame-pointer

# Пути
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
BIN_DIR = bin

# Объектные файлы
OBJ = $(SRC_DIR)/args.o $(SRC_DIR)/main.o $(SRC_DIR)/output.o $(SRC_DIR)/proc_info.o

# Исполняемый файл
TARGET = $(BIN_DIR)/procview

# ========================
# Основная цель
# ========================
all: $(TARGET)

# ========================
# Цели для отладки
# ========================
debug: CFLAGS += $(SANITIZE_FLAGS) $(DEBUG_FLAGS)
debug: $(TARGET)

release: CFLAGS += $(RELEASE_FLAGS)
release: $(TARGET)

# ========================
# Запуск программы
# ========================
run: $(TARGET)
	$(TARGET) $$

# ========================
# Очистка
# ========================
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# ========================
# Сборка исполняемого файла
# ========================
$(TARGET): $(OBJ)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

# ========================
# Общие правила для .c файлов
# ========================
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c -o $@ $<

# Объявляем цели как служебные
.PHONY: all debug release run clean