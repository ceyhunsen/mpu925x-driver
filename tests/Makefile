# Makefile for MPU-925X driver tests.
# Author: Ceyhun Şen

CC = gcc

# TARGET = mpu925x_test
# C_SOURCE = \
# test.c \
# ../src/mpu925x_core.c \
# ../src/mpu925x_internals.c \
# ../src/mpu925x_settings.c \
# Unity/src/unity.c

C_SOURCES = \
Unity/src/unity.c \
../src/mpu925x_core.c \
../src/mpu925x_internals.c \
../src/mpu925x_settings.c \

C_INCLUDE = \
-I../inc \
-IUnity/src \
-I. \

C_FLAGS = -O2 -Wall $(C_INCLUDE)

mock:
	$(CC) $(C_FLAGS) $(C_SOURCES) $@.c -o $@.out
	./$@.out

accelerometer:
	$(CC) $(C_FLAGS) $(C_SOURCES) $@.c -o $@.out
	./$@.out

all:
	$(CC) $(C_FLAGS) $(C_SOURCE) -o $(TARGET).out
	size $(TARGET).out

test: all
	./$(TARGET).out
