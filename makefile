GCC=gcc

.PHONY: run clean run-dev clean-dev

./src/vm: ./src/main.c
	gcc $^ -o $@

./dev/build: ./dev/main.c
	gcc $^ -o $@

run: 
	./src/build

clean:
	rm src/build

run-dev:
	./dev/build

clean-dev:
	rm dev/build

dev: ./dev/build

build: ./src/vm