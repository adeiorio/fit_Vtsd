#include "ttDM/stat/interface/Datacard.h"
#include "ttDM/stat/interface/DataMCHistoCollection.h"
#include "ttDM/stat/interface/Config.h"
#include "ttDM/stat/interface/skipSystematic.h"
#include <iomanip>
#include <iostream>
#include <fstream>

using namespace std;

Datacard::Datacard(const vector<shared_ptr<DataMCHistoCollection> > & datamc, 
		   unsigned int catMin) :
  verbose_(0), datamc_(datamc) {
  if(catMin == UINT_MAX) {
    catMin_ = 0;
    catMax_ = Config::get().numCategories();
  } else {
    catMin_ = catMin;
    catMax_ = catMin_ + 1;
  }
}

void Datacard::header(ostream & out) const {
  const Config & cfg = Config::get();
  auto cat = cfg.categories().begin();
  out << "imax " << catMax_ - catMin_ << " number of channels (" << *cat++;
  for(; cat != cfg.categories().end(); ++cat) 
    out << ", " << *cat;
  auto sample = cfg.backgroundSamples().begin();
  out << ")" << endl
      << "jmax " << cfg.backgroundSamples().size() << " number of backgrounds (" << (sample++)->name;
  for(; sample != cfg.backgroundSamples().end(); ++sample) 
    out << ", " << sample->name;
  out << ")" << endl
      << "kmax " << 1 + cfg.systematics().size() + cfg.normSystematics().size() << " number of nuisance parameters (lumi";
  for(auto syst = cfg.systematics().begin(); syst != cfg.systematics().end(); ++syst)
    out << ", " << *syst;
  for(auto syst = cfg.normSystematics().begin(); syst != cfg.normSystematics().end(); ++syst)
    out << ", " << *syst;
  out << ")" << endl << "---------------------" << endl;
  //  for(auto syst = cfg.rateSystematics().begin(); syst != cfg.rateSystematics().end(); ++syst)
    //    ((    out << ", " << *syst;
	  //  out << ")" << endl
	
}

void Datacard::observation(ostream & out,  bool cuts, bool dummy, unsigned int ctrlRegion) const {
  const Config & cfg = Config::get();
  if(ctrlRegion < UINT_MAX && catMax_ - catMin_ > 1) {
    throw runtime_error("datacard observation: cant't retrieve a ctrl region for more categories");
  }
  out << "bin";
  for(unsigned int cat = catMin_; cat < catMax_; ++cat) {
    out << "\t" << cfg.category(cat);
    if(ctrlRegion < UINT_MAX) {
      out << "_" << cfg.controlRegions(cat)[ctrlRegion].name << endl;
    }
  }
  if(ctrlRegion == UINT_MAX) ctrlRegion = 0;
  else ctrlRegion++;
  out << endl;
  out << "observation";
  for(unsigned int cat = catMin_; cat < catMax_; ++cat) {
    if(dummy) out << "\t" << -1;
    else if(cuts) out << "\t" << datamc_[cat]->dataYieldAfterCut(ctrlRegion);
    else out << "\t" << datamc_[cat]->dataYieldNoCut(ctrlRegion);
  }
  out << endl;
  out << "---------------------" << endl;  
}

void Datacard::shapes(ostream & out, bool ctrl) const {
  out << "shapes\t*\t*\t../ws/workspace" << (ctrl ? "_ctrl" : "") 
      << ".root\tw:$PROCESS_$CHANNEL\tw:$PROCESS_$CHANNEL_$SYSTEMATIC" << endl;
  out << "shapes\tdata_obs\t*\t../ws/workspace" << (ctrl ? "_ctrl" : "")
      << ".root\tw:data_obs_$CHANNEL" << endl;
  out << "---------------------" << endl;  
}


void Datacard::fixSystName(string & syst, unsigned int signalPoint) const {
  const Config & cfg = Config::get();
  for(unsigned int j = 0; j < cfg.numSignalProcesses(); ++j) {
    const string & processName = cfg.signalProcess(j).name;
    if(syst.find("mcstat") == 0 && syst.find(processName) != string::npos){
      syst.replace(syst.find(processName), 4, cfg.signalSample(cfg.signalSampleIndex(j, signalPoint)).name);
    }
  }
}

void Datacard::systematics(ostream& out, bool shape, int signalPoint) const {
  const Config & cfg = Config::get();
  out << "lumi\tlnN";
  double lumi = 1 + cfg.lumiUncertainty();
  unsigned int nProc = cfg.numSignalProcesses();
  for(unsigned int i = catMin_; i < catMax_; ++i) {
    out << "\t" << lumi;
    for(unsigned int j = 0; j < cfg.numBackgroundSamples(); ++j) 
      out << "\t" << lumi;
  }
  out << std::setprecision(4) << endl;
  if(cfg.hasSystematics() or cfg.hasNormSystematics() or cfg.hasRateSystematics() or cfg.hasRateParams()) {
    if(shape) {
      if(cfg.hasNormSystematics()){
	for(unsigned int s = 0; s < cfg.numNormSystematics(); ++s) {
	  unsigned int ss = s + cfg.numSystematics();
	  string syst = cfg.normSystematic(s);
	  fixSystName(syst, signalPoint);
	  out << syst << "\tlnN";
	  for(unsigned int i = catMin_; i < catMax_; ++i) {
	    string category = cfg.category(i);
	    for(unsigned int j = 0; j < nProc; ++j) {
	      if(skipSystematic(category, syst, "")) out << "\t-";
	      else out << "\t" << datamc_[i]->signalSystSizeNoCut(cfg.signalSampleIndex(j, signalPoint), ss);
	    }
	    for(unsigned int j = 0; j < cfg.numBackgroundSamples(); ++j) {
	      if(skipSystematic(category, syst, cfg.backgroundSample(j).name)) out << "\t-";
	      else out << "\t" << datamc_[i]->backgroundSystSizeNoCut(j, ss);
	    }
	  }
	  out << endl;
	}	
      }// end hasNormSystematics
      if(cfg.hasSystematics()){
	const double scale = 1;
	for(unsigned int s = 0; s < cfg.numSystematics(); ++s) {
	  string syst = cfg.systematic(s);
	  fixSystName(syst, signalPoint);
	  out << syst << "\tshape";
	  for(unsigned int i = catMin_; i < catMax_; ++i) {
	    string category = cfg.category(i);
	    // anything missing for signal???
	    for(unsigned int j = 0; j < cfg.numBackgroundSamples() + 1; ++j) {
	      out << "\t";
	      if (skipSystematic(category, syst, j == 0 ? "" : cfg.backgroundSample(j - 1).name)) out << "-";
	      else out << scale;
	    }
	  }
	  out << endl;
	}
      }//end hasShapeSystematics
      if(cfg.hasRateParams()){
	for(std::map<std::string,std::vector<std::pair<std::string, std::string> > >::const_iterator iter = cfg.rateParams().begin(); iter != cfg.rateParams().end(); ++iter)
	  {
	    std::string name =  iter->first;
	    std::vector<std::pair<std::string, std::string> > cr = iter->second;
	    for(std::vector<std::pair<std::string, std::string> >::iterator iter_cr = cr.begin(); iter_cr != cr.end(); ++iter_cr ){
	      out<<name<<"\trateParam\t"<<iter_cr->first<<"\t"<<iter_cr->second<<"\t1"<<std::endl;
	      
	    }
	  }
      }// end of hasRateParams
      
      if(cfg.hasRateSystematics()){
	for(unsigned int i = catMin_; i < catMax_; ++i) {
	  string category = cfg.category(i);
	  string rateCategory = cfg.rateCategory(i);
	  for(unsigned int s = 0; s < cfg.numRateSystematics(); ++s) {
	    string syst = cfg.rateSystematic(s);
	    // anything missing for signal???
	    for(unsigned int j = 0; j < cfg.numBackgroundSamples(); ++j) {
	      string bkg = cfg.backgroundSample(j).name;

	      if( syst == bkg){
		if(!(cfg.excludedFromCategory(rateCategory,bkg)))out << cfg.rateSystematic(s) <<"_"<<rateCategory<<"_norm\trateParam\t"<<category<<"\t"<<bkg<<"\t1"<<endl;
	      }
	    }
	  }
	  out << endl;
	}
      }//hasRateSystematics
  
    } else {
      if(cfg.hasSystematics()){
	for(unsigned int s = 0; s < cfg.numSystematics(); ++s) {
	  string syst = cfg.systematic(s);
	  out << cfg.systematic(s) << "\tlnN";
	  for(unsigned int i = catMin_; i < catMax_; ++i) {
	    string category = cfg.category(i);
	    for(unsigned int j = 0; j < nProc; ++j) {
	      if(skipSystematic(category, syst, "")) out << "\t-";
	      else out << "\t" << datamc_[i]->signalSystSizeAfterCut(cfg.signalSampleIndex(j, signalPoint), s);
	    }
	    for(unsigned int j = 0; j < cfg.numBackgroundSamples(); ++j) {
	      if(skipSystematic(category, syst, cfg.backgroundSample(j).name)) out << "\t-";
	      else out << "\t" << datamc_[i]->backgroundSystSizeAfterCut(j, s);
	    }
	  }
	  out << endl;
	}
      }//hasShapeSystematics
      if(cfg.hasNormSystematics()){
	for(unsigned int s = 0; s < cfg.numNormSystematics(); ++s) {
	  unsigned int ss = s + cfg.numSystematics();
	  string syst = cfg.normSystematic(s);
	  out << cfg.normSystematic(s) << "\tlnN";
	  for(unsigned int i = catMin_; i < catMax_; ++i) {
	    string category = cfg.category(i); 
	    for(unsigned int j = 0; j <nProc; ++j) {
	      if(skipSystematic(category, syst, "")) out << "\t-";
	      else out << "\t" << datamc_[i]->signalSystSizeAfterCut(cfg.signalSampleIndex(j, signalPoint), ss);
	    }
	    for(unsigned int j = 0; j < cfg.numBackgroundSamples(); ++j) {
	      if(skipSystematic(category, syst, cfg.backgroundSample(j).name)) out << "\t-";
	      else out << "\t" << datamc_[i]->backgroundSystSizeAfterCut(j, ss);
	    }
	  }
	  out << endl;
	}
      }//hasShapeSystematics
    }
  }
}

void Datacard::rate(ostream & out, bool noCut, unsigned int signalPoint, bool dummy, unsigned int ctrlRegion) const {
  const Config & cfg = Config::get();
  const unsigned int nProc = cfg.numSignalProcesses();
  out << "bin";
  unsigned int nSamples = cfg.numBackgroundSamples() + 1;
  for(unsigned int i = catMin_; i < catMax_; ++i) 
    for(unsigned int j = 0; j < nSamples; ++j) {
      out << "\t" << cfg.category(i);
      if(ctrlRegion < UINT_MAX) 
	out << "_" << cfg.controlRegion(catMin_, ctrlRegion).name;
    }
  out << endl;
  out << "process";
  for(unsigned int i = catMin_; i < catMax_; ++i) {
    for(unsigned int j = 0; j < nProc; ++j) 
      out << "\t" << cfg.signalSample(cfg.signalSampleIndex(j, signalPoint)).name;
    for(unsigned int j = 0; j < nSamples - 1; ++j) 
      out << "\t" << cfg.backgroundSample(j).name;
  }
  out << endl;
  out << "process";
  for(unsigned int i = catMin_; i < catMax_; ++i)
    for(unsigned int j = -(nProc - 1); j < nSamples; ++j)
      out << "\t" << j;
  out << endl;
  out << "rate" << std::setprecision(4); 
  for(unsigned int i = catMin_; i < catMax_; ++i) { 
    for(unsigned int j = 0; j < nProc; ++j) {
      if(dummy) out << "\t" <<  -1;
      else if(noCut)  out << "\t" << datamc_[i]->signalYieldNoCutNominal(cfg.signalSampleIndex(j, signalPoint));
      else  out << "\t" << datamc_[i]->signalYieldAfterCutNominal(cfg.signalSampleIndex(j, signalPoint));
    }
    for(unsigned int j = 0; j <  nSamples - 1;  ++j) {
      if(ctrlRegion < UINT_MAX && 
	 cfg.controlRegion(i, ctrlRegion).sample == cfg.backgroundSample(j).name) {
	out << "\t" << 1;
      } else if(dummy) {
	out << "\t" << -1;
      } else {
	if(noCut) out << "\t" << datamc_[i]->backgroundYieldNoCutNominal(j);
	else out << "\t" << datamc_[i]->backgroundYieldAfterCutNominal(j);
      }
    }
  }
  out << endl;
  out << "---------------------" << endl;  
}

void Datacard::save(const string & title, unsigned int signalPoint, bool noCut, unsigned int catMin) {
  //  cout << "saving "<< signalPoint<<endl;
  const Config & cfg = Config::get();
  if(catMin == UINT_MAX) { catMin_ = 0, catMax_ = cfg.numCategories(); }
  else { catMin_ = catMin; catMax_ = catMin + 1; }
  bool shape = title == "shape";
  bool ctrl = title == "ctrl";
  bool count = title == "count";
  string signalPointName = cfg.signalPoint(signalPoint).name;
  string fileNamePre = cfg.outputDirectory() + "/" + signalPointName;
  system(("mkdir -p " + fileNamePre).c_str());
  fileNamePre += "/"; fileNamePre += signalPointName;
  if(catMax_ - catMin_ == 1) { fileNamePre += "_"; fileNamePre += cfg.category(catMin_); }
  fileNamePre +="_"; fileNamePre += title;
  string fileName = fileNamePre + (ctrl ? "_sigCat" : "") + ".txt";
  ofstream out(fileName.c_str());
  out << ">>> datacard for " << signalPointName;
  if(catMax_ - catMin_ == 1)  out << " " << cfg.category(catMin_);
  out << ", " << title << endl << endl;
  header(out);
  if(shape || ctrl) shapes(out, ctrl);
  observation(out, count, false);
  rate(out, noCut, signalPoint, false);
  systematics(out, shape || ctrl, signalPoint);
  out.close();
  if(ctrl) {
    if(catMax_ - catMin_ != 1) { 
      throw(runtime_error("can't generate ctrl samples datacards for multiple categories"));
    }
    unsigned int category = catMin_;
    for(unsigned int ctrlRegion = 0; ctrlRegion < cfg.numControlRegions(category); ++ctrlRegion) {
      string ctrlSample = cfg.controlRegion(category, ctrlRegion).name;
      string fileNameCR = fileNamePre + "_" + ctrlSample + "_ctrlCat.txt";
      ofstream outCR(fileNameCR.c_str());
      outCR << "# datacard for " << cfg.signalPoint(signalPoint).name
	    << " (control region: " << ctrlSample << " )" << " " << cfg.category(category);
      outCR << ", " << title << endl << endl;
      header(outCR);
      shapes(outCR, true);
      observation(outCR, shape,  true, ctrlRegion);
      rate(outCR, noCut, signalPoint, true, ctrlRegion);
      systematics(outCR, true, signalPoint);
      outCR.close();
    }
  }
}

void Datacard::save() {
  const Config & cfg = Config::get();
  if(verbose_ > 0) 
    cout << "*** creating datacards including all samples ***" << endl;
  unsigned int nPoints = cfg.numSignalPoints();
  //  cout << "n signal points " << cfg.numSignalPoints()<<endl;
  for(unsigned int signalPoint = 0; signalPoint < nPoints; ++signalPoint) {
    //    cout << "signal point "<< signalPoint<<endl;
    if(!cfg.hasControlRegions()) {
      if(verbose_ > 1) 
	cout << "+++ signal point " << cfg.signalPoint(signalPoint).description << endl; 
      save("count", signalPoint, false);
      save("shape", signalPoint, true);
    }
    //    cout << "cncat " << cfg.numCategories()<<endl;
    for(unsigned int category = 0; category < cfg.numCategories(); ++category) {
      if(verbose_ > 1) cout << "+++ datacard for category " << cfg.category(category) << endl; 
      if(cfg.hasControlRegions())
        save("ctrl", signalPoint, true, category);
      else {
        save("count", signalPoint, false, category);
        save("shape", signalPoint, true, category);
      }
    }
  }  
}
