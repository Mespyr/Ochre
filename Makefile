CPP=clang++
CPPFLAGS=-Wall -Wextra -pedantic
LDFLAGS=

# directories
SRC_DIR=src
LEXER_DIR=$(SRC_DIR)/lexer

# object files
OBJ_DIR=obj
OBJ_FILES=$(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(wildcard $(SRC_DIR)/*.cpp))
LEXER_OBJ_FILES=$(patsubst $(LEXER_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(wildcard $(LEXER_DIR)/*.cpp))

ALL_OBJ_FILES=$(OBJ_FILES) $(LEXER_OBJ_FILES)
BIN=ochre

all: $(BIN)

$(BIN): $(ALL_OBJ_FILES) $(OBJ_DIR)
	$(CPP) $(ALL_OBJ_FILES) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(OBJ_DIR)
	$(CPP) $(CPPFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

clean:
	$(RM) -r $(OBJ_DIR) $(BIN)
