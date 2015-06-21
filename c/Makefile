CFLAGS := -Wall

test_simeck.run: main.c simeck64.h
	gcc $(CFLAGS) $< -o $@


.PHONY: clean
clean:
	rm -rf *.out *.run *.o
