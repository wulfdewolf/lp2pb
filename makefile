CC=g++
TARGET=lp2pb

BASEDIR = .
SRC_DIR := $(BASEDIR)/src
OBJ_DIR := $(BASEDIR)/obj

SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

all: $(TARGET) 

$(TARGET): $(OBJ_FILES)
	$(CC) -o $@ $^ main.cpp


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CC) -c $< -o $@

$(OBJ_DIR):
	mkdir $@

path:
	ln $(BASEDIR)/executables/lp2normal/lp2normal* ~/bin/lp2normal
	chmod +x ~/bin/lp2normal
	ln $(BASEDIR)/executables/lp2sat/lp2sat* ~/bin/lp2sat
	chmod +x ~/bin/lp2sat
	ln $(BASEDIR)/executables/lp2lp/lp2lp2* ~/bin/lp2lp2
	chmod +x ~/bin/lp2lp2

clean:
	$(RM) -r $(OBJ_DIR) $(TARGET)
