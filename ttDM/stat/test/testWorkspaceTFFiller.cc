#include "ttDM/stat/interface/WorkspaceTFFiller.h"
#include "ttDM/stat/interface/WorkspaceRooContainer.h"
#include "ttDM/stat/interface/WorkspaceCategory.h"
#include "ttDM/stat/interface/DataMCHistoCollection.h"
#include "ttDM/stat/interface/Config.h"
#include "ttDM/stat/test/makeConfig.h"
#include "ttDM/stat/test/check.h"
#include <string>
using namespace std;

bool testWorkspaceTFFiller() {
  bool pass = true;
  
  shared_ptr<Config> cfg = makeConfigMCStat();
  DataMCHistoCollection hc(0);
  hc.fill();
  WorkspaceRooContainer container;
  WorkspaceTFFiller tf(0, hc, container);

  bool exception = false;
  exception = false;
  try {
    tf.fill();
  } catch(runtime_error & er) {
    cout << er.what() << endl;
    exception = true;
  }
  pass &= check(!exception, "don't throw when filling");

  return pass;
}
