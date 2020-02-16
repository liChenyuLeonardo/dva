#include "../core/reactor/doubleBufferedQueue.h"
#include <iostream>
#include <thread>
#include <unistd.h>
#include <string>
using namespace std;
struct event_t
{
    int pid;
    string msg; 
    event_t():msg("Hello World!"),pid(0){}
};

void consumer(doubleBufferedQueue<event_t>& haha)
{
    for(int i = 0; i < 20; i++){
        usleep(300);
        if(haha.Empty())
            cout << "tid: "<<std::this_thread::get_id()<<" found that reader queue is empty" << endl;
        else{
            cout << "tid: "<<std::this_thread::get_id()<< haha.Read().msg <<" size =  "<< haha.Size() << endl;
        }
        
    }
}

void producer(doubleBufferedQueue<event_t>& haha)
{
    for(int i = 0; i < 20; i++){
        usleep(300);
        event_t event;
	event.msg = "fuck you!";
	event.pid = 0;
        haha.Push(event);
        cout << "tid: "<<std::this_thread::get_id()<< " insert into writer queue" << endl;

    }
}

int main()
{
    doubleBufferedQueue<event_t> haha;
    thread c1(consumer, std::ref(haha));
    thread c2(consumer, std::ref(haha));
    thread p1(producer, std::ref(haha));
    thread p2(producer, std::ref(haha));
    c1.join();
    cout<< "thread c1 exited" << endl;
    c2.join();
    cout<< "thread c2 exited" << endl;
    p1.join();
    cout<< "thread p1 exited" << endl;
    p2.join();
    cout<< "thread p2 exited" << endl;
}
