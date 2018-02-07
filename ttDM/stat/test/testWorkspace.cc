#include "ttDM/stat/interface/Workspace.h"
#include "ttDM/stat/interface/DataMCHistoCollection.h"
#include "ttDM/stat/interface/Config.h"
#include "ttDM/stat/test/check.h"
#include "ttDM/stat/test/makeConfig.h"
using namespace std;

bool testWorkspace() {
  bool pass = true;
  shared_ptr<Config> cfg = makeConfigMCStat();
  vector<shared_ptr<DataMCHistoCollection> > hc;
  hc.push_back(make_shared<DataMCHistoCollection>(0));
  hc.push_back(make_shared<DataMCHistoCollection>(1));
  hc[0]->fill();
  hc[1]->fill();
  Workspace w(hc);

  bool exception = false;
  try {
    w.fill();
  } catch(runtime_error& er) {
    exception = true;
  }
  exception = false;
  pass &= check(!exception, "fill workspace");
  try {
    w.save();
  } catch(runtime_error& er) {
    exception = true;
  }
  pass &= check(!exception, "save to file");

  return pass;
}
