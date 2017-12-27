CC = g++ 
CFLAGS = -Wall -std=c++11 -O3
SRC_DIR = src
BIN_DIR = bin
TARGET = run

SRC_FIL += $(SRC_DIR)/define.h
SRC_FIL += $(SRC_DIR)/Debug.h
SRC_FIL += $(SRC_DIR)/Req.h
SRC_FIL += $(SRC_DIR)/baseStruct.h
SRC_FIL += $(SRC_DIR)/Port.h
SRC_FIL += $(SRC_DIR)/TraceInput.h
SRC_FIL += $(SRC_DIR)/MemorySystem.h
SRC_FIL += $(SRC_DIR)/main.cpp

.PHONY: compile clean
$(TARGET): $(SRC_FIL)
	$(CC) $(CFLAGS) $(SRC_FIL) -o $(TARGET)

start: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(TARGET)
