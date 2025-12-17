// COMP70088 – Q9: Producer–Consumer with timeouts (C++20)
// -------------------------------------------------------
//
// Build:
//    g++ -std=c++20 -O2 -pthread main.cpp -o pc_q9
//
// Example usage:
//   ./pc_q9 -q 1 -j 1 -P 1 -C 1
//   ./pc_q9 --queue 1 --jobs 1 --producers 1 --consumers 1

#include <chrono>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <vector>
#include <semaphore>
#include <queue>
#include <random>
#include <cstdlib>

using namespace std;
using namespace std::chrono;

constexpr ptrdiff_t MAX_BUFFER = 1024;

queue<int> g_queue;
mutex g_queue_mtx;

counting_semaphore<MAX_BUFFER> emptySlots(MAX_BUFFER);
counting_semaphore<MAX_BUFFER> filledSlots(0);

// ----------------------------- Logging helpers -----------------------------
static mutex g_log_mtx;

static string now_ts() {
    auto tp = system_clock::now();
    auto s  = time_point_cast<milliseconds>(tp);
    auto ms = s.time_since_epoch().count();
    // Render seconds.milliseconds as integer.mmm
    long long sec = ms / 1000LL;
    long long mmm = ms % 1000LL;
    ostringstream oss; oss << sec << '.' << setfill('0') << setw(3) << mmm; return oss.str();
}

static void logf(string_view tag, const string &msg) {
    lock_guard<mutex> lk(g_log_mtx);
    ostringstream oss;
    oss << '[' << now_ts() << "][" << tag << "] T#" << this_thread::get_id() << ' ' << msg << '\n';
    cerr << oss.str();
}

// --------------------------- Producer & Consumer ----------------------------

void producer(int id, int jobs_per_producer) {
    // Tag for logging
    string tag = "PROD#" + to_string(id);

    // Produce the specified number of jobs
    for (int job = 1; job <= jobs_per_producer; ++job) {
        // Generate a random job time between 1 and 100 seconds
        int item = (rand() % 10) + 1;

        // Wait for an empty slot, set timeout after 10s
        if (!emptySlots.try_acquire_for(10s)) {
            logf(tag, "Timeout waiting for empty slot - quitting with "
                      + to_string(jobs_per_producer - job) + " jobs left");
            return;
        }

        // Add the item to the queue, critical section
        {
            lock_guard<mutex> lock(g_queue_mtx);
            g_queue.push(item);
            logf(tag, "Produced job " + to_string(job)
                + ", sleep for " + to_string(item) + "s. "
                + to_string(g_queue.size()) + " jobs in queue.");
        }

        // Signal that there is a new filled slot
        filledSlots.release();  
    }

    logf(tag, "Finished producing all jobs");
}

void consumer(int id) {
    // Tag for logging
    string tag = "CONS#" + to_string(id);

    // Continuously consume jobs
    while (true) {
        // Wait for a filled slot, set timeout after 5s
        if (!filledSlots.try_acquire_for(5s)) {
            logf(tag, "Timeout waiting for job - quitting");
            return;
        }

        // Consume an item from the queue, critical section
        int item = 0;
        {
            lock_guard<mutex> lock(g_queue_mtx);
            item = g_queue.front();
            g_queue.pop();
            logf(tag, "Consumed a job, sleep for " + to_string(item)
                + "s. " + to_string(g_queue.size()) + " jobs in queue.");
        }

        // Signal that there is a new empty slot
        emptySlots.release();

        // “Processing” the job, sleep for item seconds
        this_thread::sleep_for(seconds(item));
    }
}

// ----------------------------------- Main -----------------------------------
int main(int argc, char** argv) {
    logf("MAIN", string("Program Started!"));

    // Initialize variables for command line arguments
    int q = -1, j = -1, p = -1, c = -1;

    // Parse command line arguments
    for (int i = 1; i + 1 < argc; i += 2) {
        string opt = argv[i];
        string val = argv[i + 1];

        if (opt == "-q" || opt == "--queue") {
            q = stoi(val);
        } else if (opt == "-j" || opt == "--jobs") {
            j = stoi(val);
        } else if (opt == "-P" || opt == "--producers") {
            p = stoi(val);
        } else if (opt == "-C" || opt == "--consumers") {
            c = stoi(val);
        } else { return 1; }
    }
    
    // Validate arguments
    if(q <= 0 || j < 0 || p <= 0 || c <= 0) { return 1; }
    if(q > MAX_BUFFER) { return 1; }

    // Adjust the semaphore for the specified queue size
    emptySlots.release(q);

    // Create producer and consumer threads
    vector<thread> prod_threads;
    vector<thread> cons_threads;

    // Start producer and consumer threads
    for(int i = 0; i < p; i++) {
        prod_threads.emplace_back(producer, i, j);
    }
    for(int i = 0; i < c; i++) {
        cons_threads.emplace_back(consumer, i);
    }
    
    // Wait for all threads to finish
    for(auto &t : prod_threads) { t.join(); }
    for(auto &t : cons_threads) { t.join(); }
    
    logf("MAIN", string("Program Completed!"));
   
    return 0;
}
