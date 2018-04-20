#include "ttDM/stat/interface/HistoSetMCStat.h"
#include "ttDM/stat/interface/Config.h"
using namespace std;

const string HistoSetMCStat::variableName() const {
  if(systName_ == "ERROR") throw runtime_error(string("uninitialized HistoSetMCStat"));
  string ret = Config::get().variableName(categoryIndex_) + "_" + systName_;
  if(upOrDown_ == Shift::Up) ret += "_Up";
  else ret += "_Down";
  return ret;
}

