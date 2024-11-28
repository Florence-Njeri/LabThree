# or Debug
BUILD_TYPE=Release
TARGET=all

.PHONY: project_v0 project_v1 clean clear

all: project_v0 project_v1

project_v0:
	cmake -B $@/build -S $@ -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)
	cmake --build $@/build -j -t $(TARGET)
project_v1:
	cmake -B $@/build -S $@ -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)
	cmake --build $@/build -j -t $(TARGET)
clean:
	@$(MAKE) -f $(lastword $(MAKEFILE_LIST)) -s TARGET=clean all
clear: clean
	@rm -rvf project_v0/build project_v1/build
