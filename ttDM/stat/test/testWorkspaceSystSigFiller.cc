#include "ttDM/stat/interface/WorkspaceSystSigFiller.h"
#include "ttDM/stat/interface/WorkspaceRooContainer.h"
#include "ttDM/stat/interface/DataMCHistoCollection.h"
#include "ttDM/stat/interface/WorkspaceCategory.h"
#include "ttDM/stat/interface/Config.h"
#include "ttDM/stat/test/makeConfig.h"
#include "ttDM/stat/test/check.h"
#include <string>
using namespace std;

bool testWorkspaceSystSigFiller() {
  bool pass = true;
  shared_ptr<Config> cfg = makeConfigMCStat();
  DataMCHistoCollection hc(0);
  hc.fill();
  WorkspaceRooContainer container;
  WorkspaceSystSigFiller sf(container, hc.signal(0), 0,
			    cfg->signalSample(0).name, cfg->signalSample(0).description);
  bool exception = false;
  exception = false;
  try {
    sf.fill();
  } catch(runtime_error & er) {
    cout << er.what() << endl;
    exception = true;
  }
  pass &= check(!exception, "don't thorw when filling");

  return pass;
}
