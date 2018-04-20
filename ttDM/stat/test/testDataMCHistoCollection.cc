#include "ttDM/stat/interface/DataMCHistoCollection.h"
#include "ttDM/stat/test/makeConfig.h"
#include "ttDM/stat/test/check.h"
using namespace std;

bool testDataMCHistoCollection() {
  bool pass = true;
  shared_ptr<Config> cfg = makeConfigMCStatComplete();
  DataMCHistoCollection hc(0);
  pass &= check(&hc.data() != 0, "correct data");
  bool exception = false;
  try {
    hc.fill();
  } catch (runtime_error& ex) {
    cerr << ex.what() << endl;
    exception = true;
  }
  pass &= check(!exception, "correctly filled");

  return pass;
}
