#ifndef STAT_WORKSPACESYSTSAMPLEFILLER_H
#define STAT_WORKSPACESYSTSAMPLEFILLER_H
#include "ttDM/stat/interface/WorkspaceSystFiller.h"
#include "ttDM/stat/interface/WorkspaceSample.h"
#include <vector>
class WorkspaceRooContainer;
class HistoCollection;
class HistoSet;
class TH1;

class WorkspaceSystSampleFiller : public WorkspaceSample, public WorkspaceSystFiller {
 public:
  WorkspaceSystSampleFiller(WorkspaceRooContainer & container, const HistoCollection& histos,
			    unsigned int category, const std::string & sampleName, const std::string & sampleDescr);
  virtual ~WorkspaceSystSampleFiller();

 protected:
  unsigned int category_;
  std::string categoryName_;
  std::string sampleName_, histoName_;
  unsigned int nCtrl_;
  std::string systName_;

 private:
  virtual void fillNominal();
  virtual bool skip(unsigned int syst);
  virtual void fillUp(unsigned int syst);
  virtual void fillDown(unsigned int syst);
  void fillSyst(const HistoSet & histos, const std::string & upOrDown);

  const HistoCollection & histos_;

  std::vector<std::shared_ptr<RooDataHist> > rooHistos_;
};


#endif
