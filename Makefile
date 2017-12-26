CC = g++ 
CFLAGS = -Wall -std=c++11 -O3
SRC_DIR = src
BIN_DIR = bin
TARGET = run



SRC_FIL += $(SRC_DIR)/main.cpp
SRC_FIL += $(SRC_DIR)/Debug.h
SRC_FIL += $(SRC_DIR)/define.h
SRC_FIL += $(SRC_DIR)/TraceInput.h

.PHONY: compile clean
$(TARGET): $(SRC_FIL)
	$(CC) $(CFLAGS) $(SRC_FIL) -o $(TARGET)

start: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(TARGET)
