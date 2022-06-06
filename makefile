.PHONY: run clean run-dev clean-dev

./vm: ./src/main.c
	gcc $^ -o $@

./vm-dev: ./dev/main.c
	gcc $^ -o $@

run: 
	./vm

clean:
	rm vm

run-dev:
	./dev/vm

clean-dev:
	rm ./dev/vm

dev: ./vm-dev

build: ./vm