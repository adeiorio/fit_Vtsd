#ifndef STAT_WORKSPACETFFILLER_H
#define STAT_WORKSPACETFFILLER_H
#include <vector>
#include <memory>
class WorkspaceTransferFactors;
class DataMCHistoCollection;
class WorkspaceRooContainer;

class WorkspaceTFFiller {
 public:
  WorkspaceTFFiller(unsigned int category, const DataMCHistoCollection & datamc, WorkspaceRooContainer & roo);
  void fill();
  WorkspaceTransferFactors & transferFactors(unsigned int ctrl) {
    return *transferFactors_[ctrl];
  }
  void setVerbose(int v) { verbose_ = v; }

 private:
   unsigned int nCtrl_;
   std::vector<std::shared_ptr<WorkspaceTransferFactors> > transferFactors_;
   int verbose_;
};

#endif
