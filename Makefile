RELEASE_DIR := cmake-build-release
DEBUG_DIR := cmake-build-debug
LOCAL_INSTALL_DIR := lib

.PHONY: all

all:
	mkdir -p $(RELEASE_DIR) && cmake -B $(RELEASE_DIR)  -DCMAKE_BUILD_TYPE=Release && cd $(RELEASE_DIR) && make
	mkdir -p $(DEBUG_DIR) && cmake -B $(DEBUG_DIR) -DCMAKE_BUILD_TYPE=Debug && cd $(DEBUG_DIR) && make

clean:
	rm -rf $(RELEASE_DIR) $(DEBUG_DIR)

debug: 
	rm -rf ${DEBUG_DIR}
	@echo "Building in Debug mode..."
	mkdir -p $(DEBUG_DIR) && cmake -B $(DEBUG_DIR) -DCMAKE_BUILD_TYPE=Debug

release: 
	rm -rf ${RELEASE_DIR}
	@echo "Building in Release mode..."
	mkdir -p $(RELEASE_DIR) && cmake -B $(RELEASE_DIR) -DCMAKE_BUILD_TYPE=Release 

install_local:
	@echo "installing locally in ./lib..."
	mkdir -p $(RELEASE_DIR) && cmake -B $(RELEASE_DIR) -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="lib"
	mkdir -p $(LOCAL_INSTALL_DIR)
	cmake --build $(RELEASE_DIR) --target install

install: release
	@echo "installing..."
	cd $(RELEASE_DIR) && make install

debug_test: debug
	@echo "running tests in debug..."
	cd $(DEBUG_DIR) && make tests && ./tests

release_test: release
	@echo "running tests in release..."
	cd $(RELEASE_DIR) && make tests && ./tests


test: 	install_local
	@echo "running tests in debug..."
	cd $(DEBUG_DIR) && make tests && ./tests

	@echo "running tests in release..."
	cd $(RELEASE_DIR) && make tests && ./tests
