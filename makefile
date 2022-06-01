.PHONY: run clean run-dev clean-dev

./vm: ./src/main.c
	gcc $^ -o $@

./dev/vm: ./dev/main.c
	gcc $^ -o $@

run: 
	./vm

clean:
	rm vm

run-dev:
	./dev/build

clean-dev:
	rm ./dev/vm

dev: ./dev/vm

build: ./vm