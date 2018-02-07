#include "ttDM/stat/interface/stringUtils.h"
#include "ttDM/stat/interface/skipSystematic.h"
#include "ttDM/stat/interface/Config.h"
#include <string>
#include <cstring>
using namespace std;

bool skipSystematic(const string & category, const string & syst, const string & bkgSample, const string & signalProcess) {
  bool skip = false;
  static const char * mcstat = "mcstat_";
  static const unsigned int mcstat_len = strlen(mcstat);
  if(beginsWith(syst, mcstat)) {
    skip = true;
    if(!(bkgSample.empty() || beginsWith(bkgSample, signalProcess))) {
      if(syst.find(category + "_" + bkgSample + "_bin") == mcstat_len) { skip = false; }
    } else {
      if(syst.find(category + "_" + signalProcess) != string::npos) { skip = false; }
    }
  }
  return skip;
}

bool skipSystematic(const string & category, const string & syst, const string & bkgSample) {
  const Config & cfg = Config::get();
  for(unsigned int i = 0; i < cfg.numSignalProcesses(); ++i) {
    if(skipSystematic(category, syst, bkgSample, cfg.signalProcess(i).name)) return true;
  }
  return false;
}
