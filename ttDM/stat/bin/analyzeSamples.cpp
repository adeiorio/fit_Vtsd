#include "ttDM/stat/interface/Config.h"
#include "ttDM/stat/interface/DataMCHistoCollection.h"
#include "ttDM/stat/interface/Datacard.h"
#include "ttDM/stat/interface/Workspace.h"
#include <memory>
using namespace std;

int main(int argc, char *argv[]) {
  Config cfg(argc, argv);
  if(cfg.verbose() > 0) cout << "*** reading configuration file " << argv[2] << " ***" << endl;
  vector<shared_ptr<DataMCHistoCollection> > hc;
  if(cfg.verbose() > 0) cout << "*** importing histograms ***" << endl;
  for(unsigned int category = 0; category < cfg.numCategories(); ++category) { 
    if(cfg.verbose() > 0) cout << "*** importing samples for category " << cfg.category(category) << endl;
    hc.push_back(make_shared<DataMCHistoCollection>(category));
    hc.back()->verbose(cfg.verbose());
    hc.back()->fill();
  }
  Workspace w(hc);
  w.verbose(cfg.verbose());
  w.fill();
  w.save();
  Datacard datacard(hc);  
  datacard.verbose(cfg.verbose());
  datacard.save();
  return 0;
}

