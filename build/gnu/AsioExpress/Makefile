CXX := g++
LINK := g++
AR := ar rc

# may be overridden
BOOST_INCLUDE  ?= /usr/include
BOOST_LIB      ?= /usr/lib

PROJECT         := AsioExpress
SPATH           := ../../../source
IPATH           := ../../../include
OUT_DIR         := ../../../lib
BASE_FLAGS      := -D_GNU_SOURCE -Wall -Werror -Wno-unused-variable -Wno-unused-local-typedefs -std=c++03 -pthread -I$(SPATH) -I$(BOOST_INCLUDE) -c
DBG_FLAGS       := -g -O0 -D _DEBUG
REL_FLAGS       := -O3 -D NDEBUG -Wno-unused-variable
SHARED_FLAGS    := -fPIC

PROJECT_LC         := $(shell echo $(PROJECT) | tr A-Z a-z)
SL_TARGET          := $(OUT_DIR)/lib$(PROJECT_LC)$(LIB_EXT).a
SL_OBJ_DIR         := ./$(CONFIGURATION)_static_objs
SL_OBJ_DIR_EXISTS  := $(SL_OBJ_DIR)/exists
DL_TARGET          := $(OUT_DIR)/lib$(PROJECT_LC)-shared$(LIB_EXT).a
DL_OBJ_DIR         := ./$(CONFIGURATION)_shared_objs
DL_OBJ_DIR_EXISTS  := $(DL_OBJ_DIR)/exists

SOURCE_DIR      := $(SPATH)/$(PROJECT)
INC_DIR         := $(IPATH)/$(PROJECT)
VPATH           := $(shell find $(SOURCE_DIR) -type d -print)   # folders to search
INCLUDE_DIRS    := $(SPATH)/AsioExpressConfig $(SPATH)/AsioExpressError

AUTO_SOURCES     := $(shell find $(SOURCE_DIR) -path $(SOURCE_DIR)/Platform -prune -o -type f -name '*.cpp' -print)
PLAT_SOURCES     := $(shell find $(SOURCE_DIR)/Platform -type f -name '*Linux.cpp' -print)
SOURCES          := $(AUTO_SOURCES) $(PLAT_SOURCES)
EXTERN_INCLUDES  := $(shell find $(INCLUDE_DIRS) -type f -name '*.h*')
LOCAL_INCLUDES   := $(shell find $(SOURCE_DIR) -type f -name '*.h*')
INCLUDES         := $(LOCAL_INCLUDES) $(EXTERN_INCLUDES)

SL_OBJECTS       := $(addprefix $(SL_OBJ_DIR)/, $(notdir $(SOURCES:.cpp=.o)))
DL_OBJECTS       := $(addprefix $(DL_OBJ_DIR)/, $(notdir $(SOURCES:.cpp=.o)))
CXXFLAGS         := $(BASE_FLAGS)  $(addprefix -I, $(INCLUDE_DIRS))

PCH_TARGET       := $(OBJ_DIR)/pch.hpp.gch
PCH_SOURCE       := $(SOURCE_DIR)/pch.cpp

.SECONDARY: # do not delete intermediate files

.PHONY: debug
debug:
	$(MAKE) EXTRA_FLAGS="$(DBG_FLAGS)"  LIB_EXT="-dbg" CONFIGURATION=debug build

.PHONY: release
release:
	$(MAKE) EXTRA_FLAGS="$(REL_FLAGS)" CONFIGURATION=release build

.PHONY: all
all: debug release

.PHONY: build
build: $(SL_TARGET) $(DL_TARGET) $(INC_DIR)

$(INC_DIR): $(LOCAL_INCLUDES)
	rm -rf $(INC_DIR)
	mkdir -p $(INC_DIR)
	cp -ar $(SOURCE_DIR) $(IPATH)
	find $(INC_DIR)/ -iname '*.cpp' -exec rm {} \;
	rm -rf $(INC_DIR)/Proc/CodeGen

$(PCH_TARGET): $(PCH_SOURCE) $(INCLUDES)
	$(CXX) $(CXXFLAGS) $(EXTRA_FLAGS) $< -o $@

$(SL_TARGET): $(SL_OBJECTS)
	mkdir -p $(OUT_DIR)
	$(AR) $@ $^

$(DL_TARGET): $(DL_OBJECTS)
	mkdir -p $(OUT_DIR)
	$(AR) $@ $^
#	$(LINK) -shared -o $@ $^

$(SL_OBJ_DIR)/%.o: %.cpp $(INCLUDES) $(SL_OBJ_DIR_EXISTS)
	$(CXX) -I$(OBJ_DIR) $(CXXFLAGS) $(EXTRA_FLAGS) $< -o $@

$(DL_OBJ_DIR)/%.o: %.cpp $(INCLUDES) $(DL_OBJ_DIR_EXISTS)
	$(CXX) -I$(OBJ_DIR) $(CXXFLAGS) $(EXTRA_FLAGS) $(SHARED_FLAGS) $< -o $@

$(SL_OBJ_DIR_EXISTS):
	if [ ! -d $(SL_OBJ_DIR) ]; then mkdir $(SL_OBJ_DIR); fi
	touch $(SL_OBJ_DIR_EXISTS)
	echo '#error Not using precompiled header!\n' >$(SL_OBJ_DIR)/pch.hpp

$(DL_OBJ_DIR_EXISTS):
	if [ ! -d $(DL_OBJ_DIR) ]; then mkdir $(DL_OBJ_DIR); fi
	touch $(DL_OBJ_DIR_EXISTS)
	echo '#error Not using precompiled header!\n' >$(DL_OBJ_DIR)/pch.hpp

.PHONY: clean
clean: clean-debug clean-release

.PHONY: clean-debug
clean-debug:
	$(MAKE) CONFIGURATION=debug LIB_EXT="-dbg" clean-target

.PHONY: clean-release
clean-release:
	$(MAKE) CONFIGURATION=release clean-target

.PHONY: clean-target
clean-target:
	-rm -f $(SL_TARGET) 2</dev/null
	-rm -f $(DL_TARGET) 2</dev/null
	-rm -rf $(SL_OBJ_DIR) 2</dev/null
	-rm -rf $(DL_OBJ_DIR) 2</dev/null
	-rm -rf $(INC_DIR)
