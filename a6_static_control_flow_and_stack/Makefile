CFLAGS += -std=gnu11 -g
EXES    = q4a q4b

all:  $(EXES)
clean:
	rm -f $(EXES)

targets: q4a.c q4b.c
# don't treat all and clean as file targets
.PHONY: all clean
