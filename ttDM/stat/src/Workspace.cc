#include "ttDM/stat/interface/Workspace.h"
#include "ttDM/stat/interface/WorkspaceCategory.h"
#include "ttDM/stat/interface/DataMCHistoCollection.h"
#include "ttDM/stat/interface/Config.h"
#include "RooHistPdf.h"
#include "RooDataHist.h"
#include "RooRandom.h"
#include "RooAddition.h"
#include "RooFormulaVar.h"
#include <iostream>
using namespace RooFit;
using namespace std;

Workspace::Workspace(const vector<shared_ptr<DataMCHistoCollection> > & datamc) : 
  verbose_(0) {
  unsigned int cat = 0;
  for(auto coll = datamc.begin(); coll != datamc.end(); ++coll) {
    categories_.push_back(make_shared<WorkspaceCategory>(cat++, **coll, container_));
  }
}

void Workspace::fill() {
  if(verbose_ > 0) cout << "*** filling workspace ***" << endl;
  for(auto cat = categories_.begin(); cat != categories_.end(); ++cat)
    (*cat)->fill();
}

void Workspace::save() {
  if(verbose_ > 0) cout << "*** saving workspace ***" << endl;
  container_.save();
}

void Workspace::verbose(int v) { 
  verbose_ = v; 
  for(auto cat = categories_.begin(); cat != categories_.end(); ++cat)
    (*cat)->setVerbose(v);
}

//void Workspace::doAddPdf() {doAddPdf_ = true; }
//void Workspace::doAddRooDataHist() {doAddRooDataHist_ = true; }
