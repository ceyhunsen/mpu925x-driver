# Makefile for MPU-925X driver tests.
# Author: Ceyhun Şen

# All tests must have a .c file in its exact name.
TESTS = \
mock \
accelerometer \

# The rest of the file should not be touched.

CC = gcc

BUILD_DIR = build

C_SOURCES = \
Unity/src/unity.c \
../src/mpu925x_core.c \
../src/mpu925x_internals.c \
../src/mpu925x_settings.c \
../src/mpu925x_accelerometer.c \

C_INCLUDE = \
-I../inc \
-IUnity/src \
-I. \

C_FLAGS = -O2 -Wall $(C_INCLUDE)

all: $(TESTS) clean

%:
	mkdir -p $(BUILD_DIR)
	$(CC) $(C_FLAGS) $(C_SOURCES) $@.c -o $(BUILD_DIR)/$@.out
	$(BUILD_DIR)/$@.out

.PHONY: clean

clean:
	rm -rf $(BUILD_DIR) *.out *.o *.exe
