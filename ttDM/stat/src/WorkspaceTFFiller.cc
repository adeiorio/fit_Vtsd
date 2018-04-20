#include "ttDM/stat/interface/WorkspaceTFFiller.h"
#include "ttDM/stat/interface/WorkspaceTransferFactors.h"
#include "ttDM/stat/interface/Config.h"
#include "ttDM/stat/interface/DataMCHistoCollection.h"
using namespace std;

WorkspaceTFFiller::WorkspaceTFFiller(unsigned int category,
				     const DataMCHistoCollection& datamc,
				     WorkspaceRooContainer & roo) : verbose_(0) {
  const Config & cfg = Config::get();
  nCtrl_ = cfg.numControlRegions(category);
  for(unsigned int ctrl = 0; ctrl < nCtrl_; ++ctrl) {
    const string ctrlSample = cfg.controlRegion(category, ctrl).sample;
    unsigned int sampleIndex = cfg.findBackgroundSample(ctrlSample);
    const HistoCollection & histos = datamc.background(sampleIndex);
    auto ptr = make_shared<WorkspaceTransferFactors>(roo, histos, category, ctrl);
    transferFactors_.push_back(ptr);
  }
}

void WorkspaceTFFiller::fill() {
  for(unsigned int ctrl = 0; ctrl < nCtrl_; ++ctrl) {
    transferFactors_[ctrl]->fill();
  }
}
