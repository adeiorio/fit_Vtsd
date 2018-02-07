#include "ttDM/stat/interface/WorkspaceSystSampleFiller.h"
#include "ttDM/stat/interface/Config.h"
#include "ttDM/stat/interface/WorkspaceRooContainer.h"
#include "ttDM/stat/interface/HistoCollection.h"
#include "ttDM/stat/interface/skipSystematic.h"
#include "RooWorkspace.h"
#include "RooDataHist.h"
#include "TH1.h"
#include <string>
#include <iostream>
using namespace std;
WorkspaceSystSampleFiller::WorkspaceSystSampleFiller(WorkspaceRooContainer & container, 
						     const HistoCollection& histos,
						     unsigned int category, const string & sampleName, 
						     const string & sampleDescr) :
  WorkspaceSample(category, container, sampleDescr),
  WorkspaceSystFiller(Config::get().numSystematics()),
  category_(category), sampleName_(sampleName), histos_(histos) {
  const Config & cfg = Config::get();
  categoryName_ = cfg.category(category_);
  nCtrl_ = cfg.numControlRegions(category_);
  histoName_ = sampleName_ + "_" + categoryName_;
}

WorkspaceSystSampleFiller::~WorkspaceSystSampleFiller() {
}

void WorkspaceSystSampleFiller::fillNominal() {
  addHisto(histoName_, histos_.nominal().signalRegion());
  for(unsigned int ctrl = 0; ctrl < nCtrl_; ++ctrl) {
    const string & controlRegionName = Config::get().controlRegion(category_, ctrl).name;
    addHisto(histoName_ + "_" + controlRegionName, histos_.nominal().controlRegion(ctrl));
  }
}

bool WorkspaceSystSampleFiller::skip(unsigned int syst) {
  return skipSystematic(categoryName_, Config::get().systematic(syst), sampleName_);
}

void WorkspaceSystSampleFiller::fillUp(unsigned int syst) {
  fillSyst(histos_.up(syst), "Up");
}

void WorkspaceSystSampleFiller::fillDown(unsigned int syst) {
  fillSyst(histos_.down(syst), "Down");
}

void WorkspaceSystSampleFiller::fillSyst(const HistoSet & histos, const string & upOrDown) {
  string name = histoName_ + "_" + systName_ + upOrDown;
  if(verbose_ > 1) cout << "*** add histogram " << name << endl; 
  addHisto(name, histos.signalRegion());
  for(unsigned int ctrl = 0; ctrl < nCtrl_; ++ctrl) {
    const string & controlRegionName = Config::get().controlRegion(category_, ctrl).name;
    string name = histoName_ + "_" + controlRegionName + "_" + systName_ + upOrDown;
    if(verbose_ > 1) cout << "*** add histogram " << name << endl; 
    addHisto(name, histos.controlRegion(ctrl));
  }
}
