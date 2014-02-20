
INSTALL_PATH ?= /usr/local
LIBRARY_NAME := asioexpress
INCLUDE_DIR   = $(INSTALL_PATH)/include/$(LIBRARY_NAME) 
LIB_DIR       = $(INSTALL_PATH)/lib/$(LIBRARY_NAME) 

.PHONY: default
default: 
	$(MAKE) -C AsioExpressConfig $(BUILD_TARGET)
	$(MAKE) -C AsioExpressError $(BUILD_TARGET)
	$(MAKE) -C AsioExpress $(BUILD_TARGET)
	$(MAKE) -C AsioExpressErrorTest $(BUILD_TARGET)
	$(MAKE) -C AsioExpressTest $(BUILD_TARGET)
	$(MAKE) -C SampleTcpServer1 $(BUILD_TARGET)
	$(MAKE) -C SampleTcpClient1 $(BUILD_TARGET)
	$(MAKE) -C SampleIpcServer1 $(BUILD_TARGET)
	$(MAKE) -C SampleIpcClient1 $(BUILD_TARGET)
	$(MAKE) -C SampleIpcClient2 $(BUILD_TARGET)

.PHONY: all
all: 
	$(MAKE) BUILD_TARGET=all

.PHONY: debug
debug: 
	$(MAKE) BUILD_TARGET=debug

.PHONY: release
release: 
	$(MAKE) BUILD_TARGET=release

.PHONY: clean
clean: 
	$(MAKE) BUILD_TARGET=clean

.PHONY: install
install:
	rm -rf $(INCLUDE_DIR)
	rm -rf $(LIB_DIR)
	mkdir -p $(INCLUDE_DIR)
	mkdir -p $(LIB_DIR)
	cp -r ../../include/* $(INCLUDE_DIR)
	cp -r ../../lib/* $(LIB_DIR)