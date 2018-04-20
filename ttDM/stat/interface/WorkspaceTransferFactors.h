#ifndef STAT_WORKSPACETRANSFERFACTORS_H
#define STAT_WORKSPACETRANSFERFACTORS_H
#include "RooArgList.h"
#include <string>
#include <vector>
#include <memory>
class HistoCollection;
class RooRealVar;
class RooFormulaVar;
class RooArgList;
class HistoSet;
class WorkspaceRooContainer;

class WorkspaceTransferFactors {
 public:
  WorkspaceTransferFactors(WorkspaceRooContainer & container, const HistoCollection & histos,
			   unsigned int category, unsigned int  controlRegion);
  void fill();
  RooArgList & backgroundSRBins() { return backgroundSRBins_; }
  RooArgList & backgroundCRBins() { return backgroundCRBins_; }

 private:
  void fillSystematics(unsigned int bin, const std::string & stringBin);
  void fillRatioCRSR(const HistoSet & nominal, unsigned int bin, const std::string & strinBin);
  void fillSignalRegionBin(const std::string & strinBin);
  void fillControlRegionBin(unsigned int bin, const std::string & strinBin);
  WorkspaceRooContainer & container_;
  const HistoCollection & histos_;
  unsigned int category_;
  unsigned int controlRegion_;
  std::string controlRegionName_, sampleName_, categoryName_;
  std::vector<std::shared_ptr<RooRealVar> > ratioCRSR_; 
  std::vector<std::shared_ptr<RooRealVar> > signalRegionBins_;
  std::vector<std::vector<std::shared_ptr<RooRealVar> > > slopes_;
  std::vector<std::shared_ptr<RooFormulaVar> > controlRegionBins_;
  RooArgList backgroundSRBins_, backgroundCRBins_;
};

#endif
