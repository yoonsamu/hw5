#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

#include "schedwork.h"

using namespace std;

static const Worker_T INVALID_ID = (unsigned int)-1;

// Function prototypes
bool complete(const AvailabilityMatrix& avail, 
  const size_t dailyNeed,
  const size_t maxShifts,
  const size_t day,
  const size_t num,
  DailySchedule& sched,
  vector<Worker_T>& numOfShifts);

bool schedule(
  const AvailabilityMatrix& avail,
  const size_t dailyNeed,
  const size_t maxShifts,
  DailySchedule& sched) {
    if (avail.empty()) return false;

    sched = DailySchedule(avail.size(), vector<Worker_T>(dailyNeed, INVALID_ID)); // Initialize the schedule
    vector<Worker_T> numOfShifts(avail[0].size(), 0); // Initialize shift counts

    return complete(avail, dailyNeed, maxShifts, 0, 0, sched, numOfShifts);
}

bool complete(
    const AvailabilityMatrix& avail, 
    const size_t dailyNeed,
    const size_t maxShifts,
    const size_t day,
    const size_t num,
    DailySchedule& sched,
    vector<Worker_T>& numOfShifts
) {
    if (day == avail.size()) {
        return true; // All days scheduled successfully
    }

    for (size_t i = 0; i < avail[day].size(); i++) {
        if (avail[day][i] && numOfShifts[i] < maxShifts) {
            sched[day][num] = i;
            numOfShifts[i]++;

            bool status = (num + 1 == dailyNeed) ?
                          complete(avail, dailyNeed, maxShifts, day + 1, 0, sched, numOfShifts) :
                          complete(avail, dailyNeed, maxShifts, day, num + 1, sched, numOfShifts);

            if (status) return true;

            // Backtrack
            sched[day][num] = INVALID_ID;
            numOfShifts[i]--;
        }
    }
    return false; // Unable to schedule this day
}

