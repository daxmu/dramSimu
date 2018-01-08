CC = g++
CFLAGS = -Wall -std=c++11 -O2
#CFLAGS = -Wall -std=c++11
SRC_DIR = src
BIN_DIR = bin
OBJ_DIR = obj
TARGET = run

SRC_FIL += $(SRC_DIR)/define.h
SRC_FIL += $(SRC_DIR)/Debug.h
SRC_FIL += $(SRC_DIR)/func.cpp
SRC_FIL += $(SRC_DIR)/Req.h
SRC_FIL += $(SRC_DIR)/baseStruct.h
SRC_FIL += $(SRC_DIR)/Port.cpp
SRC_FIL += $(SRC_DIR)/MultiPort.cpp
SRC_FIL += $(SRC_DIR)/TraceInput.cpp
SRC_FIL += $(SRC_DIR)/MemorySystem.cpp
SRC_FIL += $(SRC_DIR)/Controller.cpp
SRC_FIL += $(SRC_DIR)/CommandQueue.cpp
SRC_FIL += $(SRC_DIR)/StatusMachine.cpp
SRC_FIL += $(SRC_DIR)/main.cpp

.PHONY: compile clean
$(TARGET): $(SRC_FIL)
	$(CC) $(CFLAGS) $(SRC_FIL) -o $(TARGET)

start: $(TARGET)
	./$(TARGET) -t traceInput/k6_aoe_02_short.trc -c 1000000 > log

gdb_run: $(SRC_FIL)
	$(CC) -g $(CFLAGS) $(SRC_FIL) -o gdb_run

clean:
	rm -rf $(TARGET) gdb_run
