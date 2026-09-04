CC := gcc

# стандарт, основные предупреждения, дополнительные, 
# нарушения стандарта, любое предупреждение - ошибка, добавлил заголовки т к ошибки были
CFLAGS := -std=c17 -Wall -Wextra -Wpedantic -Werror -Iinclude

# пути к уже существующим каталогам проекта 
SRC_DIR := src
BUILD_DIR := build
BIN_DIR := bin

TARGET := $(BIN_DIR)/procview

# перечисляет все .c файлы
SOURCES := src/main.c src/args.c src/proc_info.c src/output.c
# создает соответствующие объекты
OBJECTS := build/main.o build/args.o build/proc_info.o build/output.o

.PHONY: all

all: $(TARGET)

# СОЗДАНИЕ ДИРЕКТОРИЙ (линковка)
# создаёт директорию bin
$(TARGET): $(OBJECTS)
	mkdir -p $(BIN_DIR)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $(TARGET)

# создаёт директорию build
build/%.o: src/%.c include/args.h include/proc_info.h include/output.h
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@


# ДЕБАГ
# санитайзеры требуемые по заданию
SANITIZE_FLAGS := -fsanitize=address,undefined -fno-omit-frame-pointer

.PHONY: debug

# добавление для дебаг сборки отладочную информацию и отключение оптимизации
debug: CFLAGS += -g -O0 $(SANITIZE_FLAGS)
# Говорим чтобы санитайзеры были не только при компиляции но и при линковке
debug: LDFLAGS += $(SANITIZE_FLAGS)

debug: clean all


# RELEASE
.PHONY: release
# для релейза добавляем оптимизацию 
release: CFLAGS += -O2
release: clean all

#тут был RUN но я удалил т к незачем

# CLEAN
.PHONY: clean

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)