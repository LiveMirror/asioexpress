CC := g++
LINK := g++
LD := ld
AR := ar rc

# may be overridden
BOOST_INCLUDE  ?= /usr/include
BOOST_LIB      ?= /usr/lib

PROJECT         := AsioExpressError
SPATH           := ../../../source
IPATH           := ../../../include
OUT_DIR         := ../../../lib
BASE_CFLAGS     := -D_GNU_SOURCE -Wall -Werror -Wno-unused-variable -Wno-unused-local-typedefs -std=c++03 -pthread -I$(SPATH) -I$(BOOST_INCLUDE) -c
DBG_CFLAGS      := -g -O0 -D _DEBUG
REL_CFLAGS      := -O3 -D NDEBUG
SHARED_CFLAGS   := -fPIC

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
INCLUDE_DIRS    := $(SPATH)/AsioExpressConfig

AUTO_SOURCES     := $(shell find $(SOURCE_DIR) -path $(SOURCE_DIR)/Windows -prune -o -type f -name '*.cpp' ! -name 'pch.cpp' -print)
PLAT_SOURCES     :=
SOURCES          := $(AUTO_SOURCES) $(PLAT_SOURCES)
EXTERN_INCLUDES  := $(shell find $(INCLUDE_DIRS) -type f -name '*.h*' -print)
LOCAL_INCLUDES   := $(shell find $(SOURCE_DIR)  -path $(SOURCE_DIR)/Windows -prune -o -type f -name '*.h*' -print)
INCLUDES         := $(LOCAL_INCLUDES) $(EXTERN_INCLUDES)

SL_OBJECTS       := $(addprefix $(SL_OBJ_DIR)/, $(notdir $(SOURCES:.cpp=.o)))
DL_OBJECTS       := $(addprefix $(DL_OBJ_DIR)/, $(notdir $(SOURCES:.cpp=.o)))
CFLAGS           := $(BASE_CFLAGS)  $(addprefix -I, $(INCLUDE_DIRS))

.SECONDARY: # do not delete intermediate files

.PHONY: debug
debug:
	$(MAKE) EXTRA_CFLAGS="$(DBG_CFLAGS)" LIB_EXT="-dbg" CONFIGURATION=debug build

.PHONY: release
release:
	$(MAKE) EXTRA_CFLAGS="$(REL_CFLAGS)" CONFIGURATION=release build

.PHONY: all
all: debug release

.PHONY: build
build: $(SL_TARGET) $(DL_TARGET) $(INC_DIR)

$(INC_DIR): $(LOCAL_INCLUDES)
	rm -rf $(INC_DIR)
	mkdir -p $(INC_DIR)
	cp -ar $(SOURCE_DIR) $(IPATH)
	find $(INC_DIR)/ -iname '*.cpp' -exec rm {} \;

$(SL_TARGET): $(SL_OBJECTS)
	mkdir -p $(OUT_DIR)
	$(AR) $@ $^

$(DL_TARGET): $(DL_OBJECTS)
	mkdir -p $(OUT_DIR)
	$(AR) $@ $^
	#$(LINK) -shared -o $@ $^

$(SL_OBJ_DIR)/%.o: %.cpp $(INCLUDES) $(SL_OBJ_DIR_EXISTS)
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $< -o $@

$(DL_OBJ_DIR)/%.o: %.cpp $(INCLUDES) $(DL_OBJ_DIR_EXISTS)
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(SHARED_CFLAGS) $< -o $@

$(SL_OBJ_DIR_EXISTS):
	if [ ! -d $(SL_OBJ_DIR) ]; then mkdir $(SL_OBJ_DIR); fi
	touch $(SL_OBJ_DIR_EXISTS)

$(DL_OBJ_DIR_EXISTS):
	if [ ! -d $(DL_OBJ_DIR) ]; then mkdir $(DL_OBJ_DIR); fi
	touch $(DL_OBJ_DIR_EXISTS)

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
