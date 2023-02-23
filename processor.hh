#ifndef PROCESSOR_HH
#define PROCESSOR_HH

#include <vector>

struct process
{
    int id;
    int priority;
    int exec_time;


    process():
        id{-1}, priority{-1}, exec_time{-1}
    {}
    process(int id_, int priority_, int exec_time_):
        id{id_}, priority{priority_}, exec_time{exec_time_}
    {}
};


struct core
{
    private:
    

    public:
    process task;
    bool busy;
    
    core():
        task{process()}, busy{false}
    {}

    void attach(process task_);
    process detach();
    void tick();
    int get_id();
};


void core::attach(process task_){
    busy = true;
    task = task_;
}

process core::detach(){
    process tmp = task;
    busy = false;
    task = process();
    return tmp;
}

void core::tick(){
    task.exec_time--;
    if(task.exec_time == 0){
        detach();
    }
        
}

int core::get_id(){
    return task.id;
}


#endif