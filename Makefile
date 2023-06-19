all:
	gcc func.c system.c map.c computer.c main.c -o main
	gcc func.c system.c map.c computer.c main.c -o test -D DEBUG
	gcc func.c system.c map.c computer.c main_demo.c -o main_demo
	gcc func.c system.c map.c computer.c main_demo.c -o test_demo -D DEBUG
	@echo "Complete successfully.\n"
