all:
	sudo clang++ --std=c++17 -o /var/www/cgi-bin/todolist.cgi todolist.cpp -lwcp -lwolfrini
	sudo chown -R apache:apache /var/www/cgi-bin/*
	sudo chown -R apache:apache /var/www/cgi-bin
	sudo touch /var/www/cgi-bin/todo.txt
	sudo chmod +777 /var/www/cgi-bin/todo.txt