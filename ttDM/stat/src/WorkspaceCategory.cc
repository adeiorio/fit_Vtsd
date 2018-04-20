#include "ttDM/stat/interface/WorkspaceCategory.h"
#include "ttDM/stat/interface/WorkspaceDataFiller.h"
#include "ttDM/stat/interface/WorkspaceSystSigFiller.h"
#include "ttDM/stat/interface/WorkspaceSystBkgFiller.h"
#include "ttDM/stat/interface/WorkspaceTFFiller.h"
#include "ttDM/stat/interface/DataMCHistoCollection.h"
#include "ttDM/stat/interface/Config.h"
using namespace std;

WorkspaceCategory::WorkspaceCategory(unsigned int category,
				     const DataMCHistoCollection & datamc,
				     WorkspaceRooContainer & container) :
  verbose_(0) {
  const Config & cfg = Config::get();
  nSig_ = cfg.numSignalSamples();
  nBkg_ = cfg.numBackgroundSamples();
  nCtrl_ = cfg.numControlRegions(category);
  unsigned int nCtrl = cfg.numControlRegions(category);
  data_ = make_shared<WorkspaceDataFiller>(container, datamc.data(), category, nCtrl);
  for(unsigned int sig = 0; sig < nSig_; ++sig) {
    const Config::Sample & sample = cfg.signalSample(sig);
    signals_.push_back(make_shared<WorkspaceSystSigFiller>(container, datamc.signal(sig),
							   category, sample.name, sample.description));
  }
  transferFactors_ = make_shared<WorkspaceTFFiller>(category, datamc, container);
  for(unsigned int bkg = 0; bkg < nBkg_; ++bkg) {
    const Config::Sample & sample = cfg.backgroundSample(bkg);
    backgrounds_.push_back(make_shared<WorkspaceSystBkgFiller>(container, datamc.background(bkg),
							       datamc.data(), transferFactors_, 
							       category, sample.name, sample.description));
  }
}

void WorkspaceCategory::fill() {
  data_->fill();
  for(unsigned int sig = 0; sig < nSig_; ++sig) {
    if(verbose_> 0) cout << "*** fill workspace for signal " << Config::get().signalSample(sig).name << endl;
    signals_[sig]->fill();
  }
  if(verbose_> 0) cout << "*** fill transfer factors for control regions " << endl;  
  transferFactors_->fill();
  for(unsigned int bkg = 0; bkg < nBkg_; ++bkg) {
    if(verbose_> 0) cout << "*** fill workspace for background " << Config::get().backgroundSample(bkg).name << endl;
    backgrounds_[bkg]->fill();
  }
}

void WorkspaceCategory::setVerbose(int v) {
  verbose_ = v;
  for(unsigned int sig = 0; sig < nSig_; ++sig)
    signals_[sig]->setVerbose(v);
  transferFactors_->setVerbose(v);
  for(unsigned int bkg = 0; bkg < nBkg_; ++bkg)
    backgrounds_[bkg]->setVerbose(v);

}
