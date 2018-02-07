#include "ttDM/stat/interface/HistoSetSyst.h"
#include "ttDM/stat/interface/Config.h"
#include "ttDM/stat/interface/stringUtils.h"
#include <iostream>
using namespace std;

string HistoSetSyst::histoName(const string & ctrlRegion) const {
  string systName = Config::get().systematic(systIndex_);
  if(beginsWith(systName, "mcstat_")) systName += "_";
  string postfix = "_" + systName + (upOrDown_ == Shift::Up ? "Up" : "Down");
  string prefix = variableName() + "_" + ctrlRegion;
  return prefix + postfix;
}

