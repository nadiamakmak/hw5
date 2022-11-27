

#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please
#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
bool createSchedule(DailySchedule& schedule, const AvailabilityMatrix& available, const size_t maxShifts, const size_t dailyNeed); //function to handle the recursion
bool isOverworked(unsigned int& worker, DailySchedule& schedule, const size_t maxShifts); //function to see if a worker should no longer be scheduled
Worker_T selectWorker(int& today, DailySchedule& schedule); //function to select which worker to attempt to schedule


// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    // Add your code below

    //emplace_back() used because push_back() wasn't working?? used stack overflow
    sched.emplace_back(); //create a spot for the new day
    //sched.push_back(new vector<worker_T>);

    if(createSchedule(sched, avail, maxShifts, dailyNeed)==true){
        return true;
    }
    return false;

}

bool createSchedule(DailySchedule& schedule, const AvailabilityMatrix& available, const size_t maxShifts, const size_t dailyNeed){
    unsigned int daysToSchedule = available.size();
    unsigned int scheduledDays = schedule.size();

    if(scheduledDays>daysToSchedule){ //check to see if we have finished building our schedule, if so, remove the extra added day
        schedule.pop_back();
        return true;
    }

    int today = schedule.size()-1; //get the last element in the schedule, aka the day we will be creating the schedule for

    Worker_T currWorker = selectWorker(today, schedule); //select the worker for which we will now be trying to schedule

    while(currWorker<available[today].size()){ //while we haven't looked through all the workers
        //if the current worker is not available today OR is already working the alotted amount of shifts
        if(available[today][currWorker] == 0 || isOverworked(currWorker, schedule, maxShifts)==true){ 
            currWorker++; //move on to the next worker (go back to start of the loop)
            continue;
        }

        schedule[today].push_back(currWorker); //otherwise, we add them to the schedule to work today

        if(schedule[today].size()==dailyNeed){ //if we have scheduled the amount of people we need for today
            schedule.emplace_back(); //add a new day

            if(createSchedule(schedule, available, maxShifts, dailyNeed)==true){ //start filling in the next spot
                return true;
            }

            else{ //if filling in the next spot doesn't work, remove the day we just added
                schedule.pop_back();
            }
        }

        else if(createSchedule(schedule, available, maxShifts, dailyNeed)==true){ //otherwise, don't add a new day, just start filling in the next spot
            return true;
        }
        
        //getting here meant we had to backtrace in one of the previous statements
        schedule[today].pop_back(); //remove the worker for which scheduling didn't work for, and increment to try with the next worker
        currWorker++;
    }
    return false; //technically, should never get here unless there is no solution found 
}

bool isOverworked(unsigned int& worker, DailySchedule& schedule, const size_t maxShifts){ //function to see if a worker should no longer be scheduled
    int shiftCount = 0;
    for(unsigned int i=0; i<schedule.size(); i++){ //iterate through the entire schedule and count how many shifts the worker is scheduled for 
        for(unsigned int j=0; j<schedule[i].size(); j++){
            if(schedule[i][j]==worker){
                shiftCount++;
            }
        }
    }

    if(shiftCount>=(int)maxShifts){ //if they are working the max shifts or above, they can no longer work anymore
        return true;
    }
    return false;
}

Worker_T selectWorker(int& today, DailySchedule& schedule){ //function to select which worker to attempt to schedule
    if(schedule[today].size()==0){ //if this is the first shift we're adding to the day, start by looking at worker 0
        return 0;
    }
    
    //if this isn't the first shift, we're gonna look at the next worker from which one we were last looking at
    return schedule[schedule.size()-1].back()+1; 
   
}