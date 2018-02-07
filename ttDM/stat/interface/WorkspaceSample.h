#ifndef STAT_WORKSPACESAMPLE_H
#define STAT_WORKSPACESAMPLE_H
#include <memory>
#include <vector>
#include <string>
class RooDataHist;
class RooHistPdf;
class WorkspaceRooContainer;
class TH1;

class WorkspaceSample {
 public:
  //  WorkspaceSample(unsigned int category, WorkspaceRooContainer & container, const std::string & sampleDescr, bool doAddPdf=true, bool doAddDataHisto=true);
  WorkspaceSample(unsigned int category, WorkspaceRooContainer & container, const std::string & sampleDescr);
  virtual ~WorkspaceSample();

 protected:
  void addHisto(const std::string & name, TH1 & histo);

  unsigned int category_;
  std::vector<std::shared_ptr<RooDataHist> > rooHistos_;
  std::vector<std::shared_ptr<RooHistPdf> > rooHistoPdfs_;
  WorkspaceRooContainer & container_;
  std::string sampleDescr_; 
  bool addPdf_,addDataHist_;
};

#endif
