#include "ttDM/stat/interface/WorkspaceSystBkgFiller.h"
#include "ttDM/stat/interface/WorkspaceRooContainer.h"
#include "ttDM/stat/interface/WorkspaceCategory.h"
#include "ttDM/stat/interface/WorkspaceTFFiller.h"
#include "ttDM/stat/interface/DataMCHistoCollection.h"
#include "ttDM/stat/interface/Config.h"
#include "ttDM/stat/test/makeConfig.h"
#include "ttDM/stat/test/check.h"
#include <string>
using namespace std;

bool testWorkspaceSystBkgFiller() {
  bool pass = true;
  
  shared_ptr<Config> cfg = makeConfigMCStat();
  DataMCHistoCollection hc(0);;
  hc.fill();
  WorkspaceRooContainer container;
  shared_ptr<WorkspaceTFFiller> tf = make_shared<WorkspaceTFFiller>(0, hc, container);
  WorkspaceSystBkgFiller sf(container, hc.background(0), hc.data(), tf, 0,
			    cfg->backgroundSample(0).name, cfg->backgroundSample(0).description);
  tf->fill();
  bool exception = false;
  exception = false;
  try {
    sf.fill();
  } catch(runtime_error & er) {
    cout << er.what() << endl;
    exception = true;
  }
  check(!exception, "don't throw when filling (don't worry about above error message)");

  return pass;
}
