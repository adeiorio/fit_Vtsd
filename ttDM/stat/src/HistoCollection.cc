#include "ttDM/stat/interface/HistoCollection.h" 
#include "ttDM/stat/interface/Config.h" 
#include "ttDM/stat/interface/HistoSetSyst.h"
#include "ttDM/stat/interface/HistoSetMCStat.h"
#include "ttDM/stat/interface/stringUtils.h" 
#include "ttDM/stat/interface/skipSystematic.h" 
#include <iostream>
using namespace std;

HistoCollection::HistoCollection(unsigned int categoryIndex) : verbose_(0) {
  nominal_ = make_shared<HistoSet>(categoryIndex);
  const Config & cfg = Config::get();
  for(unsigned int i = 0; i < cfg.numSystematics() + cfg.numNormSystematics(); ++i) {
    if(i < cfg.numSystematics() && beginsWith(cfg.systematic(i), "mcstat_")) {
      up_.push_back(make_shared<HistoSetMCStat>(categoryIndex, i, HistoSetSyst::Shift::Up));
      down_.push_back(make_shared<HistoSetMCStat>(categoryIndex, i, HistoSetSyst::Shift::Down));
    } else {
      up_.push_back(make_shared<HistoSetSyst>(categoryIndex, i, HistoSetSyst::Shift::Up));
      down_.push_back(make_shared<HistoSetSyst>(categoryIndex, i, HistoSetSyst::Shift::Down));
    }
  }
}

void HistoCollection::fill(const string & sampleName) {
  const Config & cfg = Config::get();
  int categoryIndex = nominal_->categoryIndex();
  string category = cfg.category(categoryIndex);
  string catFileName = cfg.categoryFileName(categoryIndex);
  string nominalFileName = sampleName + "_" + catFileName + ".root";
  if(verbose_ > 1) 
    cout << "*** filling nominal sample from file " << nominalFileName << endl;
  nominal_->fill(nominalFileName);
  // Systematics loop
  for(unsigned int i = 0; i < cfg.numSystematics(); ++i) {
    string prefix = sampleName + "_" + catFileName;
    string systName = cfg.systematic(i);
    if(!skipSystematic(category, systName, sampleName)) {
      bool mcstat = beginsWith(systName, "mcstat_");
      string upFileName, downFileName;
      if(mcstat) {
	upFileName = downFileName = prefix + ".root";
	string sys = systName;
	size_t found = sys.find("DMtt");
	if(found != string::npos) sys.replace(found, 4, sampleName);
	up_[i]->setSystName(sys);
	down_[i]->setSystName(sys);
      } else {
	upFileName = prefix + "_" + systName + "Up.root";
	downFileName = prefix + "_" + systName + "Down.root";
      }
      if(verbose_ > 1) 
	cout << "*** filling systematic sample " << systName << " Up from file " << upFileName << endl;
      up_[i]->fill(upFileName);
      if(verbose_ > 1) 
	cout << "*** filling systematic sample " << systName << " Down from file " << downFileName << endl;
      down_[i]->fill(downFileName);
    } else {
      if(verbose_ > 1) 
	cout << "*** skipping systematic sample " << systName << endl;
    }
  } // end systematics loop

  // NormSystematics loop
  for(unsigned int i = 0; i < cfg.numNormSystematics(); ++i) {
    unsigned int j = i + cfg.numSystematics();
    string prefix = sampleName + "_" + catFileName;
    string systName = cfg.normSystematic(i);
    if(!skipSystematic(category, systName, sampleName)) {
      string upFileName = prefix + "_" + systName + "Up.root";
      string downFileName = prefix + "_" + systName + "Down.root";
      if(verbose_ > 1) 
	cout << "*** filling systematic sample " << systName << " Up from file " << upFileName << endl;
      up_[j]->fill(upFileName);
      if(verbose_ > 1) 
	cout << "*** filling systematic sample " << systName << " Down from file " << downFileName << endl;
      down_[j]->fill(downFileName);
    } else {
      if(verbose_ > 1) 
	cout << "*** skipping systematic sample " << systName << endl;
    }
  } // end NormSystematics loop
}
