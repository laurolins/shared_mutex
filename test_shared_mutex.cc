#include <iostream>
#include <iomanip>
#include <mutex>
#include <string>
#include <thread>
#include <chrono>
#include <random>
#include <atomic>

#include <boost/thread/shared_mutex.hpp>
//#include <boost/thread/unique_lock.hpp>
//#include <boost/thread/lock_types.hpp>

static bool done = false;

struct Nanocube {

    void write(int value) {
        ++write_count;
        data.push_back(value);
    }

    int read(int index) {
        ++read_count;
        return data[index % data.size()];
    }

    std::vector<int> data;

    mutable boost::shared_mutex mutex;

    std::atomic<uint64_t> read_count;
    std::atomic<uint64_t> write_count;

};

struct NanocubeAccess {
    NanocubeAccess(Nanocube &nanocube, std::string name):
        nanocube ( nanocube ),
        name ( name )
    {}
    Nanocube    &nanocube;
    std::string  name;
};

struct Writer: public NanocubeAccess {

    using NanocubeAccess::NanocubeAccess;

    void run() {
        int count = 1;
        while (!done) {
            {
                boost::unique_lock<boost::shared_mutex> lock(nanocube.mutex);
                for (int i=0;i<100;i++) {
                    nanocube.write(count++);
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }
};

struct Reader: public NanocubeAccess {
    using NanocubeAccess::NanocubeAccess;
    void run() {
        std::default_random_engine generator;
        std::uniform_int_distribution<int> distribution(1,100);
        while (!done) {
            {
                boost::shared_lock<boost::shared_mutex> lock(nanocube.mutex);
                nanocube.read(nanocube.data.size()-1);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(distribution(generator)));
        }
    }
};

struct Logger: public NanocubeAccess {
    using NanocubeAccess::NanocubeAccess;
    void run() {
        while (!done) {
            std::cout << "reads: "    << std::setw(10) << nanocube.read_count
                      << ", writes: " << std::setw(10) << nanocube.write_count
                      << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    }
};

int main() {

    Nanocube nanocube;

    nanocube.write(0);

    std::vector<std::thread> threads;

    threads.push_back(std::thread(&Writer::run, Writer(nanocube, "Writer")));

    // start writer thread
    for (int i=0;i<10;i++) {
        threads.push_back(std::thread(&Reader::run, Reader(nanocube, "Reader_" + std::to_string(i))));
    }

    threads.push_back(std::thread(&Logger::run, Logger(nanocube, "Logger")));


    char ch_done = 'x';
    while (!done) {
        std::cin >> ch_done;
        if (ch_done == 'q') {
            done = true;
        }
    }

    std::cout << "Joining threads" << std::endl;
    for (auto &th: threads) {
        th.join();
        std::cout << "thread done" << std::endl;
    }

    return 0;
}
