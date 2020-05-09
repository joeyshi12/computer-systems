CFLAGS += -std=gnu11 -g
EXES    = greet BinaryTree

all:  $(EXES)
clean:
	rm -f $(EXES)

all: greet.c BinaryTree.c
# don't treat all and clean as file targets
.PHONY: all clean
