CXX := g++
LINK := g++
AR := ar rc

# may be overridden
BOOST_INCLUDE  ?= /usr/include
BOOST_LIB      ?= /usr/lib

PROJECT         := SampleIpcServer1
SPATH           := ../../../source
OUT_DIR         := ../../../bin/$(CONFIGURATION)
LIB_DIR         := ../../../lib
OBJ_DIR         := ./$(CONFIGURATION)_objs
OBJ_DIR_EXISTS  := $(OBJ_DIR)/exists
BASE_FLAGS      := -D_GNU_SOURCE -Wall -Werror -Wno-unused-variable -Wno-unused-local-typedefs -std=c++03 -pthread -I$(SPATH) -I$(BOOST_INCLUDE) -c
DBG_FLAGS       := -g -O0 -D _DEBUG
REL_FLAGS       := -O3 -D NDEBUG

TARGET          := $(OUT_DIR)/$(PROJECT)
SOURCE_DIR      := $(SPATH)/$(PROJECT)
VPATH           := $(shell find $(SOURCE_DIR) -type d -print)   # folders to search
INCLUDE_DIRS    := $(SPATH)/AsioExpressConfig $(SPATH)/AsioExpressError $(SPATH)/AsioExpress

LIBS            := $(LIB_DIR)/libasioexpress$(LIB_EXT).a $(LIB_DIR)/libasioexpresserror$(LIB_EXT).a \
                   $(BOOST_LIB)/libboost_system.a $(BOOST_LIB)/libboost_thread.a $(BOOST_LIB)/libboost_chrono.a -lrt

AUTO_SOURCES     := $(shell find $(SOURCE_DIR) -type f -name '*.cpp' -print)
PLAT_SOURCES     :=
SOURCES          := $(AUTO_SOURCES) $(PLAT_SOURCES)
EXTERN_INCLUDES  := $(shell find $(INCLUDE_DIRS) -type f -name '*.h*' -print)
INCLUDES         := $(shell find $(SOURCE_DIR) -type f -name '*.h*' -print) $(EXTERN_INCLUDES)

OBJECTS   := $(addprefix $(OBJ_DIR)/, $(notdir $(SOURCES:.cpp=.o)))
CXXFLAGS  := $(BASE_FLAGS)  $(addprefix -I, $(INCLUDE_DIRS))

.SECONDARY: # do not delete intermediate files

.PHONY: debug
debug:
	$(MAKE) EXTRA_FLAGS="$(DBG_FLAGS)" LIB_EXT="-dbg" CONFIGURATION=debug build

.PHONY: release
release:
	$(MAKE) EXTRA_FLAGS="$(REL_FLAGS)" CONFIGURATION=release build

.PHONY: all
all: debug release

.PHONY: build
build: $(TARGET)

$(TARGET): $(OBJECTS) $(LIBS)
	mkdir -p $(OUT_DIR)
	$(LINK) -o $@ $^ -pthread

$(OBJ_DIR)/%.o: %.cpp $(INCLUDES) $(OBJ_DIR_EXISTS)
	$(CXX) $(CXXFLAGS) $(EXTRA_FLAGS) $< -o $@

$(OBJ_DIR_EXISTS):
	if [ ! -d $(OBJ_DIR) ]; then mkdir $(OBJ_DIR); fi
	touch $(OBJ_DIR_EXISTS)

.PHONY: clean
clean: clean-debug clean-release

.PHONY: clean-debug
clean-debug:
	$(MAKE) CONFIGURATION=debug clean-target

.PHONY: clean-release
clean-release:
	$(MAKE) CONFIGURATION=release clean-target

.PHONY: clean-target
clean-target:
	-rm -f $(TARGET) 2</dev/null
	-rm -rf $(OBJ_DIR) 2</dev/null
