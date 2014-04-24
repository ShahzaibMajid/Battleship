
all:
	gcc -o server BattleServer.c
	gcc -o client client.c

test:
	@echo Please read the README.

submit:
	sudo svn commit -m "submitted for grading"

clean:
	-sudo rm server
	-sudo rm client
