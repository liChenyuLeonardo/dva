#include "process.h"

processList::processList()
{
    currentNum = 0;
    head = new process_t();
    rear = head;
}

bool processList::insertList(process_t* theProcess)
{
    if(theProcess->pid == -1){
        //logger::log(FATAL_ERROR,"processList::insertList", "cannot insert an unset process" );
        return false;
    }
    if(rear == NULL){
        //logger::log(FATAL_ERROR,"processList::insertList", "process _list head pointer is not set! " );
        return false;
    }
    rear->next = theProcess;
    chlist(NULL, rear, theProcess);
    rear = rear->next;
    currentNum++;
    return true;
}

bool processList::removeList(pid_t pid)
{
    cout<<"remove pid " <<pid<<endl;
     if(pid < 0){
         //logger::log(FATAL_ERROR,"processList::removeList", "invalid pid! " );
        return false;        
     }
    if(head == NULL){
        //logger::log(FATAL_ERROR,"processList", "process _list head pointer is not set! " );
        return false;
    }
     process_t* temp = searchList(pid);
    if(temp == head){
         //logger::log(ERROR,"processList::removeList", "head pointer cannot be removed! " );
         return false;
     }
     if(temp == NULL){
            //logger::log(ERROR,"processList::removeList", "cannot find the specified process! " );
            return false;
     }
     else{
        process_t* a = temp->former;
        process_t* b = temp->next;
        a->next = b;
        if(b != NULL)
            b->former = a;
        if(temp == rear){
            rear = a;
        }
            delete temp;
            currentNum--;
            cout<<"currentNum is " << currentNum <<" , head->next " << (head->next == NULL? "NULL" : "NOT NULL")<< endl;
            return true;
     }
}

process_t* processList::searchList(pid_t pid)
{
     if(pid < 0){
         //logger::log(FATAL_ERROR,"processList::searchList", "invalid pid! " );
        return NULL;        
     }
    if(head == NULL){
        //logger::log(FATAL_ERROR,"processList", "process list head pointer is not set! " );
        return NULL;
    }
    process_t* temp = head->next;
    list_for_each(temp){
            if(temp->pid == pid)
                return temp;
    }
    return NULL;
}

/*bool processList::changeStatus(pid_t pid, pstatus_t status)
{
    process_t* temp = searchList(pid);
    if(temp == NULL){
        //logger::log(ERROR,"processList::changeStatus", "cannot find the specified process! " );
        return false;
    }
    else{
        temp->pstatus = status;
        return true;
    }
}*/

bool processList::changeConf(pid_t pid, string& conf)
{
    process_t* temp = searchList(pid);
    if(temp == NULL){
        //logger::log(ERROR,"processList::changeConf", "cannot find the specified process! " );
        return false;
    }
    else{
        temp->conf = conf;
        return true;
    }
}

bool processList::changePath(pid_t pid, string& path)
{
    process_t* temp = searchList(pid);
    if(temp == NULL){
        //logger::log(ERROR,"processList::changePath", "cannot find the specified process! " );
        return false;
    }
    else{
        temp->path = path;
        return true;
    }
}

int processList::getCurrentNum()
{
    return currentNum;
}

processList::~processList()
{
    process_t* temp = head;
    list_for_each(temp){
        process_t* a = temp->next;
        delete temp;
        temp = a;
    }
}

process_t* processList::getHead()
{
    return head->next;
}