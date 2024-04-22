TARGET = TRV-Emu

run:
	cmake -S . -B build/ -D CMAKE_BUILD_TYPE=Debug && cmake --build ./build/
ifeq ($(OS),Windows_NT)
	./build/Release/$(TARGET).exe
else
	./build/$(TARGET)
endif
