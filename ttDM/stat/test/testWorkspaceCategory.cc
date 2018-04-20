#include "ttDM/stat/interface/WorkspaceCategory.h"
#include "ttDM/stat/interface/WorkspaceRooContainer.h"
#include "ttDM/stat/interface/DataMCHistoCollection.h"
#include "ttDM/stat/interface/Config.h"
#include "ttDM/stat/test/check.h"
#include "ttDM/stat/test/makeConfig.h"
#include "RooWorkspace.h"
using namespace std;
using namespace RooFit;

bool testWorkspaceCategory() {
  bool pass = true;
  shared_ptr<Config> cfg = makeConfigMCStat();
  DataMCHistoCollection hc(0);
  hc.fill();
  WorkspaceRooContainer container;
  WorkspaceCategory w(0, hc, container);

  bool exception = false;
  try {
    w.fill();
  } catch(runtime_error& er) {
    exception = true;
  }
  exception = false;
  pass &= check(!exception, "fill workspace");

  return pass;
}
