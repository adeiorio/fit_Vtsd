#ifndef STAT_WORKSPACESYSTSIGFILLER_H
#define STAT_WORKSPACESYSTSIGFILLER_H
#include "ttDM/stat/interface/WorkspaceSystSampleFiller.h"

class WorkspaceSystSigFiller : public WorkspaceSystSampleFiller {
 public:
  WorkspaceSystSigFiller(WorkspaceRooContainer & container, const HistoCollection& histos,
			 unsigned int category, const std::string & sampleName, const std::string & sampleDescr);
  virtual ~WorkspaceSystSigFiller();

 private:
  virtual void prepare(unsigned int syst);
};


#endif
