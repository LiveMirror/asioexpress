CC = g++
LINK = g++ -pthread
LD = ld
AR = ar rc

BOOST_INCLUDE  ?= /usr/include     # maybe overridden
BOOST_LIB      ?= /usr/lib         # maybe overridden

PROJECT         = SampleIpcClient1
SPATH           = ../../../source
OUT_DIR         = ../../../bin/$(CONFIGURATION)
LIB_DIR         = ../../../lib/$(CONFIGURATION)
OBJ_DIR         = ./$(CONFIGURATION)_objs
OBJ_DIR_EXISTS  = $(OBJ_DIR)/exists 
BASE_CFLAGS     = -Wall -Werror -std=c++03 -pthread -fPIC -I$(SPATH) -I$(BOOST_INCLUDE) -c
DBG_CFLAGS      = -g -O0 -D _DEBUG -D_GNU_SOURCE
REL_CFLAGS      = -O2 -D NDEBUG -D_GNU_SOURCE -fwhole-program

TARGET          = $(OUT_DIR)/$(PROJECT)
SOURCE_DIR      = $(SPATH)/$(PROJECT)
VPATH           = $(shell find $(SOURCE_DIR) -type d -print)   # folders to search
INCLUDE_DIRS    = $(SPATH)/AsioExpressConfig $(SPATH)/AsioExpressError $(SPATH)/AsioExpress
LIBS            = 
EXTERN_LIBS     = $(LIB_DIR)/libasioexpress.a $(LIB_DIR)/libasioexpresserror.a \
                  $(BOOST_LIB)/libboost_system.a $(BOOST_LIB)/libboost_thread.a -lrt

AUTO_SOURCES     = $(shell find $(SOURCE_DIR) -type f -name '*.cpp' -print)
PLAT_SOURCES     = 
SOURCES          = $(AUTO_SOURCES) $(PLAT_SOURCES)
EXTERN_INCLUDES  = $(shell find $(INCLUDE_DIRS) -type f -name '*.h*' -print)
INCLUDES         = $(shell find $(SOURCE_DIR) -type f -name '*.h*' -print) $(EXTERN_INCLUDES)

OBJECTS = $(addprefix $(OBJ_DIR)/, $(notdir $(SOURCES:.cpp=.o))) 
CFLAGS  = $(BASE_CFLAGS)  $(addprefix -I, $(INCLUDE_DIRS))

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
build: $(TARGET) 
	
$(TARGET): $(OBJECTS) $(LIBS)
	mkdir -p $(OUT_DIR)
	$(LINK) -o $@ $^ $(EXTERN_LIBS) 
	
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

.PHONY: clean-release
clean-release:
	$(MAKE) CONFIGURATION=release clean-target

.PHONY: clean-target
clean-target:
	-rm -f $(TARGET) 2</dev/null
	-rm -rf $(OBJ_DIR) 2</dev/null
