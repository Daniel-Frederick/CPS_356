// Adapted from:
// https://mariusbancila.ro/blog/2017/01/16/dining-philosophers-in-cpp11/
#include <array>
#include <atomic>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <random>
#include <string>
#include <string_view>
#include <thread>

constexpr int no_of_philosophers = 5;

struct fork {
  std::mutex mutex;
};

struct table {
  std::atomic<bool> ready{false};
  std::array<fork, no_of_philosophers> forks;
};

struct philosopher {
private:
  std::string const name;
  table const &dinnertable;
  fork &left_fork;
  fork &right_fork;
  std::thread lifethread;
  std::mt19937 rng{std::random_device{}()};

public:
  philosopher(std::string n, table const &t, fork &l, fork &r)
      : name(n), dinnertable(t), left_fork(l), right_fork(r),
        lifethread(&philosopher::dine, this) {}

  ~philosopher() { lifethread.join(); }

  void dine() {
    while (!dinnertable.ready)
      ;

    do {
      eat();
      think();
    } while (dinnertable.ready);
  }

  void print(std::string text) {
    // This mutex is used to serialize prints. Without this lock, the output
    // from multiple philosophers may become jumbled together.
    static std::mutex lockprint;

    std::lock_guard<std::mutex> cout_lock(lockprint);
    std::cout << std::left << std::setw(10) << std::setfill(' ') << name << text
              << std::endl;

    // cout_lock is automatically destructed here and releases lockprint
  }

  // This version of eat() differs from the correct solution provided in the
  // referenced article. This version almost always results in a deadlock
  // that causes all philosophers to starve. It is not guaranteed to produce
  // deadlock because there is still a race condition to acquire both forks
  // before a neighbor acquires one of the forks that are needed.
  void eat() {
    static thread_local std::uniform_int_distribution<> dist(1, 6);

    left_fork.mutex.lock();
    print(" acquired left fork");

    // This delay makes if very likely the race will be lost and another
    // philosopher will grab the right fork before this philosopher gets it.
    // Please note: the race condition still exists without this delay! The
    // probability of losing the race is reduced but not eliminated by
    // deleting the next line.
    std::this_thread::sleep_for(std::chrono::milliseconds(dist(rng) * 50));

    right_fork.mutex.lock();
    print(" acquired right fork");

    std::lock_guard<std::mutex> left_lock(left_fork.mutex, std::adopt_lock);
    std::lock_guard<std::mutex> right_lock(right_fork.mutex, std::adopt_lock);

    print(" started eating.");

    std::this_thread::sleep_for(std::chrono::milliseconds(dist(rng) * 50));

    print(" finished eating.\n");

    // left_lock and right_lock are automatically destructed here and release
    // their respective mutexes
  }

  void think() {
    static thread_local std::uniform_int_distribution<> wait(1, 6);
    std::this_thread::sleep_for(std::chrono::milliseconds(wait(rng) * 150));

    print(" is thinking ");
  }
};

void dine() {
  std::this_thread::sleep_for(std::chrono::seconds(1));
  std::cout << "Dinner started!" << std::endl;

  {
    table table;
    std::array<philosopher, no_of_philosophers> philosophers{{
        {"Aristotle", table, table.forks[0], table.forks[1]},
        {"Plato", table, table.forks[1], table.forks[2]},
        {"Descartes", table, table.forks[2], table.forks[3]},
        {"Kant", table, table.forks[3], table.forks[4]},
        {"Nietzsche", table, table.forks[4], table.forks[0]},
    }};

    table.ready = true;
    std::this_thread::sleep_for(std::chrono::seconds(5));
    table.ready = false;
  }

  std::cout << "Dinner done!" << std::endl;
}

int main() {
  dine();

  return 0;
}
