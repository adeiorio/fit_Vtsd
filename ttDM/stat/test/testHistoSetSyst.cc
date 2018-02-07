#include "ttDM/stat/interface/HistoSetSyst.h"
#include "ttDM/stat/test/makeConfig.h"
#include "ttDM/stat/test/check.h"
#include <stdexcept>

using namespace std;

bool testHistoSetSyst() {
  bool pass = true;
  shared_ptr<Config> cfg = makeConfigMCStat();
  HistoSetSyst hs(0, 0, HistoSetSyst::Shift::Up);
  bool exception = false;
  try {
    hs.fill("TT_semileptonic.root");
  } catch(runtime_error& er) {
    cerr << er.what() << endl;
    exception = true;
  }
  pass &= check(!exception, "correct fill from config.");
  
  return pass;
}
