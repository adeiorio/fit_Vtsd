#ifndef STAT_WORKSPACECATEGORY_H
#define STAT_WORKSPACECATEGORY_H
#include <vector>
#include <memory>
class WorkspaceDataFiller;
class WorkspaceSystBkgFiller;
class WorkspaceSystSigFiller;
class WorkspaceTFFiller;
class DataMCHistoCollection;
class WorkspaceRooContainer;

class WorkspaceCategory {
public:
  WorkspaceCategory(unsigned int category, const DataMCHistoCollection & datamc, 
		    WorkspaceRooContainer & container);
  void fill();
  void setVerbose(int v);
private:
  unsigned int nSig_, nBkg_, nCtrl_;
  std::shared_ptr<WorkspaceDataFiller> data_;
  std::vector<std::shared_ptr<WorkspaceSystSigFiller> > signals_;
  std::vector<std::shared_ptr<WorkspaceSystBkgFiller> > backgrounds_;
  std::shared_ptr<WorkspaceTFFiller> transferFactors_;
  int verbose_;
};

#endif
