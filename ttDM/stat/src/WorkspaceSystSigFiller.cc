#include "ttDM/stat/interface/WorkspaceSystSigFiller.h"
#include "ttDM/stat/interface/Config.h"

WorkspaceSystSigFiller::WorkspaceSystSigFiller(WorkspaceRooContainer & container, const HistoCollection& histos,
					       unsigned int category, const std::string & sampleName, const std::string & sampleDescr) :
  WorkspaceSystSampleFiller(container, histos, category, sampleName, sampleDescr) { }
    
WorkspaceSystSigFiller::~WorkspaceSystSigFiller() {}

void WorkspaceSystSigFiller::prepare(unsigned int syst) {
  systName_ = Config::get().systematic(syst);
  if(systName_.find("mcstat_") == 0) systName_.replace(systName_.find("DMtt"), 4, sampleName_);
}

