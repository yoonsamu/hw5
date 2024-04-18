#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#endif

#include "schedwork.h"
using namespace std;

static const Worker_T INVALID_ID = (unsigned int)-1;  // Constant for an invalid worker ID

// Prototype for the helper function used in recursive scheduling
bool complete(const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    const size_t day,
    const size_t num,
    DailySchedule& sched,
    vector<Worker_T>& numOfShifts);

// Schedules workers based on their availability
bool schedule(const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched) {
    if(avail.empty()) return false;  // Return false if availability matrix is empty

    sched.clear();  // Clear previous schedule if any

    // Initialize the schedule with placeholders for workers
    for (size_t i = 0; i < avail.size(); i++) {
        sched.push_back(vector<Worker_T>(dailyNeed, INVALID_ID));
    }

    // Track number of shifts per worker
    vector<Worker_T> numOfShifts(avail[0].size(), 0);

    // Begin recursive scheduling process
    return complete(avail, dailyNeed, maxShifts, 0, 0, sched, numOfShifts);
}

// Recursively tries to fill the schedule with available workers
bool complete(const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    const size_t day,
    const size_t num,
    DailySchedule& sched,
    vector<Worker_T>& numOfShifts) {
    if (day == avail.size()) {
        return true;  // Return true if all days are scheduled successfully
    }

    // Attempt to assign workers to each shift
    for (size_t i = 0; i < avail[day].size(); i++) {
        if (avail[day][i] && numOfShifts[i] < maxShifts) {
            sched[day][num] = i;  // Assign worker i to the shift
            numOfShifts[i]++;  // Increment shift count for worker i

            bool status;
            // Check if the day's needs are met and proceed accordingly
            if (num == dailyNeed - 1) {
                status = complete(avail, dailyNeed, maxShifts, day + 1, 0, sched, numOfShifts);
            } else {
                status = complete(avail, dailyNeed, maxShifts, day, num + 1, sched, numOfShifts);
            }

            if (status) return true;  // If scheduling is successful, return true

            // Backtrack if the current assignment doesn't lead to a solution
            sched[day][num] = INVALID_ID;
            numOfShifts[i]--;
        }
    }
    return false;  // Return false if no valid scheduling is possible for current configuration
}
