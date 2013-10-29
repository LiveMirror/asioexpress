CC := g++
LINK := g++
LD := ld
AR := ar rc

# may be overridden
BOOST_INCLUDE  ?= /usr/include
BOOST_LIB      ?= /usr/lib

PROJECT         := AsioExpress
SPATH           := ../../../source
IPATH           := ../../../include
OUT_DIR         := ../../../lib/$(CONFIGURATION)
BASE_CFLAGS     := -D_GNU_SOURCE -Wall -Werror -std=c++03 -pthread -fPIC -I$(SPATH) -I$(BOOST_INCLUDE) -c
DBG_CFLAGS      := -g -O0 -D _DEBUG
REL_CFLAGS      := -O2 -D NDEBUG -fwhole-program

PROJECT_LC      := $(shell echo $(PROJECT) | tr A-Z a-z)
TARGET          := $(OUT_DIR)/lib$(PROJECT_LC).a
OBJ_DIR         := ./$(CONFIGURATION)_objs
OBJ_DIR_EXISTS  := $(OBJ_DIR)/exists 

SOURCE_DIR      := $(SPATH)/$(PROJECT)
INC_DIR         := $(IPATH)/$(PROJECT)
VPATH           := $(shell find $(SOURCE_DIR) -type d -print)   # folders to search
INCLUDE_DIRS    := $(SPATH)/AsioExpressConfig $(SPATH)/AsioExpressError
LIBS            := 
EXTERN_LIBS     := 

AUTO_SOURCES     := $(shell find $(SOURCE_DIR) -path $(SOURCE_DIR)/Platform -prune -o -type f -name '*.cpp' -print)
PLAT_SOURCES     := $(shell find $(SOURCE_DIR)/Platform -type f -name '*Linux.cpp' -print)
SOURCES          := $(AUTO_SOURCES) $(PLAT_SOURCES)
EXTERN_INCLUDES  := $(shell find $(INCLUDE_DIRS) -type f -name '*.h*')
LOCAL_INCLUDES   := $(shell find $(SOURCE_DIR) -type f -name '*.h*')
INCLUDES         := $(LOCAL_INCLUDES) $(EXTERN_INCLUDES)

OBJECTS := $(addprefix $(OBJ_DIR)/, $(notdir $(SOURCES:.cpp=.o)))
CFLAGS  := $(BASE_CFLAGS)  $(addprefix -I, $(INCLUDE_DIRS))

PCH_TARGET      := $(OBJ_DIR)/pch.hpp.gch
PCH_SOURCE      := $(SOURCE_DIR)/pch.cpp

.SECONDARY: # do not delete intermediate files 

.PHONY: debug
debug:
	$(MAKE) EXTRA_CFLAGS="$(DBG_CFLAGS)" CONFIGURATION=debug build

.PHONY: release
release:
	$(MAKE) EXTRA_CFLAGS="$(REL_CFLAGS)" CONFIGURATION=release build
	
.PHONY: all
all: debug release

.PHONY: build
build: $(TARGET) $(INC_DIR)

$(INC_DIR): $(LOCAL_INCLUDES)
	rm -rf $(INC_DIR)
	mkdir -p $(INC_DIR)
	cp -ar $(SOURCE_DIR) $(IPATH)
	find $(INC_DIR)/ -iname '*.cpp' -exec rm {} \;
	rm -rf $(INC_DIR)/Proc/CodeGen

$(PCH_TARGET): $(PCH_SOURCE) $(INCLUDES)
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $< -o $@

$(TARGET): $(OBJECTS) $(LIBS)
	mkdir -p $(OUT_DIR)
	$(AR) $@ $^	

$(OBJ_DIR)/%.o: %.cpp $(INCLUDES) $(OBJ_DIR_EXISTS) $(PCH_TARGET) 
	$(CC) -I$(OBJ_DIR) $(CFLAGS) $(EXTRA_CFLAGS) $< -o $@

$(OBJ_DIR_EXISTS): 
	if [ ! -d $(OBJ_DIR) ]; then mkdir $(OBJ_DIR); fi
	touch $(OBJ_DIR_EXISTS)
	echo '#error Not using precompiled header!\n' >$(OBJ_DIR)/pch.hpp 

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
	-rm -rf $(INC_DIR)
