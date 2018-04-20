#include "ttDM/stat/interface/WorkspaceTransferFactors.h"
#include "ttDM/stat/interface/WorkspaceRooContainer.h"
#include "ttDM/stat/interface/DataMCHistoCollection.h"
#include "ttDM/stat/interface/Config.h"
#include "ttDM/stat/test/check.h"
#include "ttDM/stat/test/makeConfig.h"
#include <iostream>
using namespace std;

bool testWorkspaceTransferFactors() {
  bool pass = true;
  shared_ptr<Config> cfg = makeConfigMCStat();
  DataMCHistoCollection hc(0);
  hc.fill();
  WorkspaceRooContainer container;
  WorkspaceTransferFactors tf(container, hc.background(0), 0, 0);
  bool exception = false;
  try {
    tf.fill();
  } catch(runtime_error& ex) {
    cout << ex.what() << endl;
    exception = true;
  }
  pass &= check(!exception, "fill workspace");

  return pass;
}
