#include "ttDM/stat/interface/WorkspaceTransferFactors.h"
#include "ttDM/stat/interface/WorkspaceRooContainer.h"
#include "ttDM/stat/interface/HistoSet.h"
#include "ttDM/stat/interface/HistoCollection.h"
#include "ttDM/stat/interface/Config.h"
#include "TH1.h"
#include "RooRealVar.h"
#include "RooArgList.h"
#include "RooFormulaVar.h"
#include <sstream>
#include <stdexcept>
using namespace std;
using namespace RooFit;

WorkspaceTransferFactors:: WorkspaceTransferFactors(WorkspaceRooContainer & container,
						    const HistoCollection & histos, 
						    unsigned int category,
						    unsigned int controlRegion) :
  container_(container), histos_(histos), category_(category), controlRegion_(controlRegion) { 
  categoryName_ = Config::get().category(category_);
  const Config::ControlRegion & ctrl = Config::get().controlRegion(category_, controlRegion_);
  controlRegionName_ = ctrl.name;
  sampleName_ = ctrl.sample;
}

void WorkspaceTransferFactors::fillRatioCRSR(const HistoSet & nominal, unsigned int bin, const string & stringBin) {
  double ratio = 1, d = nominal.signalRegion().GetBinContent(bin + 1);
  if(d > 0) ratio = nominal.controlRegion(controlRegion_).GetBinContent(bin + 1) / d;
  
  string name = "ratioCRSR_" + categoryName_ + "_" + sampleName_ + "_" + controlRegionName_ + "_bin" + stringBin;
  string descr = "Ratio CR/SR, " + categoryName_ + " => " + sampleName_ + ", " + controlRegionName_;
  ratioCRSR_.push_back(make_shared<RooRealVar>(name.c_str(), descr.c_str(), ratio));
  ratioCRSR_.back()->setConstant();
}

void WorkspaceTransferFactors::fillSignalRegionBin(const string & stringBin) {
  string name = "bkg_SR_" + categoryName_ + "_" + controlRegionName_ +"_bin" + stringBin;
  string descr = "Bkg. SR " + sampleName_ + " yield in SR " + controlRegionName_ + ", bin " + stringBin;
  signalRegionBins_.push_back(make_shared<RooRealVar>(name.c_str(), descr.c_str(), 10, 0, 100000));
}

void WorkspaceTransferFactors::fillControlRegionBin(unsigned int bin, const string & stringBin) {
  const Config& cfg = Config::get();
  string name = "bkg_CR_" + sampleName_ + "_"+ controlRegionName_ +"_bin" + stringBin;
  string descr = "Bkg. CR " + sampleName_ + " yield in SR " + controlRegionName_ + ", bin " + stringBin;
  if(cfg.hasSystematics()) {
    string formula = "@0*(";
    RooArgList args;
    args.add(*signalRegionBins_[bin]);
    unsigned int count = 1;
    for(unsigned int syst = 0; syst < cfg.numSystematics(); ++syst) {
      ostringstream ssc1; ssc1 << count; formula += "@" + ssc1.str();
      ostringstream ssc2; ssc2 << ++count; formula += "*@" + ssc2.str();
      args.add(container_.systematicParameter(syst));
      args.add(*slopes_[syst][bin]);
      if(syst < cfg.numSystematics() - 1) formula += " + ";
    }
    formula += ")";
    controlRegionBins_.push_back(make_shared<RooFormulaVar>(name.c_str(), descr.c_str(), formula.c_str(), args));
  } else {
    RooArgList args(*signalRegionBins_[bin], *ratioCRSR_[bin]);
    controlRegionBins_.push_back(make_shared<RooFormulaVar>(name.c_str(), descr.c_str(), "@0*@1", args));
  }
}

void WorkspaceTransferFactors::fillSystematics(unsigned int bin, const string & stringBin) {
  const Config & cfg = Config::get();
  unsigned int nSyst = cfg.numSystematics();
  slopes_.resize(nSyst);
  for(unsigned int syst = 0; syst < nSyst; ++syst) {
    const string & systematic = cfg.systematic(syst);
    const TH1 & upCR = histos_.up(syst).controlRegion(controlRegion_); 
    const TH1 & upNominal = histos_.up(syst).signalRegion(); 
    const TH1 & downCR = histos_.down(syst).controlRegion(controlRegion_); 
    const TH1 & downNominal = histos_.down(syst).signalRegion(); 
    double delta_up = upCR.GetBinContent(bin + 1) - upNominal.GetBinContent(bin + 1);
    double delta_down = downNominal.GetBinContent(bin + 1) - downCR.GetBinContent(bin + 1);
    double ratio_slope = 0.5 * (delta_up + delta_down);
    const string name = "ratioCRSR_" + categoryName_ + "_" + sampleName_ + "_" + 
      controlRegionName_ + "_" + systematic + "Slope_bin" + stringBin;
    const string descr = "Ratio CR/SR, " + categoryName_ + " => " + 
      sampleName_ + ", " + systematic + "Slope, " + controlRegionName_;
    slopes_[syst].push_back(make_shared<RooRealVar>(name.c_str(), descr.c_str(), ratio_slope));
    slopes_[syst].back()->setConstant();
  }
}

void WorkspaceTransferFactors::fill() {
  const HistoSet & nominal = histos_.nominal();
  unsigned int nBins = nominal.signalRegion().GetNbinsX();
  if(int(nBins) != nominal.controlRegion(controlRegion_).GetNbinsX())
    throw runtime_error("signal and control region have different binning");
  for(unsigned int bin = 0; bin < nBins; ++bin) {
    ostringstream sBin; sBin << bin + 1;
    fillRatioCRSR(nominal, bin, sBin.str());
    fillSignalRegionBin(sBin.str());
    fillSystematics(bin, sBin.str());
    fillControlRegionBin(bin, sBin.str());
    backgroundSRBins_.add(*signalRegionBins_.back());
    backgroundCRBins_.add(*controlRegionBins_.back());
  }
}
