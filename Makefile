NAME = DOOM

CC = clang
CXX = clang++

ifdef IN_NIX_SHELL
    CXXFLAGS = -std=c++17 -Wall -g -O2 $(shell pkg-config --cflags sdl2 SDL2_image SDL2_ttf SDL2_mixer)
    LDFLAGS  = $(shell pkg-config --libs sdl2 SDL2_image SDL2_ttf SDL2_mixer)
else
    CXXFLAGS = -std=c++17 -Wall -g -O2
    LDFLAGS  = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
endif

INCLUDE = -Iinclude

MODULES = WAD Engine
SRC_DIRS = $(addprefix src/,$(MODULES)) src
SRCS  = $(foreach sdir,$(SRC_DIRS),$(wildcard $(sdir)/*.cpp))

OBJ_DIR = obj
OBJS = $(patsubst src/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

.PHONY: all
all: $(NAME)

$(OBJ_DIR)/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	@echo "Compiling $<"
	$(CXX) -c $(CXXFLAGS) $(INCLUDE) $< -o $@
	@echo ""

$(NAME): $(OBJS)
	$(CXX) $^ $(LDFLAGS) -o $@
	@echo ""

.PHONY: clean
clean:
	rm -f $(NAME)
	@rmdir $(OBJ_DIR) 2>/dev/null || true

.PHONY: run
run: $(NAME)
	@chmod +x $(NAME)
	./$(NAME)
	@echo ""

.PHONY: re
re: clean all
	@echo ""
