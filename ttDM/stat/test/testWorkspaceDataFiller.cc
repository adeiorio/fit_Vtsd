#include "ttDM/stat/interface/WorkspaceDataFiller.h"
#include "ttDM/stat/interface/WorkspaceCategory.h"
#include "ttDM/stat/interface/DataMCHistoCollection.h"
#include "ttDM/stat/interface/WorkspaceRooContainer.h"
#include "ttDM/stat/test/makeConfig.h"
#include "ttDM/stat/test/check.h"
#include <string>
using namespace std;

bool testWorkspaceDataFiller() {
  bool pass = true;
  shared_ptr<Config> cfg = makeConfigMCStat();
  DataMCHistoCollection hc(0);
  hc.fill();
  WorkspaceRooContainer container;
  WorkspaceDataFiller df(container,  hc.data(), 0, cfg->numControlRegions(0));
  bool exception = false;
  try {
    df.fill();
  } catch(runtime_error & er) {
    cout << er.what() << endl;
    exception = true;
  }
  pass &= check(!exception, "don't thorw when filling");

  return pass;
}
