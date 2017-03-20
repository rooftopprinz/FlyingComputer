TLD = ..
LIBOUT = $(TLD)/libs
CC = g++
INCLUDE = -I.
CFLAGS = -std=c++11 -Wall -Werror $(INCLUDE)

all:
	mkdir -p $(LIBOUT)
	$(CC) $(CFLAGS) -c gmock-gtest-all.cc -o $(LIBOUT)/gtest.o