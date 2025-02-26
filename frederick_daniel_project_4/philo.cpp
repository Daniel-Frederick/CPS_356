// Adapted from Dijkstra's Solution in c++20
// https://en.wikipedia.org/wiki/Dining_philosophers_problem
#include <array>
#include <chrono>
#include <iostream>
#include <mutex>
#include <random>
#include <semaphore>
#include <string_view>
#include <thread>

constexpr int no_of_philosophers = 5;

enum class State {
  THINKING = 0,
  HUNGRY = 1,
  EATING = 2,
};

inline size_t left(size_t i) {
  return (i + no_of_philosophers - 1) % no_of_philosophers;
}

inline size_t right(size_t i) { return (i + 1) % no_of_philosophers; }

State state[no_of_philosophers];
std::mutex crit_region_mtx;
std::mutex output_mtx;

// Create an array of philosopher names.
const std::array<std::string_view, no_of_philosophers> philosopher_names = {
    "Aristotle", "Platon", "Descartes", "Kant", "Nietzsche"};

std::binary_semaphore both_forks_available[no_of_philosophers]{
    std::binary_semaphore(0), std::binary_semaphore(0),
    std::binary_semaphore(0), std::binary_semaphore(0),
    std::binary_semaphore(0),
};

void test(size_t i) {
  if (state[i] == State::HUNGRY && state[left(i)] != State::EATING &&
      state[right(i)] != State::EATING) {
    state[i] = State::EATING;
    both_forks_available[i].release();
  }
}

void think(size_t i) {
  {
    std::lock_guard<std::mutex> lk(output_mtx);
    std::cout << philosopher_names[i] << " is thinking" << std::endl;
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(6));
}

void take_forks(size_t i) {
  {
    std::lock_guard<std::mutex> lk(crit_region_mtx);
    state[i] = State::HUNGRY;
    {
      std::lock_guard<std::mutex> lk(output_mtx);
      std::cout << "\t\t" << philosopher_names[i] << " is HUNGRY" << std::endl;
    }
    test(i);
  }
  both_forks_available[i].acquire();
}

void eat(size_t i) {
  {
    std::lock_guard<std::mutex> lk(output_mtx);
    std::cout << "\t\t\t\t" << philosopher_names[i] << " is EATING"
              << std::endl;
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(6));
}

void put_forks(size_t i) {
  std::lock_guard<std::mutex> lk(crit_region_mtx);
  state[i] = State::THINKING;
  test(left(i));
  test(right(i));
}

void philosopher(size_t i, std::stop_token stopToken) {
  while (!stopToken.stop_requested()) {
    think(i);
    take_forks(i);
    eat(i);
    put_forks(i);
  }
}

int main() {
  std::cout << "Dinner Started!" << std::endl;
  std::jthread t0([&](std::stop_token st) { philosopher(0, st); });
  std::jthread t1([&](std::stop_token st) { philosopher(1, st); });
  std::jthread t2([&](std::stop_token st) { philosopher(2, st); });
  std::jthread t3([&](std::stop_token st) { philosopher(3, st); });
  std::jthread t4([&](std::stop_token st) { philosopher(4, st); });

  std::this_thread::sleep_for(std::chrono::seconds(5));

  std::cout << "Dinner Done!" << std::endl;
  return 0;
}
