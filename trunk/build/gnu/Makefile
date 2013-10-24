
.PHONY: default
default: 
	$(MAKE) -C AsioExpressConfig $(BUILD_TARGET)
	$(MAKE) -C AsioExpressError $(BUILD_TARGET)
	$(MAKE) -C AsioExpress $(BUILD_TARGET)
	$(MAKE) -C AsioExpressTest $(BUILD_TARGET)
	$(MAKE) -C SampleTcpServer1 $(BUILD_TARGET)
	$(MAKE) -C SampleTcpClient1 $(BUILD_TARGET)
	$(MAKE) -C SampleIpcServer1 $(BUILD_TARGET)
	$(MAKE) -C SampleIpcClient1 $(BUILD_TARGET)

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
