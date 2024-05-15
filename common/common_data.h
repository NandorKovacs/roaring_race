#ifndef ROARING_RACE_COMMON_DATA
#define ROARING_RACE_COMMON_DATA
#include <functional>
#include <mutex>
#include <utility>

template <typename T>
class MutexVar {
 public:
  MutexVar(T data) : data{data} {}
  MutexVar() { data = T(); }

  void set(T new_data) {
    mut.lock();
    data = new_data;
    mut.unlock();
  }

  T get() {
    mut.lock();
    T new_data = data;
    mut.unlock();
    return new_data;
  }

  template <typename U>
  U exec_with(std::function<U(T &)> *func) {
    mut.lock();
    U res = *func(data);
    mut.unlock();
    return res;
  }

 protected:
  T data;
  std::mutex mut;
};

struct CarData {
  std::pair<float, float> position{0, 0};
  float angle = 0;
  float wheel_angle[2] = {0, 0};
};

#endif