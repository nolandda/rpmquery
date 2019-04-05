
LDFLAGS := -lrpm -lrpmio

.PHONY: all
all: rpmquery.elf
	@echo "Build finished"

rpmquery.elf: rpmquery.o
	gcc -o rpmquery.elf rpmquery.o $(LDFLAGS)

rpmquery.o: rpmquery.c
	gcc -g -c rpmquery.c

.PHONY: clean
clean:
	rm -f *.o *.elf
