
TARGET_DIR = ../../../include/AsioExpressConfig
SOURCE_DIR = ../../../source/AsioExpressConfig
TARGET     = $(TARGET_DIR)/config.hpp
SOURCE     = $(SOURCE_DIR)/config.hpp

#.SECONDARY: # do not delete intermediate files 

.PHONY: debug
debug: $(TARGET)

.PHONY: release 
release: $(TARGET)

.PHONY: all
all: $(TARGET)

$(TARGET): $(SOURCE)
	mkdir -p $(TARGET_DIR)
	cp $^ $@	

clean:
	-rm -rf ../../../include/AsioExpressConfig 2</dev/null
