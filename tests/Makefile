# MPU-925X driver, test makefile.

CC = gcc

TARGET = mpu925x_test

C_SOURCE = \
test.c \
../src/mpu925x_core.c \
../src/mpu925x_internals.c \
../src/mpu925x_settings.c \
Unity/src/unity.c

C_INCLUDE = \
-I../inc \
-IUnity/src

C_FLAGS = -Og -Wall $(C_INCLUDE)

all:
	$(CC) $(C_FLAGS) $(C_SOURCE) -o $(TARGET).out
	size $(TARGET).out

test: all
	./$(TARGET).out
