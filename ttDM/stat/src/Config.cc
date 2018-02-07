#include "ttDM/stat/interface/Config.h"
#include "ttDM/stat/interface/stringUtils.h"
#include <boost/program_options.hpp>
#include <string>
#include <stdexcept>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <climits>
using namespace std;

Config * Config::instance_ = 0;

template<typename T>
void readVector(vector<T> & str, const string & cmd, istringstream & ss) {
  T sub;
  do { if(ss >> sub) str.push_back(sub); } while (ss);
}
void readVectorPair(vector<std::pair<std::string, std::string> > & str, const string & cmd, istringstream & ss) {
  std::pair<std::string, std::string> p;
  do { if(ss >> p.first and ss>>p.second) str.push_back(p); } while (ss);
}

void Config::hints(const boost::program_options::options_description & desc) const {
  cout << "Allowed options:"<< endl;
  cout << desc;
}

Config::~Config() { 
  instance_ = 0;
}

bool Config::excludedFromCategory(const string & cat, const string & sample) const {
  bool found=false;
  for (size_t c=0;c<(excludedCat_.size());++c){found = found || excludedCat_.at(c)==cat;}
  if(found) {
    for(size_t et=0; et<excludeFromCategory_.at(cat).size(); ++et){
    if( sample==excludeFromCategory_.at(cat).at(et) )return true;
    }
  }//else cout << "not found "<<found;
  return false;
}

unsigned int Config::findBackgroundSample(const string & name) const {
  auto found = backgroundSamples_.begin();
  for(; found != backgroundSamples_.end(); ++found)
    if(found->name == name) break;
  if(found == backgroundSamples_.end()) {
    throw runtime_error("background sample not found");
  }
  else return found - backgroundSamples_.begin();
}

unsigned int Config::findControlRegion(unsigned int cat, const std::string sample) const {
  if(cat >= controlRegions_.size()) return npos;
  auto found = controlRegions_[cat].begin();
  for(; found != controlRegions_[cat].end(); ++found) 
    if(found->sample == sample) break;
  if(found == controlRegions_[cat].end()) return npos;
  else return found - controlRegions_[cat].begin();
}

Config::Config(int argc, char ** argv) {
  using namespace boost;
  namespace po = boost::program_options;
  if(instance_ != 0) {
    instance_ = 0;
    throw runtime_error("configuration already loaded");
  }
  instance_ = this;
  string dataCardsFileName;
  po::options_description desc("Main options");
  desc.add_options()
    ("datacard,d", po::value<string>(&dataCardsFileName), 
     "Datacard file (can also be specified directly without the -d or --datacard)")
    ("dir,D", po::value<string>(&outputDirectory_)->default_value("."), "Output directory")
    ("verbose,v", po::value<int>(&verbose_)->default_value(1), "Verbose level (0 = quiet)")
    ("pdf,p", po::value<bool>(&addPdf_)->default_value(true), "add RooHistPdf in the workspace")
    ("hist,H", po::value<bool>(&addDataHist_)->default_value(true), "add RooDataHist in the workspace")
    ("help,h", "Produce help message")
    ;
  po::positional_options_description p;
  p.add("datacard", -1);
  po::variables_map vm;
  try{
    po::store(po::command_line_parser(argc, argv).positional(p).options(desc).allow_unregistered().run(), vm);
    po::notify(vm);
  } catch(std::exception &ex) {
    hints(desc);
    throw std::invalid_argument(string("invalid option: ") + ex.what()); 
  } catch(...) {
    cerr << "Unidentified error parsing options." << endl;
    instance_ = 0;
    throw(std::bad_exception());
  }
  if(vm.count("help")) { 
    hints(desc); 
    exit(0);
  }
  if(dataCardsFileName == "") {
    cerr << "Missing datacard file argument" << endl;
    hints(desc);
    instance_ = 0;
    throw std::invalid_argument("missing datacard file argument");
  }
  
  lumiUncertainty_ = 0;
  rooRealVar_ = "met";

  ifstream datacardFile(dataCardsFileName);
  if(!datacardFile.is_open()) {
    instance_ = 0;
    throw runtime_error("can't open datacard file: " + dataCardsFileName);
  }
  for(string line; getline(datacardFile, line); ) {
    bool commentLine = true;
    for(string::const_iterator c = line.begin(); c != line.end(); ++c) {
      if(*c == '#') {  break; }
      if(*c != ' ' || *c != '\t') { commentLine = false; break; }
    }
    if(!commentLine) {
      istringstream ss(line);
      string cmd, option;
      string name, description;
      ss >> cmd;
      if(cmd == "HistoDirectory") { ss >> histoDirectory_; }
      else if(cmd == "LumiUncertainty") { ss >> lumiUncertainty_; }
      else if(cmd == "RooRealVar") { ss >> rooRealVar_; }
      else if(cmd == "Categories") { readVector(categories_, cmd, ss); }
      else if(cmd == "CategoryFiles") { readVector(categoryFileNames_, cmd, ss); }
      else if(cmd == "VariableNames") { readVector(variableNames_, cmd, ss); }
      else if(cmd == "BinCut") { readVector(binCuts_, cmd, ss); }
      else if(cmd == "BackgroundSample") { 
	ss >> name; getline(ss, description);
	backgroundSamples_.push_back(Sample(name, trim(description)));
      }
      else if(cmd == "SignalSample") { 
	ss >> name; getline(ss, description);
	signalProcesses_.push_back(Sample(name, trim(description)));
      }
      else if(cmd == "SignalPoint") { 
	ss >> name; getline(ss, description);
	signalPoints_.push_back(Sample(name, trim(description)));
      }
      else if(cmd == "Systematics") { readVector(systematics_, cmd, ss); }
      else if(cmd == "NormSystematics") { readVector(normSystematics_, cmd, ss); }
      else if(cmd == "RateParamCategories") { readVector(rateCategories_, cmd, ss); }
      else if(cmd == "RateParamOld") { readVector(rateSystematics_, cmd, ss); }
      else if(cmd == "RateParam"){
	string name;
	ss>>name;
	vector<std::pair<std::string, std::string> > ch_process;
	readVectorPair(ch_process, cmd, ss);
	rateParams_[name] = ch_process ;
      }
      else if(cmd == "ExcludeFromCategory") { 
	string category; 
	vector<string> samples_to_exclude;
	ss >> category;
	readVector(samples_to_exclude, cmd, ss);
	const auto found = find(rateCategories_.begin(), rateCategories_.end(), category);
	if(found == rateCategories_.end()) { 
	  instance_ = 0;
	  throw std::runtime_error(string("category: ") + category + " not found");
	}
	excludeFromCategory_[category]=samples_to_exclude;
	excludedCat_.push_back(category); 
	cout << " excluding from category "<< category << " the following samples: "<<endl;
	  for(size_t et = 0; et<excludeFromCategory_[category].size();++et){
	    cout << " " << excludeFromCategory_[category][et]<<endl;
	  }
	//	excludeFro

	//	readVector(excludeFromCategories_, cmd, ss); 

      }
      else if(cmd == "ControlRegion") {
	controlRegions_.resize(categories_.size());
	string category, name, sample;
	ss >> category >> name >> sample;
	const auto found = find(categories_.begin(), categories_.end(), category);
	if(found == categories_.end()) { 
	  instance_ = 0;
	  throw std::runtime_error(string("category: ") + category + " not found");
	}
	controlRegions_[found - categories_.begin()].push_back(ControlRegion(name, sample, category));
      } else { 
	instance_ = 0;
	throw std::runtime_error(string("error parsing line: ") + line); 
      }
    }
  }
  unsigned int size = categories_.size();
  if(categoryFileNames_.size() != size || variableNames_.size() != size ||
     binCuts_.size() != size || (hasRateSystematics() && rateCategories_.size()!=size)) {
    instance_ = 0;
    throw std::runtime_error("inconsistent entries length in datacards");
  }
  if(signalPoints_.size() == 0) {
    // this is just for backward compatibility, will be removed
    cout << "signal points "<<endl;
    signalSamples_ = signalProcesses_;
    signalPoints_ = signalProcesses_;
    signalProcesses_.clear();
    signalProcesses_.push_back(Sample("DMtt", "tt+DM"));
    for(auto i = signalPoints_.begin(); i != signalPoints_.end(); ++i) {
      cout << "signal points "<< i->description<<endl;
    }
  } else {
    for(auto i = signalProcesses_.begin(); i != signalProcesses_.end(); ++i) {
      for(auto j = signalPoints_.begin(); j != signalPoints_.end(); ++j) {
	signalSamples_.push_back(Sample(i->name + "_" + j->name, i->description + ", " + j->description));
      }    
    }
  }
}
