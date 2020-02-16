g++ -std=c++11 -lpthread test/acceptorTest.cpp core/reactor/Acceptor.h core/reactor/Acceptor.cpp core/reactor/epollEngine.h core/reactor/epollEngine.cpp core/reactor/doubleBufferedQueue.h lib/Socket.h lib/Socket.cpp -o deal 
g++ -std=c++11 test/clientTest.cpp -o client 
