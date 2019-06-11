#pragma once

#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

#include <math.h>
#include <sys/time.h>
#include <time.h>

#define KEEP_UNUSED 0
#define KEEP_UNREACHABLE 0

static inline long getUS() {
  struct timeval tv;
  gettimeofday(&tv, nullptr);
  return ((long)tv.tv_sec) * 1000000 + ((long)tv.tv_usec);
}

#define getSEC(us) (((float)(us)) / 1000000)

class AccTime;
extern long INIT_TIME;
extern vector<AccTime *> accTimes;

#if 0
#define DEFINE_START_END long start, end
#define STEMP(x) x = getUS();
#define TIMEP_BOUND(module, start, end, bound)                                 \
  end = getUS();                                                               \
  if (end - start >= bound) {                                                  \
    cout << "TIME: " << setw(15);                                              \
    cout << getSEC(end - start) << "," << setw(15)                             \
         << fmod(getSEC(start - INIT_TIME), 10000) << "," << setw(15)          \
         << fmod(getSEC(end - INIT_TIME), 10000) << ",\t" << module << endl;   \
  }
#define TIMEP(module, start, end) TIMEP_BOUND(module, start, end, 1000000)
#define ACCTM(module, start, end)                                              \
  TIMEP_BOUND(#module, start, end, 1000000)                                    \
  static AccTime module##AccTime(#module);                                     \
  {                                                                            \
    long tmp = module##AccTime.getAccTime();                                   \
    module##AccTime.acc(end - start);                                          \
    if (module##AccTime.getAccTime() / 10000000 != tmp / 10000000) {           \
      cout << "TIME" << module##AccTime.to_string() << endl;                   \
    }                                                                          \
  }
#else
#define DEFINE_START_END
#define STEMP(x)
#define TIMEP(module, start, end)
#define ACCTM(module, start, end)
#endif

extern map<void *, void *> cache_analysis_induct_map_in_scope;
extern void invalidateCache();

class AccTime {
public:
  AccTime(string pInfo) {
    info = "[ACC TIME] " + pInfo;
    timeUS = 0;
    index = accTimes.size();
    accTimes.push_back(this);
  }

  void acc(long pTimeUS) {
    this->timeUS += pTimeUS;
    ++this->freq;
  }
  static int getIndex() { return AccTime::index; }
  // string to_string() {
  //  return info + ": " + std::to_string(getSEC(timeUS)) + "s (" +
  //         std::to_string(freq) + "times)";
  //}

  long getAccTime() { return this->timeUS; }

private:
  string info;
  long timeUS;
  long freq = 0;
  static int index;
};

#define LOG(level, title, x)                                                   \
  if ((level) == 0) {                                                          \
    cout << "[" << title << " ERROR][" << __func__ << ", " << __LINE__ << "]"  \
         << x << endl;                                                         \
  } else if ((level) == 1) {                                                   \
    cout << "[" << title << " WARNING][" << __func__ << ", " << __LINE__       \
         << "]" << x << endl;                                                  \
  } else if ((level) == 2) {                                                   \
    cout << "[" << title << " ALGO][" << __func__ << ", " << __LINE__ << "]"   \
         << x << endl;                                                         \
  } else if ((level) == 3) {                                                   \
    cout << "[" << title << " INFO][" << __func__ << ", " << __LINE__ << "]"   \
         << x << endl;                                                         \
  }
#if 1 // logs
#define PRESTP(title, x) LOG(2, title, x)
#else
#define PRESTP(title, x)
#endif

#if 0 // logs
#define INFOP(title, x) LOG(3, title, x)
#define ERRORP(title, x) LOG(0, title, x)
#define WARNINGP(title, x) LOG(1, title, x)
#define ALGOPP(title, x) LOG(2, title, x)
#define ALGOP(title, x) LOG(2, title, x)
#else
#define ERRORP(title, x)
#define WARNINGP(title, x)
#define ALGOPP(title, x)
#define INFOP(title, x)
#define ALGOP(title, x)
#endif

template <typename T> static string to_string(vector<T> vec) {
  stringstream ss;

  if (vec.size() == 0) {
    return "";
  }

  ss << vec[0];
  for (int i = 1; i < vec.size(); ++i) {
    ss << ", " << vec[i];
  }

  return ss.str();
}
