#include "ttDM/stat/interface/DataMCHistoCollection.h"
#include "ttDM/stat/interface/Config.h"
#include <iostream>
using namespace std;

DataMCHistoCollection::DataMCHistoCollection(unsigned int categoryIndex) : verbose_(0) {
  data_ = make_shared<HistoSet>(categoryIndex);
  for(unsigned int i = 0; i < Config::get().numBackgroundSamples(); ++i) {
    backgrounds_.push_back(make_shared<HistoCollection>(categoryIndex));
  }
  for(unsigned int i = 0; i < Config::get().numSignalSamples(); ++i) {
    signals_.push_back(make_shared<HistoCollection>(categoryIndex));
  }
}

void DataMCHistoCollection::fill() {
  const Config & cfg = Config::get();
  string catFileName = cfg.categoryFileName(data_->categoryIndex());
  if(verbose_ > 0) cout << "*** importing data histograms ***" << endl;
  data_->fill("Data_" + catFileName + ".root");
  for(unsigned int i = 0; i < cfg.numBackgroundSamples(); ++i) {
    if(verbose_ > 0) cout << "*** importing background MC histograms for " << cfg.backgroundSample(i).description << " ***" << endl;
    backgrounds_[i]->fill(cfg.backgroundSample(i).name);
  }
  for(unsigned int i = 0; i < cfg.numSignalSamples(); ++i) {
    if(verbose_ > 0) cout << "*** importing signal MC histograms for " << cfg.signalSample(i).description << " ***" << endl;
    signals_[i]->fill(cfg.signalSample(i).name);
  }
}

void DataMCHistoCollection::verbose(int v) { 
  verbose_ = v; 
  for(auto i = signals_.begin(); i != signals_.end(); ++i) 
    (*i)->verbose(v);
  for(auto i = backgrounds_.begin(); i != backgrounds_.end(); ++i)
    (*i)->verbose(v);
}

