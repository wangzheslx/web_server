CXX ?= g++
server: main.cpp  ./timer/lst_timer.cpp ./http/http_conn.cpp ./log/log.cpp ./CGImysql/sql_connection_pool.cpp  webserver.cpp config.cpp
	$(CXX) -o server  $^ -lpthread -lmysqlclient

clean:
	rm  -r server
