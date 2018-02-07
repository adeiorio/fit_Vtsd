#include "ttDM/stat/interface/WorkspaceDataFiller.h"
#include "ttDM/stat/interface/HistoSet.h"
#include "ttDM/stat/interface/Config.h"
using namespace std;

WorkspaceDataFiller::WorkspaceDataFiller(WorkspaceRooContainer & container, const HistoSet & data,
					 unsigned int category, unsigned int nCtrl) :
  WorkspaceSample(category, container, "observed data sample"),
  data_(data), category_(category), nCtrl_(nCtrl) {
}

WorkspaceDataFiller::~WorkspaceDataFiller() {
}

void WorkspaceDataFiller::fill() {
  const Config & cfg = Config::get();
  string categoryName = cfg.category(category_);
  addHisto(("data_obs_" + categoryName).c_str(), data_.signalRegion());
  for(unsigned int ctrl = 0; ctrl < nCtrl_; ++ctrl) {
    const string ctrlName = cfg.controlRegion(category_, ctrl).name;
    addHisto("data_obs_" + categoryName + "_" + ctrlName, data_.controlRegion(ctrl));
  }
}
