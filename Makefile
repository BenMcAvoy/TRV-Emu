TARGET = TRV-Emu

run:
	cmake -S . -B build/ -D CMAKE_BUILD_TYPE=Debug && cmake --build ./build/
ifeq ($(OS),Windows_NT)
    cd ./build; $(TARGET).exe $(TARGET).exe
else
	cd ./build && ./$(TARGET) ./$(TARGET)
endif
