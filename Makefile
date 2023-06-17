all:
	gcc func.c system.c map.c computer.c main.c -o main
	gcc func.c system.c map.c computer.c main.c -o test -D DEBUG
	@echo "Complete successfully.\n"
