CC = g++
TARGET = arith_acm1987
CFLAGS = -Wall -Werror -Wextra -std=c++17
SRC = main arithmetic_coding encode decode

$(TARGET): $(addsuffix .cpp, $(SRC))
	$(CC) $(CFLAGS) -o $@ $^

clean:
	/bin/rm -f $(addsuffix .o, $(SRC))