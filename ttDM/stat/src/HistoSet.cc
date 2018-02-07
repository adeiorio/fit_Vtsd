#include "ttDM/stat/interface/HistoSet.h"
#include "ttDM/stat/interface/Config.h"
#include "TFile.h"
#include "TH1.h"
#include <stdexcept>
#include <iostream>
#include <memory>

using namespace std;

HistoSet::HistoSet(unsigned int categoryIndex) : categoryIndex_(categoryIndex) { 
}

HistoSet::~HistoSet() { 
}

shared_ptr<TH1> HistoSet::getHisto(const string & path, const string & histoName) const {
  TH1 * histo;
  TFile file(path.c_str(), "OPEN");
  if(!file.IsOpen()) { throw runtime_error(string("can't open ") + path); }
  file.GetObject(histoName.c_str(), histo);
  if(histo == 0) { throw runtime_error(string("can't find ") + histoName + " in " + path); }
  histo->SetDirectory(0);
  file.Close();
  return shared_ptr<TH1>(histo);
}

void HistoSet::fill(const string & fileName) {
  const Config & cfg = Config::get();
  string path = cfg.histoDirectory() + "/" + fileName;
  histos_.push_back(getHisto(path, variableName()));
  if(cfg.hasControlRegions()) {
    fillCtrlRegionsHistos(path);
  }
}

void HistoSet::fillCtrlRegionsHistos(const string & fileName) {
  const vector<Config::ControlRegion> & ctrlRegions = Config::get().controlRegions(categoryIndex_);
  for(auto i = ctrlRegions.begin(); i != ctrlRegions.end(); ++i) {
    string h = histoName(i->name);
    histos_.push_back(getHisto(fileName, h));
  }
}

string HistoSet::histoName(const string & ctrlRegion) const {
  return variableName() + "_" + ctrlRegion;
}

double HistoSet::yieldNoCut(unsigned int i) const {
  if(histos_[i] == 0) throw runtime_error("HitoSet::yiedlNoCut called for null histogram");
  return histos_[i]->Integral();
}

double HistoSet::yieldAfterCut(unsigned int i) const {
  if(histos_[i] == 0) throw runtime_error("HistoSet::yiedlAfterCut called for null histogram");
  Int_t bins = histos_[i]->GetNbinsX();
  return histos_[i]->Integral(Config::get().binCut(categoryIndex_) + 1, bins);
}

const string HistoSet::variableName() const {
  return Config::get().variableName(categoryIndex_);
}

