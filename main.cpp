#include <string.h>
#include <iostream>
#include <fstream>
#include "schedule.hh"

// 1->strategia, 2->liczba jaj, 3->kwant wywlaszczania
int main(int argc, char** argv) {
    int strategy;
    int cores = 1;
    int quantum = 1;

    if(argc > 4 || argc < 2){
        throw std::invalid_argument("Bledne wywolanie programu!\n");
    }

    strategy = std::stoi(argv[1]);
    if(strategy<0 || strategy > 6){
        throw std::invalid_argument("Bledne id szeregowania!\n");
    }


    if(argc == 3){
        cores = std::stoi(argv[2]);
    }
    else if(argc == 4){
        cores = std::stoi(argv[2]);
        quantum = std::stoi(argv[3]);
    }
        
    scheduler sched(strategy, cores, quantum);
    sched.run();
}