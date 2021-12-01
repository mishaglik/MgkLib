LIB_DIR = lib/
BIN_DIR = bin/
SRC_DIR = src/

MAJOR_VERSION = 1
MINOR_VERSION = 0
# BUILD_VERSION = `cat bld_version`
# TODO: Auto increment version

# SDL_LIB   = SDL2 SDL2main SDL2_image
LIBRARIES = 

CXXFLAGS = `cat $(LIB_DIR)Cflags` -I$(LIB_DIR)
SANFLAGS = `cat $(LIB_DIR)SanitizeFlags`
LXXFLAGS = -L$(LIB_DIR) $(addprefix -l, $(LIBRARIES))

# CXXFLAGS += -DMAJOR_VERSION=$(MAJOR_VERSION)
# CXXFLAGS += -DMINOR_VERSION=$(MINOR_VERSION)
# CXXFLAGS += -DBUILD_VERSION=$(BUILD_VERSION)

CXXFLAGS += $(SANFLAGS)

HEADERS = Logger/Logger.h Utils.h

SOURCES_Logger= Logger.cpp

SUBDIRS = ${shell find $(SRC_DIR) -type d -printf '%P '}

SOURCES = $(foreach dir, $(SUBDIRS), $(addprefix $(dir)/, $(SOURCES_$(dir))))

EXECUTABLE  = 

SRC = $(SOURCES) $(EXECUTABLE)

OBJ = $(SRC:.cpp=.o)

DEP = $(SRC:.cpp=.d)

TARGETS = lib

.PHONY: init
init:
	mkdir -p $(addprefix $(BIN_DIR), $(SUBDIRS))
	mkdir -p $(LIB_DIR)
	mkdir -p $(LIB_DIR)MGK

all: $(TARGETS)
	# ./increaseVersion.sh bld_version

.PHONY: lib
lib: $(addprefix $(BIN_DIR), $(OBJ))
	ar crf $(BIN_DIR)LibMGK.a $^
	cp $(BIN_DIR)LibMGK.a $(LIB_DIR)
	cp $(addprefix $(SRC_DIR), $(HEADERS)) $(LIB_DIR)MGK

$(BIN_DIR)%.o : $(SRC_DIR)%.cpp
	g++ -c $(CXXFLAGS) $(LXXFLAGS) -o $@ $<

.PHONY: deps
deps: $(addprefix $(BIN_DIR), $(DEP))
	echo "Deps builded"
	
$(BIN_DIR)%.d : $(SRC_DIR)%.cpp
	g++ -MM -MT $(@:.d=.o) $< -o $@ -I$(LIB_DIR)

-include $(addprefix $(BIN_DIR), $(DEP))

.PHONY: clean
clean:
	rm -f $(addprefix $(BIN_DIR), $(OBJ) $(DEP))
	rm $(TARGETS)
