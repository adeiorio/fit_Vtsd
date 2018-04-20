#ifndef STAT_WORKSPACESYSTBKGFILLER_H
#define STAT_WORKSPACESYSTBKGFILLER_H
#include "ttDM/stat/interface/WorkspaceSystSampleFiller.h"
class RooArgList;
class RooAbsPdf;
class RooAddition;
class WorkspaceTFFiller;
class HistoSet;

class WorkspaceSystBkgFiller : public WorkspaceSystSampleFiller {
 public:
  WorkspaceSystBkgFiller(WorkspaceRooContainer & container, const HistoCollection& histos,
			 const HistoSet & data, std::shared_ptr<WorkspaceTFFiller> tfFiller,
			 unsigned int category, const std::string & sampleName, const std::string & sampleDescription);
  virtual ~ WorkspaceSystBkgFiller();
  virtual void fill();

 private:
  void prepare(unsigned int syst);
  void  addParametricPdf(const std::string & pdfName, const std::string & pdfDescr,
			 RooArgList & bins, TH1 & hist);
  const HistoSet & data_;
  std::shared_ptr<RooAbsPdf> pdf_;
  std::shared_ptr<RooAddition> norm_;
  std::shared_ptr<WorkspaceTFFiller> tfFiller_;
};


#endif
