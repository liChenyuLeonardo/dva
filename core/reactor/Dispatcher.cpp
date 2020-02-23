#include "Dispatcher.h"

void Dispatcher::reader(event_t& ev)
{
    if(ev.fd < 0 || ev.type != 1) return;
    
    
}