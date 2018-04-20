#include "ttDM/stat/interface/WorkspaceSystBkgFiller.h"
#include "ttDM/stat/interface/Config.h"
#include "ttDM/stat/interface/WorkspaceRooContainer.h"
#include "ttDM/stat/interface/WorkspaceTransferFactors.h"
#include "ttDM/stat/interface/WorkspaceTFFiller.h"
#include "ttDM/stat/interface/HistoCollection.h"
#include "ttDM/stat/interface/HistoSet.h"
#include "HiggsAnalysis/CombinedLimit/interface/RooParametricHist.h"
#include "RooWorkspace.h"
#include "RooDataHist.h"
#include "RooArgList.h"
#include "RooAddition.h"
#include "TH1.h"
#include <string>
#include <iostream>
using namespace std;

WorkspaceSystBkgFiller::WorkspaceSystBkgFiller(WorkspaceRooContainer & container, const HistoCollection& histos,
					       const HistoSet & data, shared_ptr<WorkspaceTFFiller> tfFiller,
					       unsigned int category, const string & sampleName, const string & sampleDescr) :
  WorkspaceSystSampleFiller(container, histos, category, sampleName, sampleDescr),
  data_(data), tfFiller_(tfFiller) {
}

WorkspaceSystBkgFiller::~WorkspaceSystBkgFiller() {
}

void WorkspaceSystBkgFiller::prepare(unsigned int syst) {
  systName_ = Config::get().systematic(syst);
}


void WorkspaceSystBkgFiller::addParametricPdf(const string & pdfName, const string & pdfDescr,
					      RooArgList & bins, TH1 & hist) { 
  pdf_ = make_shared<RooParametricHist>(pdfName.c_str(), pdfDescr.c_str(),
					container_.variable(category_), bins, hist);
  container_.workspace().import(*pdf_);
  norm_ = make_shared<RooAddition>((pdfName+"_norm").c_str(), pdfDescr.c_str(), bins);
  container_.workspace().import(*norm_, RooFit::RecycleConflictNodes());
}

void WorkspaceSystBkgFiller::fill() {
  const Config & cfg = Config::get();
  unsigned int ctrlRegion = cfg.findControlRegion(category_, sampleName_);
  if(ctrlRegion != Config::npos) {
    if(verbose_>0) cout << "*** filling for bkg control region " << categoryName_ << ", " << sampleName_ << ", idx = " << ctrlRegion << endl;
    WorkspaceTransferFactors & tf = tfFiller_->transferFactors(ctrlRegion);
    RooArgList & bkgSRBins = tf.backgroundSRBins();
    RooArgList & bkgCRBins = tf.backgroundCRBins();
    TH1 & dataHist = data_.signalRegion();
    string pdfName = histoName_; 
    string pdfDescr = "Background PDF in signal region - " + pdfName;
    if(verbose_ > 0) cout << "*** add parametric pdf SR bins " << pdfName << endl;
    addParametricPdf(pdfName, pdfDescr, bkgSRBins, dataHist);
    //    for(unsigned int ctrl = 0; ctrl < nCtrl_; ++ctrl) 
    {
      if(verbose_ > 0) cout << "*** fill for ctrl region n. " << ctrlRegion /* */  << endl;
      TH1 & dataHist = data_.controlRegion(ctrlRegion);
      const string & controlRegionName = cfg.controlRegion(category_, ctrlRegion /* */).name;
      string pdfName = histoName_ + "_"+ controlRegionName;
      string pdfDescr = "Background PDF in signal region - " + pdfName;
      if (verbose_ > 0) cout << "*** add parametric pdf CR bins " << pdfName << endl;
      addParametricPdf(pdfName, pdfDescr, bkgCRBins, dataHist);
    }
  } else {
    WorkspaceSystFiller::fill();
  }
}
