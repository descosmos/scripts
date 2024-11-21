#include <string>
#include <chrono>
#include <ctime>

class ScopedTimer {
public:
    ScopedTimer(const std::string& name = "ScopedTimer")
        : name_(name), start_wall_time_(std::chrono::high_resolution_clock::now()) {
        clock_gettime(CLOCK_THREAD_CPUTIME_ID, &start_cpu_time_);
    }

    ~ScopedTimer() {
      if (count < 21) {
        ++count;
        auto end_wall_time = std::chrono::high_resolution_clock::now();
        struct timespec end_cpu_time;
        clock_gettime(CLOCK_THREAD_CPUTIME_ID, &end_cpu_time);

        // Calculate the wall and CPU durations in milliseconds
        double wall_duration = std::chrono::duration<double, std::milli>(end_wall_time - start_wall_time_).count();
        double cpu_duration = (end_cpu_time.tv_sec - start_cpu_time_.tv_sec) * 1000.0 +
                              (end_cpu_time.tv_nsec - start_cpu_time_.tv_nsec) / 1e6;

        sum_wall_duration += wall_duration;
        sum_cpu_duration += cpu_duration; 

        std::cout << "ScopedTimer Wall Time: " << wall_duration << " milliseconds\n";
        std::cout << "ScopedTimer Thread CPU Time: " << cpu_duration << " milliseconds\n";
      }
    }

public:
    std::string name_;
    std::chrono::high_resolution_clock::time_point start_wall_time_;
    struct timespec start_cpu_time_;
    static int count;
    static double sum_cpu_duration;
    static double sum_wall_duration;
};

int ScopedTimer::count = 0;
double ScopedTimer::sum_cpu_duration = 0.0;
double ScopedTimer::sum_wall_duration = 0.0;
