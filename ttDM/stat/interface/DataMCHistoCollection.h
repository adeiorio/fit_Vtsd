#ifndef STAT_DATAMCHISTOCOLLECTION_H
#define STAT_DATAMCHISTOCOLLECTION_H
#include "ttDM/stat/interface/HistoCollection.h"
#include <iostream>

class DataMCHistoCollection {
 public:
  DataMCHistoCollection(unsigned int categoryIndex);
  const HistoSet & data() const { return * data_; }
  const HistoCollection & signal(unsigned int sampleIndex) const { return * signals_[sampleIndex]; }
  unsigned int numSignals() const { return signals_.size(); }
  const HistoCollection & background(unsigned int sampleIndex) const { return * backgrounds_[sampleIndex]; }
  unsigned int numBackgrounds () const { return backgrounds_.size(); }
  void fill();
  double dataYieldNoCut(unsigned int i = 0) const { return data_->yieldNoCut(i); }
  double dataYieldAfterCut(unsigned int i = 0) const { return data_->yieldAfterCut(i); }

  double signalYieldNoCutNominal(unsigned int smp, unsigned int i = 0) const { 
    return signals_[smp]->yieldNoCutNominal(i); 
  }
  double signalYieldNoCutUp(unsigned int smp, unsigned int syst, unsigned int i = 0) const { 
    return signals_[smp]->yieldNoCutUp(syst, i); 
  }
  double signalYieldNoCutDown(unsigned int smp, unsigned int syst, unsigned int i = 0) const { 
    return signals_[smp]->yieldNoCutDown(syst, i); 
  }
  double signalSystSizeNoCut(unsigned int smp, unsigned int syst, unsigned int i = 0) const { 
    return signals_[smp]->systSizeNoCut(syst, i); 
  }
  double signalYieldAfterCutNominal(unsigned int smp, unsigned int i = 0) const { 
    return signals_[smp]->yieldAfterCutNominal(i); 
  }
  double signalYieldAfterCutUp(unsigned int smp, unsigned int syst, unsigned int i = 0) const { 
    return signals_[smp]->yieldAfterCutUp(syst, i); 
  }
  double signalYieldAfterCutDown(unsigned int smp, unsigned int syst, unsigned int i = 0) const { 
    return signals_[smp]->yieldAfterCutDown(syst, i); 
  }
  double signalSystSizeAfterCut(unsigned int smp, unsigned int syst, unsigned int i = 0) const { 
    return signals_[smp]->systSizeAfterCut(syst, i); 
  }
  double backgroundYieldNoCutNominal(unsigned int smp, unsigned int i = 0) const { 
    return backgrounds_[smp]->yieldNoCutNominal(i); 
  }
  double backgroundYieldNoCutUp(unsigned int smp, unsigned int syst, unsigned int i = 0) const { 
    return backgrounds_[smp]->yieldNoCutUp(syst, i); 
  }
  double backgroundYieldNoCutDown(unsigned int smp, unsigned int syst, unsigned int i = 0) const { 
    return backgrounds_[smp]->yieldNoCutDown(syst, i); 
  }
  double backgroundSystSizeNoCut(unsigned int smp, unsigned int syst, unsigned int i = 0) const { 
    return backgrounds_[smp]->systSizeNoCut(syst, i); 
  }
  double backgroundYieldAfterCutNominal(unsigned int smp, unsigned int i = 0) { return backgrounds_[smp]->yieldAfterCutNominal(i); }
  double backgroundYieldAfterCutUp(unsigned int smp, unsigned int syst, unsigned int i = 0) const { 
    return backgrounds_[smp]->yieldAfterCutUp(syst, i); 
  }
  double backgroundYieldAfterCutDown(unsigned int smp, unsigned int syst, unsigned int i = 0) const { 
    return backgrounds_[smp]->yieldAfterCutDown(syst, i); 
  }
  double backgroundSystSizeAfterCut(unsigned int smp, unsigned int syst, unsigned int i = 0) const { 
    return backgrounds_[smp]->systSizeAfterCut(syst, i); 
  }
  void verbose(int v);

 private:
  int verbose_;
  std::shared_ptr<HistoSet> data_;
  std::vector<std::shared_ptr<HistoCollection> > signals_, backgrounds_;
};

#endif
