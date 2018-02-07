#ifndef STAT_HISTOCOLLECTION_H
#define STAT_HISTOCOLLECTION_H
#include "ttDM/stat/interface/HistoSet.h"
#include "ttDM/stat/interface/HistoSetSyst.h"
#include "ttDM/stat/interface/Config.h"
#include <vector>
#include <memory>
#include <cmath>
#include <iostream>
#include "TH1.h"

class HistoCollection {
 public:
  explicit HistoCollection(unsigned int categoryIndex);
  const HistoSet & nominal() const { return * nominal_;}
  const HistoSet & up(unsigned int systIndex) const { return * up_[systIndex]; }
  const HistoSet & down(unsigned int systIndex) const { return * down_[systIndex]; }
  unsigned int numSystematics() const { return up_.size(); }
  void fill(const std::string & sampleName);
  double yieldNoCutNominal(unsigned int i = 0) const { 
    if(!nominal_) throw std::runtime_error("HistoCollection::yieldAfterCutUp: null pointer");
    return nominal_->yieldNoCut(i); 
  }
  double yieldNoCutUp(unsigned int syst, unsigned int i = 0) const { 
    if(!up_[syst]) throw std::runtime_error("HistoCollection: null pointer");
    return up_[syst]->yieldNoCut(i); 
  }
  double yieldNoCutDown(unsigned int syst, unsigned int i = 0) const { 
    if(!down_[syst]) throw std::runtime_error("HistoCollection: null pointer");
    return down_[syst]->yieldNoCut(i); 
  }
  double yieldAfterCutNominal(unsigned int i = 0) const { 
    if(!nominal_) throw std::runtime_error("HistoCollection: null pointer");
    return nominal_->yieldAfterCut(i); 
  }
  double yieldAfterCutUp(unsigned int syst, unsigned int i = 0) const { 
    if(!up_[syst]) throw std::runtime_error("HistoCollection: null pointer");
    return up_[syst]->yieldAfterCut(i); 
  }
  double yieldAfterCutDown(unsigned int syst, unsigned int i = 0) const { 
    if(!down_[syst]) throw std::runtime_error("HistoCollection: null pointer");
    return down_[syst]->yieldAfterCut(i); 
  }
  double systSizeAfterCut(unsigned int syst, unsigned int i = 0) const {
    double yield = yieldAfterCutNominal(i);
    if(yield > 0) 
      return 1.0 + 0.5 * (fabs(yieldAfterCutUp(syst, i) - yield) + 
			  fabs(yieldAfterCutDown(syst, i) - yield)) / yield;
   else return 1;
  }

  double systSizeNoCut(unsigned int syst, unsigned int i = 0) const {
    double yield = yieldNoCutNominal(i);
    if(yield > 0) 
      return 1.0 + 0.5 * (fabs(yieldNoCutUp(syst, i) - yield) + 
			  fabs(yieldNoCutDown(syst, i) - yield)) / yield;
   else return 1;
  }

  void verbose(int v) { verbose_ = v; }

 private:
  int verbose_;
  std::shared_ptr<HistoSet> nominal_;
  std::vector<std::shared_ptr<HistoSetSyst> > up_, down_;
};

#endif
