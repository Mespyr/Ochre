CPP=clang++
CPPFLAGS=-Wall -Wextra -pedantic
LDFLAGS=

SRC_DIRS=src src/lexer src/parser src/type_checker src/compiler src/assembly src/include
SRC_FILES=$(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.cpp))
HEADERS=$(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.hpp))

OBJ_DIR=obj
OBJ_FILES=$(foreach dir, $(SRC_DIRS), \
  $(patsubst $(dir)/%.cpp, $(OBJ_DIR)/%.o, $(wildcard $(dir)/*.cpp)))
BIN=ochre

define compile_dir
$(OBJ_DIR)/%.o: $(1)/%.cpp $(OBJ_DIR)
	$(CPP) $(CPPFLAGS) -c $$< -o $$@
endef
$(foreach dir, $(SRC_DIRS), $(eval $(call compile_dir, $(dir))))

all: $(BIN)
$(BIN): $(OBJ_FILES) $(OBJ_DIR)
	$(CPP) $(OBJ_FILES) -o $@

format: $(SRC_FILES) $(HEADERS)
	clang-format -i $(SRC_FILES) $(HEADERS) -style=file

test: all
	python3 run_tests.py

$(OBJ_DIR):
	mkdir -p $@

clean:
	$(RM) -r $(OBJ_DIR) $(BIN)
