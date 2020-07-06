//
//  main.cpp
//  Logger
//
//  Created by Subham Mallick on 07/07/20.
//  Copyright © 2020 Subham Mallick. All rights reserved.
//
#include <iostream>
#include <queue>
#include <unordered_map>
#include <ctime>
#include <chrono>
#include <thread>

#define GETSET(TYPE,VAR) \
TYPE get##VAR() { return VAR; } \
void set##VAR(TYPE newVal) { VAR = newVal; }

using namespace std;

class Log{
public:
    virtual void start(string)=0;
    virtual void end(string)=0;
    virtual void poll()=0;
};

// O(1)
long getCurrentTime(){
    std::chrono::milliseconds timespan(1605); // or whatever
    std::this_thread::sleep_for(timespan);
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    
    
}

class Process{
    string id;
    long startTime;
    long endTime;
public:
    Process(string newProcessId){
        id = newProcessId;
    }
    GETSET(string,id)
    GETSET(long,startTime)
    GETSET(long,endTime)
};

class LogImplemented:public Log{
    
    priority_queue<pair<long,string>,vector<pair<long,string>>, greater<pair<long,string>> > start_process;
    
    unordered_map<string,long> process_end;
    
public:
    // O(logn)
    void start(string processId){
        Process newProcess(processId);
        start_process.push({getCurrentTime(),newProcess.getid()});
        process_end[newProcess.getid()] = -1;
    }
    // O(1)
    void end(string processId){
        process_end[processId] = getCurrentTime();
    }
    // O(log n)
    void poll(){
        if(start_process.size()==0){
            cout<<"\nNothing to show";
            return;
        }
        auto oldestProcess = start_process.top();
        if( process_end[oldestProcess.second]==-1){
            cout<<"\nProcess not yet finished";
            return;
        }
        
        cout<<"\n{"<<oldestProcess.second<<"}"<<" started at {"<<oldestProcess.first<<"}"<<" and ended at {"<<process_end[oldestProcess.second]<<"}\n";
        
        start_process.pop();
        process_end.erase(oldestProcess.second);
        return;
        
    }
};



int main(int argc, const char * argv[]) {
    
    LogImplemented log;
    
    log.poll();
    log.start("3");
    log.poll();
    log.start("2");
    log.poll();
    log.start("1");
    log.poll();
    log.end("2");
    log.poll();
    log.end("1");
    log.poll();
    log.end("3");
    log.poll();
    log.poll();
    log.poll();
    
    return 0;
}
