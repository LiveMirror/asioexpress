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
OUT_DIR         := ../../../lib/$(CONFIGURATION)
BASE_CFLAGS     := -D_GNU_SOURCE -Wall -Werror -std=c++03 -pthread -I$(SPATH) -I$(BOOST_INCLUDE) -c
DBG_CFLAGS      := -g -O0 -D _DEBUG
REL_CFLAGS      := -O3 -D NDEBUG
SHARED_CFLAGS   := -fPIC

PROJECT_LC      := $(shell echo $(PROJECT) | tr A-Z a-z)
TARGET          := $(OUT_DIR)/lib$(PROJECT_LC).a
OBJ_DIR         := ./$(CONFIGURATION)_objs
OBJ_DIR_EXISTS  := $(OBJ_DIR)/exists 

SOURCE_DIR      := $(SPATH)/$(PROJECT)
INC_DIR         := $(IPATH)/$(PROJECT)
VPATH           := $(shell find $(SOURCE_DIR) -type d -print)   # folders to search
INCLUDE_DIRS    := $(SPATH)/AsioExpressConfig
LIBS            := 
EXTERN_LIBS     := 

AUTO_SOURCES     := $(shell find $(SOURCE_DIR) -path $(SOURCE_DIR)/Windows -prune -o -type f -name '*.cpp' ! -name 'pch.cpp' -print)
PLAT_SOURCES     := 
SOURCES          := $(AUTO_SOURCES) $(PLAT_SOURCES)
EXTERN_INCLUDES  := $(shell find $(INCLUDE_DIRS) -type f -name '*.h*' -print)
LOCAL_INCLUDES   := $(shell find $(SOURCE_DIR)  -path $(SOURCE_DIR)/Windows -prune -o -type f -name '*.h*' -print)
INCLUDES         := $(LOCAL_INCLUDES) $(EXTERN_INCLUDES)

OBJECTS          := $(addprefix $(OBJ_DIR)/, $(notdir $(SOURCES:.cpp=.o))) 
CFLAGS           := $(BASE_CFLAGS)  $(addprefix -I, $(INCLUDE_DIRS))

.SECONDARY: # do not delete intermediate files 

.PHONY: debug
debug:
	$(MAKE) EXTRA_CFLAGS="$(DBG_CFLAGS)" CONFIGURATION=debug build
	$(MAKE) EXTRA_CFLAGS="$(DBG_CFLAGS) $(SHARED_CFLAGS)" CONFIGURATION=debug_shared build

.PHONY: release
release:
	$(MAKE) EXTRA_CFLAGS="$(REL_CFLAGS)" CONFIGURATION=release build
	$(MAKE) EXTRA_CFLAGS="$(REL_CFLAGS) $(SHARED_CFLAGS)" CONFIGURATION=release_shared build

.PHONY: all
all: debug release

.PHONY: build
build: $(TARGET) $(INC_DIR)

$(INC_DIR): $(LOCAL_INCLUDES)
	rm -rf $(INC_DIR)
	mkdir -p $(INC_DIR)
	cp -ar $(SOURCE_DIR) $(IPATH)
	find $(INC_DIR)/ -iname '*.cpp' -exec rm {} \; 

$(TARGET): $(OBJECTS) $(LIBS)
	mkdir -p $(OUT_DIR)
	$(AR) $@ $^	

$(OBJ_DIR)/%.o: %.cpp $(INCLUDES) $(OBJ_DIR_EXISTS) 
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $< -o $@

$(OBJ_DIR_EXISTS): 
	if [ ! -d $(OBJ_DIR) ]; then mkdir $(OBJ_DIR); fi
	touch $(OBJ_DIR_EXISTS)

.PHONY: clean
clean: clean-debug clean-release

.PHONY: clean-debug
clean-debug:
	$(MAKE) CONFIGURATION=debug clean-target
	$(MAKE) CONFIGURATION=debug_shared clean-target

.PHONY: clean-release
clean-release:
	$(MAKE) CONFIGURATION=release clean-target
	$(MAKE) CONFIGURATION=release_shared clean-target

.PHONY: clean-target
clean-target:
	-rm -f $(TARGET) 2</dev/null
	-rm -rf $(OBJ_DIR) 2</dev/null
	-rm -rf $(INC_DIR)
