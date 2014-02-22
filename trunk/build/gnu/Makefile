
INSTALL_PATH ?= /usr/local
INCLUDE_DIR   = $(INSTALL_PATH)/include
LIB_DIR       = $(INSTALL_PATH)/lib

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
	mkdir -p $(INCLUDE_DIR)
	mkdir -p $(LIB_DIR)
	rm -rf $(INCLUDE_DIR)/AsioExpress
	rm -rf $(INCLUDE_DIR)/AsioExpressError
	rm -rf $(INCLUDE_DIR)/AsioExpressConfig
	cp -r ../../include/* $(INCLUDE_DIR)
	cp ../../lib/* $(LIB_DIR)

PKG_NAME := libasioexpress
SVN_REVISION := $(shell svn info . | grep Revision | sed -e's/.*: //')
FULL_VERSION := 1.0-$(SVN_REVISION)

.PHONY: package
package: release
	sudo checkinstall -y --install=no --pkgname $(PKG_NAME)\
		--pkgversion $(FULL_VERSION) --fstrans=yes\
		--pkglicense "Boost Software License \(BSL1.0\)"\
		--maintainer "Wurldtech Security Technologies <info@wurldtech.com>"\
		--exclude "/home/*"\
		$(MAKE) install
