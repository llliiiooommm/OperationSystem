CC := gcc

# стандарт, основные предупреждения, дополнительные, 
# нарушения стандарта, любое предупреждение - ошибка
CFLAGS := -std=c17 -Wall -Wextra -Wpedantic -Werror

# пути к уже существующим каталогам проекта 
SRC_DIR := src
BUILD_DIR := build
BIN_DIR := bin

TARGET := $(BIN_DIR)/procview

# перечисляет все .c файлы
SOURCES := src/main.c src/args.c src/proc_info.c src/output.c
# создает соответствующие объекты
OBJECTS := build/main.o build/args.o build/proc_info.o build/output.o

# берет объектные файлы и объединяет в одну программу
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

# .c -> .o
build/%.o: src/%.c include/args.h include/proc_info.h include/output.h
	$(CC) $(CFLAGS) -c $< -o $@


.PHONY: all

all: $(TARGET)

# СОЗДАНИЕ ДИРЕКТОРИЙ
# создаёт директорию bin
$(TARGET): $(OBJECTS)
	mkdir -p $(BIN_DIR)
	$(CC) $(OBJECTS) -o $(TARGET)

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