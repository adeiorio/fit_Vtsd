#ifndef STAT_HISTOSETSYST_H
#define STAT_HISTOSETSYST_H
#include "ttDM/stat/interface/HistoSet.h"

class HistoSetSyst : public HistoSet {
public:
  enum class Shift { Up, Down };   
  HistoSetSyst(unsigned int categoryIndex, unsigned int systIndex, Shift upOrDown) : 
  HistoSet(categoryIndex), upOrDown_(upOrDown), systIndex_(systIndex) { }
  virtual ~HistoSetSyst() { }
  unsigned int systIndex() const { return systIndex_; }
  virtual void setSystName(const std::string & systName) {}
 protected:
  Shift upOrDown_;
 private:
  unsigned int systIndex_;
  virtual std::string histoName(const std::string & ctrlRegion = "") const;

  
};

#endif
