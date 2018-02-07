#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <boost/program_options.hpp>
#include "common.h"
using namespace std;

void readString(string & str, const string& cmd, istringstream & ss) {
  ss >> str;
  cout << cmd << " " << str << endl;  
}

void readBool(bool & b, const string& cmd, istringstream & ss) {
  string option;
  ss >> option;
  if(option == "true") { b = true; } 
  else if(option == "false") { b = false; } 
  else {
    cerr << "parse error: " << cmd << " " << option << endl;
    exit(1001);
  }
  cout << cmd << " " << option << endl;
}

void push2Strings(vector<string> & v1, vector<string> & v2, const string & cmd, istringstream & ss) {
  string shortName, longName;
  ss >> shortName; getline(ss, longName);
  if(shortName.size() == 0) {
    cerr << "parse error: " << cmd << " " << shortName << " " << longName << endl;
    exit(1001);
  }
  v1.push_back(shortName);
  v2.push_back(longName);
  cout << cmd << " " << shortName << " " << longName << endl;
}

void readVString(vector<string> & str, const string & cmd, istringstream & ss) {
  do {
    string sub;
    if(ss >> sub) str.push_back(sub);
  } while (ss);
  cout << cmd;
  for(unsigned int i = 0; i < str.size(); ++i) 
    cout << " " << str[i];
  cout << endl;
}

void readVInt(vector<int> & str, const string & cmd, istringstream & ss) {
  do {
    int sub;
    if(ss >> sub) str.push_back(sub);
  } while (ss);
  cout << cmd;
  for(unsigned int i = 0; i < str.size(); ++i) 
    cout << " " << str[i];
  cout << endl;
}

int parseArgs(int argc, char *argv[]) {
  using namespace boost;
  namespace po = boost::program_options;
  string datacard;
  po::options_description desc("Main options");
  desc.add_options()
    ("datacard,d", po::value<string>(&datacard), "Datacard file (can also be specified directly without the -d or --datacard)")
    ("dir,D", po::value<string>(&outDir)->default_value("."), "Output directory")
    ("help,h", "Produce help message")
    ;
  po::positional_options_description p;
  p.add("datacard", -1);
  po::variables_map vm;
  try{
    po::store(po::command_line_parser(argc, argv).options(desc).allow_unregistered().run(), vm);
    po::notify(vm);
  } catch(std::exception &ex) {
    cerr << "Invalid options: " << ex.what() << endl;
    cout << "Invalid options: " << ex.what() << endl;
    cout << "Use combine --help to get a list of all the allowed options"  << endl;
    return 999;
  } catch(...) {
    cerr << "Unidentified error parsing options." << endl;
    return 1000;
  }
  // if help, print help
  if(vm.count("help")) {
    cout << "Usage: combine datacard [options]\n";
    cout << desc;
    exit(0);
  }
  if(datacard == "") {
    cerr << "Missing datacard file" << endl;
    cout << "Usage: analyzeHistos [options]\n";
    cout << "Use combine --help to get a list of all the allowed methods and options"  << endl;
    return 1002;
  }

  syst = false;
  cat = -1;
  ctrlRegions = false;
  histoDir ="histos";

  nBkg = 0; nSig = 0; nSamples = 0;
  vector<string> bkgShortName, bkgLongName, sigShortName, sigLongName;

  ifstream datacardFile(datacard);
  for(string line; getline(datacardFile, line); ) {
    bool comment = true;
    for(string::const_iterator c = line.begin(); c != line.end(); ++c) {
      if(*c == '#') {  break; }
      if(*c != ' ' || *c != '\t') { comment = false; break; }
    }
    if(!comment) {
      istringstream ss(line);
      string cmd, option;
      ss >> cmd;
      if(cmd == "HistoDirectory") { readString(histoDir, cmd, ss); } 
      else if(cmd == "Categories") { readVString(catName, cmd, ss); }
      else if(cmd == "CategoryFiles") { readVString(fileCatName, cmd, ss); }
      else if(cmd == "VariableNames") { readVString(varName, cmd, ss); }
      else if(cmd == "BinCut") { readVInt(metBinCut, cmd, ss); }
      else if(cmd == "BackgroundSample") { push2Strings(bkgShortName, bkgLongName, cmd, ss); nBkg++; nSamples++; }
      else if(cmd == "SignalSample") { push2Strings(sigShortName, sigLongName, cmd, ss); nSig++; nSamples++; }
      else if(cmd == "Systematics") { readVString(systName, cmd, ss); }
      else if(cmd == "ControlRegion") {
	crNames.resize(catName.size());
	crSamples.resize(catName.size());
	nCr.resize(catName.size(), 0);
	ctrlRegions = true;
	string cat, cr, sample;
	ss >> cat >> cr >> sample;
	bool found = false;
	unsigned int ci;
	for(unsigned int i = 0; i < catName.size(); ++i) {
	  if(cat == catName[i]) { ci = i; found = true; break; }
	}
	if(!found) {
	  cerr << cmd << ": no category found: " << cat << endl;
	  exit(1001);
	}
	crNames[ci].push_back(cr);
	crSamples[ci].push_back(sample);
	nCr[ci]++;
	cout << cmd << " " << cat << " " << cr << " " << sample << endl;
      } else {
	cerr << "parse error: " << cmd << " " << option << endl;;
	exit(1001);
      }
    }
  }

  if(catName.size() != fileCatName.size() || catName.size() != varName.size() ||
     catName.size() != metBinCut.size()) {
    cerr << "inconsistent datacards" << endl;
    exit(1);
  }
  if(!system((string("mkdir -p ") + outDir).c_str())) { cerr <<"info: can't create " << outDir << endl; }
  
  nCat = catName.size();
  cout << "[datacard] categories: " << nCat << endl;
  shortName.insert(shortName.end(), bkgShortName.begin(), bkgShortName.end());
  shortName.insert(shortName.end(), sigShortName.begin(), sigShortName.end());
  longName.insert(longName.end(), bkgLongName.begin(), bkgLongName.end());
  longName.insert(longName.end(), sigLongName.begin(), sigLongName.end());

  nSyst = systName.size();
  syst = nSyst > 0;
  cout << "[datacard] systematics: " << nSyst << endl;
  yieldCutObs.resize(nCat); yieldNoCutObs.resize(nCat);
  yieldCut.resize(nCat); yieldNoCut.resize(nCat);
  yieldCut_up.resize(nCat); yieldNoCut_up.resize(nCat);
  yieldCut_down.resize(nCat); yieldNoCut_down.resize(nCat);
  systCount.resize(nCat);
  for(unsigned int i = 0; i < nCat; ++i) {
    yieldCut[i].resize(nSamples); yieldNoCut[i].resize(nSamples);
    yieldCut_up[i].resize(nSamples); yieldNoCut_up[i].resize(nSamples);
    yieldCut_down[i].resize(nSamples); yieldNoCut_down[i].resize(nSamples);
    systCount[i].resize(nSamples);
    for(unsigned int s = 0; s < nSamples; ++s) {
      yieldCut_up[i][s].resize(nSyst); yieldNoCut_up[i][s].resize(nSyst);
      yieldCut_down[i][s].resize(nSyst); yieldNoCut_down[i][s].resize(nSyst);
      systCount[i][s].resize(nSyst);
    }
  }
  return 0;
}
