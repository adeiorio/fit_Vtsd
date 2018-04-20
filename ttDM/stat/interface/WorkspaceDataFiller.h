#ifndef STAT_WORKSPACEDATAFILLER_H
#define STAT_WORKSPACEDATAFILLER_H
#include "ttDM/stat/interface/WorkspaceSample.h"
#include <memory>
class RooDataHist;
class HistoSet;

class WorkspaceDataFiller : public WorkspaceSample {
 public:
  WorkspaceDataFiller(WorkspaceRooContainer & container, const HistoSet & data,
		      unsigned int category, unsigned int nCtrl);
  virtual ~WorkspaceDataFiller();
  void fill();

 private:
  const HistoSet & data_;
  unsigned int category_;
  unsigned int nCtrl_;
  std::vector<std::shared_ptr<RooDataHist> > rooHistos_;
};

#endif
