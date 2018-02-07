#ifndef STAT_WORKSPACE_H
#define STAT_WORKSPACE_H
#include "ttDM/stat/interface/WorkspaceRooContainer.h"
#include <vector>
#include <memory>
class DataMCHistoCollection;
class WorkspaceCategory;

class Workspace {
public:
  explicit Workspace(const std::vector<std::shared_ptr<DataMCHistoCollection> > & datamc);
  void fill();
  void save();
  void verbose(int v);
  void doAddPdf();
  void doAddRooDataHist();

private:
  WorkspaceRooContainer container_;
  std::vector<std::shared_ptr<WorkspaceCategory> > categories_;
  int verbose_;
  //bool addPdf_,addRooDataHist_;
};

#endif
