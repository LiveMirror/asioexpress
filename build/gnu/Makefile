
INSTALL_PATH        := /usr/local
INSTALL_INCLUDE_DIR := $(INSTALL_PATH)/include
INSTALL_LIB_DIR     := $(INSTALL_PATH)/lib

INCLUDE_DIR         := ../../include
LIB_DIR             := ../../lib

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
	rm -f *.tgz *.deb description-pak

.PHONY: install
install:
	mkdir -p $(INSTALL_INCLUDE_DIR)
	mkdir -p $(INSTALL_LIB_DIR)
ifneq ($(DEBUG_NAME_SUFFIX), -dbg)
	# copy header files only for non-debug package, debug package will depend
	# on non-debug package to avoid having to deal with packages conflicting
	cp -r $(INCLUDE_DIR)/* $(INSTALL_INCLUDE_DIR)
endif
	cp $(LIB_DIR)/libasioexpress$(DEBUG_NAME_SUFFIX).*  $(INSTALL_LIB_DIR)
	cp $(LIB_DIR)/libasioexpresserror$(DEBUG_NAME_SUFFIX).*  $(INSTALL_LIB_DIR)

PKG_NAME := libasioexpress
SVN_REVISION := $(shell svn info . | grep Revision | sed -e's/.*: //')
FULL_VERSION := 1.0-$(SVN_REVISION)

.PHONY: do-package
do-package:
	sudo checkinstall -y --install=no --pkgname $(PKG_NAME)$(DEBUG_NAME_SUFFIX)\
		--pkgversion $(FULL_VERSION) --fstrans=yes\
		--pkglicense "Boost Software License \(BSL1.0\)"\
		--maintainer "Ross MacGregor \[http://sourceforge.net/projects/asioexpress/\]"\
		--exclude "/home/*" $(DEB_REQUIRE)\
		$(MAKE) install DEBUG_NAME_SUFFIX=$(DEBUG_NAME_SUFFIX)

.PHONY: release-package
release-package: release
	$(MAKE) do-package

.PHONY: debug-package
debug-package: debug
	$(MAKE) do-package DEBUG_NAME_SUFFIX=-dbg DEB_REQUIRE='--requires $(PKG_NAME)'

.PHONY: package
package: release-package debug-package
