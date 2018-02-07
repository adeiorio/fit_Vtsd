#ifndef STAT_HISTOSETMCSTAT_H
#define STAT_HISTOSETMCSTAT_H
#include "ttDM/stat/interface/HistoSetSyst.h"

class HistoSetMCStat : public HistoSetSyst {
public:
 HistoSetMCStat(unsigned int categoryIndex, unsigned int systIndex, Shift upOrDown) : 
  HistoSetSyst(categoryIndex, systIndex,upOrDown), systName_("ERROR") { }
  virtual ~HistoSetMCStat() { }
  virtual void setSystName(const std::string & systName) { systName_ = systName; }
protected:
  virtual const std::string variableName() const;
private:
  std::string systName_;
};

#endif
