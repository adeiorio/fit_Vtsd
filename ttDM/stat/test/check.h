#ifndef STAT_CHECK_H
#define STAT_CHECK_H
#include <iostream>

inline bool check(bool cond, const char * msg) {
  using namespace std;
  if(cond) cout << "+++ pass: " << msg << endl;
  else cerr << "*** fail: " << msg << endl;
  return cond;
}

#endif
