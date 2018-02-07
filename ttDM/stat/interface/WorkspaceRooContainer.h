#ifndef STAT_WORKSPACEROOCONTAINER_H
#define STAT_WORKSPACEROOCONTAINER_H
#include "RooRealVar.h"
#include "RooWorkspace.h"
#include <vector>
#include <stdexcept>

class WorkspaceRooContainer {
 public:
  WorkspaceRooContainer();
  RooRealVar &  variable(unsigned int cat) { return variable_[cat]; }
  RooWorkspace & workspace() { return workspace_; }
  const RooRealVar & systematicParameter(unsigned int syst) const { 
    if(syst >= systPar_.size()) throw std::runtime_error("WorkspaceRooContainer: systematic par. out of range");
    return systPar_[syst]; 
  }
  void save();

 private:
  RooWorkspace workspace_;
  std::vector<RooRealVar> variable_;
  std::vector<RooRealVar> systPar_;
};

#endif
