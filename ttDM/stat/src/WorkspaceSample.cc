#include "ttDM/stat/interface/WorkspaceSample.h"
#include "ttDM/stat/interface/WorkspaceRooContainer.h"
#include "RooDataHist.h"
#include "RooHistPdf.h"
#include "ttDM/stat/interface/Config.h"
using namespace std;

//WorkspaceSample::WorkspaceSample(unsigned int category, WorkspaceRooContainer & container, const string & sampleDescr, bool doAddPdf, bool doAddDataHisto) :
//  category_(category), container_(container), sampleDescr_(sampleDescr), addPdf_(doAddPdf), addDataHisto_ (doAddDataHisto) {
//}

WorkspaceSample::WorkspaceSample(unsigned int category, WorkspaceRooContainer & container, const string & sampleDescr) :
  category_(category), container_(container), sampleDescr_(sampleDescr) {
}

WorkspaceSample::~WorkspaceSample() {
}

void WorkspaceSample::addHisto(const string & name, TH1 & histo) {

  const Config & cfg = Config::get();
  addPdf_ = cfg.addPdf();
  addDataHist_ = cfg.addDataHist();
  //  cout << "adddatahist "<<addDataHist_<<endl; 
  //rooHistos_.push_back(make_shared<RooDataHist>(name.c_str(), sampleDescr_.c_str(), container_.variable(category_), &histo));
  //rooHistoPdfs_.push_back(make_shared<RooHistPdf>(name.c_str(), sampleDescr_.c_str(), container_.variable(category_), *rooHistos_.back()));
  if(addDataHist_){  
    rooHistos_.push_back(make_shared<RooDataHist>(name.c_str(), sampleDescr_.c_str(), container_.variable(category_), &histo));
    container_.workspace().import(*rooHistos_.back());
  }
  if(addPdf_){
    RooDataHist TmpRooDataHist(name.c_str(), sampleDescr_.c_str(), container_.variable(category_), &histo);
    rooHistoPdfs_.push_back(make_shared<RooHistPdf>(name.c_str(), sampleDescr_.c_str(), container_.variable(category_),TmpRooDataHist));
    container_.workspace().import(*rooHistoPdfs_.back());
  }
}


