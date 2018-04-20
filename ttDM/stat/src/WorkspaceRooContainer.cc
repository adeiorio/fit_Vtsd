#include "ttDM/stat/interface/WorkspaceRooContainer.h"
#include "ttDM/stat/interface/Config.h"
#include "RooMsgService.h"
#include "TFile.h"
using namespace std;

WorkspaceRooContainer::WorkspaceRooContainer() :
  workspace_("w") {
  const Config & cfg = Config::get();
  for(unsigned int i = 0; i < cfg.numCategories(); ++i) {
    string name = cfg.rooRealVar() + "_" + cfg.category(i);
    variable_.push_back(RooRealVar(name.c_str(), name.c_str(), -10, 14000));
  }
  RooMsgService::instance().setStreamStatus(1,false);
  for(unsigned int syst = 0; syst < cfg.numSystematics(); ++syst) {
    const char * systName = cfg.systematic(syst).c_str();
    systPar_.push_back(RooRealVar(systName, systName, -5, 5));
  }  
  for(unsigned int i = 0; i < cfg.numCategories(); ++i)
    workspace_.import(variable_[i]);
}

void WorkspaceRooContainer::save() {
  const Config & cfg = Config::get();
  string fileName = cfg.outputDirectory() + "/ws";
  system(("mkdir -p " + fileName).c_str());
  fileName += "/workspace";
  if(cfg.hasControlRegions()) fileName += "_ctrl";
  fileName += ".root";
  TFile file(fileName.c_str(),"RECREATE");
  if(!file.IsOpen()) throw runtime_error("can't open file " + fileName);
  file.cd();
  workspace_.Write();
  file.Close();
}
