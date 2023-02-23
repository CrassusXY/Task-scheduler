#ifndef SCHEDULE_HH
#define SCHEDULE_HH

#include <vector>
#include <string>
#include <algorithm>
#include <bits/stdc++.h>
#include "processor.hh"


class scheduler
{
    private:
    int strategy;
    int quantum;
    int tick;

    std::vector<core> processor;
    std::vector<process> tasks;

    void strategy_manager();
    void swap_cores_tasks();
    void fcfs();
    void sjf();
    void srtf();
    void rr();
    void pfcfszw();
    void psrtfzw();
    void pfcfsbw();

    public:
    scheduler(int strategy_, int cores_, int quantum_):
        strategy(strategy_),
        quantum(quantum_),
        tick(0)
        {
            for (int i = 0; i < cores_; ++i){
                processor.emplace_back(core());
            }
        }

    void processor_state();
    void run();
};

//prints state of all cores
void scheduler::processor_state(){
    std::cout << tick;
    for (size_t i = 0; i < processor.size(); ++i){
        std::cout << " " << processor[i].get_id() << "/" << processor[i].task.exec_time;
    }
    std::cout << std::endl;
}

void scheduler::run(){
    int tmp_time, tmp_id, tmp_priority, tmp_exec_time;
    bool all_clear = false;
    std::string line;

    //petla na ladowanie nowych procesow
    while(std::cin >> tmp_time){
        std::getline(std::cin, line);
        std::stringstream tmpss(line);

        while(tmpss >> tmp_id >> tmp_priority >> tmp_exec_time){
            tasks.push_back(process(tmp_id, tmp_priority, tmp_exec_time));
        }
        strategy_manager();
    }

    //petla na dokonczenie zadan
    while(1){
        strategy_manager();
        if(tasks.empty()){
            all_clear = true;
        }
        
        for (size_t i = 0; i < processor.size(); ++i){
            if (processor[i].busy){
                all_clear = false;
            }
        }

        if(all_clear)
            break;
    }

    //wypisanie ostatniego stanu procesora
    processor_state();
}


void scheduler::swap_cores_tasks(){
    for (size_t i = 0; i < processor.size(); ++i){
        if(!processor[i].busy){
            for (size_t j = i+1; j < processor.size(); ++j){
                if(processor[j].busy == true){
                    processor[i].attach(processor[j].detach());
                }
            }
            
        }
    }
}

//scheduler algorithm function
void scheduler::strategy_manager(){
    //Jesli rdzenie o nizszym id sa wolne, to przesuwam na nie zadania z tych o wyzszym id
    swap_cores_tasks();

    switch (strategy){
    case 0:
        fcfs();
        break;
    case 1:
        sjf();
        break;
    case 2:
        srtf();
        break;
    case 3:
        rr();
        break;
    case 4:
        pfcfszw();
        break;
    case 5:
        psrtfzw();
        break;
    case 6:
        pfcfsbw();
        break;
    default:
        break;
    }

    //wyswietlam stan procesora
    processor_state();

    //tick dla kazdego procesora
    for (size_t i = 0; i < processor.size(); ++i){
        processor[i].tick();
    }
    tick++;
}

//First come first served
void scheduler::fcfs(){
    for (size_t i = 0; i < processor.size(); ++i){
        if (!processor[i].busy && !tasks.empty()){
            processor[i].attach(tasks[0]);
            tasks.erase(tasks.begin());
        }
    }
}

//shortest jobtime first
void scheduler::sjf(){

    std::sort(tasks.begin(), tasks.end(), [](const process& l, const process& r) -> bool
    {
        return l.exec_time < r.exec_time;
    });

    for (size_t i = 0; i < processor.size(); ++i){
        if (!processor[i].busy && !tasks.empty()){
            processor[i].attach(tasks[0]);
            tasks.erase(tasks.begin());
        }
    }
}

//shortest remaining jobtime first
void scheduler::srtf(){
    //wywlaszczanie
    if(tick%quantum == 0){
        for (size_t i = 0; i < processor.size(); ++i){
            if(processor[i].busy)
                tasks.push_back(processor[i].detach());
        }
    }

    std::sort(tasks.begin(), tasks.end(), [](const process& l, const process& r) -> bool
    {
        return l.exec_time < r.exec_time;
    });

    for (size_t i = 0; i < processor.size(); ++i){
        if (!processor[i].busy && !tasks.empty()){
            processor[i].attach(tasks[0]);
            tasks.erase(tasks.begin());
        }
    }
}

//round robin
void scheduler::rr(){
    //wywlaszczanie
    if(tick%quantum == 0){
        for (size_t i = 0; i < processor.size(); ++i){
            if(processor[i].busy)
                tasks.push_back(processor[i].detach());
        }
    }
    

    for (size_t i = 0; i < processor.size(); ++i){
        if (!processor[i].busy && !tasks.empty()){
            processor[i].attach(tasks[0]);
            tasks.erase(tasks.begin());
        }
    }
}

//priority sort with fcfs for equal priorities
void scheduler::pfcfszw(){
    //wywlaszczanie
    if(tick%quantum == 0){
        for (size_t i = 0; i < processor.size(); ++i){
            if(processor[i].busy)
                tasks.push_back(processor[i].detach());
        }
    }

    std::sort(tasks.begin(), tasks.end(), [](const process& l, const process& r) -> bool
    {
        return l.priority < r.priority;
    });

    for (size_t i = 0; i < processor.size(); ++i){
        if (!processor[i].busy && !tasks.empty()){
            processor[i].attach(tasks[0]);
            tasks.erase(tasks.begin());
        }
    }
}

//priority sort with srtf for equal priorities
void scheduler::psrtfzw(){
    //wywlaszczanie
    if(tick%quantum == 0){
        for (size_t i = 0; i < processor.size(); ++i){
            if(processor[i].busy)
                tasks.push_back(processor[i].detach());
        }
    }

    std::sort(tasks.begin(), tasks.end(), [](const process& l, const process& r) -> bool
    {
        if (l.priority == r.priority)
            return l.exec_time < r.exec_time;
        
        return l.priority < r.priority;
    });

    for (size_t i = 0; i < processor.size(); ++i){
        if (!processor[i].busy && !tasks.empty()){
            processor[i].attach(tasks[0]);
            tasks.erase(tasks.begin());
        }
    }
}

//priority sort with fcfs for equal priorities
void scheduler::pfcfsbw(){
    std::sort(tasks.begin(), tasks.end(), [](const process& l, const process& r) -> bool
    {
        return l.priority < r.priority;
    });

    for (size_t i = 0; i < processor.size(); ++i){
        if (!processor[i].busy && !tasks.empty()){
            processor[i].attach(tasks[0]);
            tasks.erase(tasks.begin());
        }
    }
}



#endif