#ifndef STAT_HISTOSET_H
#define STAT_HISTOSET_H
#include <vector>
#include <string>
#include <stdexcept>
#include <memory>
#include <iostream>
class TH1;
class TFile;

class HistoSet {
public:
  explicit HistoSet(unsigned int categoryIndex);
  virtual ~HistoSet();
  void fill(const std::string & fileName);
  std::shared_ptr<TH1> getHisto(const std::string & fileName, const std::string & histoName) const;
  unsigned int categoryIndex() const { return categoryIndex_; }
  TH1 & signalRegion() const { 
    if(histos_.size() == 0) throw std::runtime_error("HistoSet uninitialized");
    return * histos_[0]; 
  }
  TH1 & controlRegion(unsigned int ctrlIndex) const {
    if(histos_.size() < ctrlIndex + 2) {
      std::cerr << "array size: " << histos_.size() << ", ctrlIndex: " << ctrlIndex << std::endl;
      throw std::runtime_error("HistoSet array out of bound");
    }
    return * histos_[ctrlIndex + 1]; 
  }
  unsigned int numControlRegions() const { return histos_.size() - 1; }
  double yieldNoCut(unsigned int i = 0) const;
  double yieldAfterCut(unsigned int i = 0) const;
  unsigned int numRegions() const { return histos_.size(); }

protected:
  unsigned int categoryIndex_;
  std::vector<std::shared_ptr<TH1> > histos_;
  void fillCtrlRegionsHistos(const std::string & path);
  virtual std::string histoName(const std::string & ctrlRegion = "") const;
  virtual const std::string variableName() const;
};

#endif
