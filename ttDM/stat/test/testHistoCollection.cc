#include "ttDM/stat/interface/HistoCollection.h"
#include "ttDM/stat/test/makeConfig.h"
#include "ttDM/stat/test/check.h"
#include <memory>
using namespace std;

bool testHistoCollection() {
  bool pass = true;
  shared_ptr<Config> cfg(makeConfigMCStat());
  HistoCollection hc(0);
  pass &= check(&hc.nominal() != 0, "correct nominal");
  pass &= check(hc.numSystematics() == cfg->numSystematics(), "correct num. of systematics");
  pass &= check(&hc.up(0) != 0, "correct up shift");
  pass &= check(&hc.down(0) != 0, "correct down shift");
  bool exception = false;
  try {
    hc.fill("TT");
  } catch (runtime_error& ex) {
    exception = true;
  }
  pass &= check(!exception, "correctly filled");
  return pass;
}
