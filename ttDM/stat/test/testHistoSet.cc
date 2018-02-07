#include "ttDM/stat/interface/HistoSet.h"
#include "ttDM/stat/interface/stringUtils.h"
#include "ttDM/stat/test/makeConfig.h"
#include "ttDM/stat/test/check.h"
#include <iostream>
#include <memory>
using namespace std;

bool testHistoSet() {
  bool pass = true;
  shared_ptr<Config> cfg = makeConfig();
  HistoSet hs(0);
  try { 
    hs.getHisto("histos/Data_fullhadronic.root", "metFinal");
  } catch(runtime_error& er) {
    pass = false;
  }
  check(pass, "correctly found histogram");
  bool exception = false;
  try {
    hs.getHisto("histos/Data_fullhadronic.root", "pincopallino");
  } catch(runtime_error& er) {
    exception = beginsWith(er.what(), "can't find");
  }
  pass &= check(exception, "throw on wrong histogram name");
  exception = false;
  try {
    cout << "+++ An error message should appear below:" << endl;
    hs.getHisto("histos/pincopallino.root", "metFinal");
  } catch(runtime_error& er) {
    exception = beginsWith(er.what(), "can't open");
  }
  pass &= check(exception, "throw on wrong file name (don't worry of about error message)");
  exception = false;
  try {
    hs.fill("Data_semileptonic.root");
  } catch(runtime_error& er) {
    exception = true;
  }
  pass &= check(!exception, "correct fill from config.");

  pass &= check(hs.numControlRegions() == 2, "correct num. of control regions");
  pass &= check(&hs.signalRegion() != 0, "returns signal region");
  pass &= check(&hs.controlRegion(0) != 0, "returns control region");
  cout << hs.yieldNoCut(0) << endl;
  pass &= check(hs.yieldNoCut(0) == 32.0, "correct yield in sig. region");
  cout << hs.yieldNoCut(1) << endl;
  pass &= check(hs.yieldNoCut(1) == 242.0, "correct yield in ctrl. region");
  return pass;
}
