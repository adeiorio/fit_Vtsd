#include <iostream>
#include <fstream>
#include <iomanip>
#include "common.h"

using namespace std;

void datacardHeader(bool syst, ofstream& datacard, unsigned int iMin, unsigned int iMax) {
  datacard << "imax " << iMax - iMin << " number of channels (" << catName[iMin];
  for(unsigned int i = iMin + 1; i < iMax; ++i) datacard << ", " << catName[i];
  datacard << ")" << endl
           << "jmax " << nBkg << " number of backgrounds (" << shortName[0];
  for(unsigned int i = 1; i < nBkg; ++i) datacard << ", " << shortName[i];
  datacard << ")" << endl
           << "kmax " << 1 + (syst ? nSyst : 0) << " number of nuisance parameters (lumi";
  if(syst) for(unsigned int s = 0; s < nSyst; ++s ) datacard << ", " << systName[s];
  datacard << ")" << endl
           << "---------------------" << endl;
}

void datacardObs(ofstream& datacard, bool cuts, unsigned int iMin, unsigned int iMax, 
		 bool dummy, unsigned int cr) {
  datacard << "bin\t";
  for(unsigned int i = iMin; i < iMax; ++i) {
    datacard << "\t" << catName[i];
    if(cr < 1000) datacard << "_" << crNames[iMin][cr] << endl;
  }
  datacard << endl;
  datacard << "observation";
  for(unsigned int i = iMin; i < iMax; ++i)
    datacard << "\t" << (dummy ? -1 : (cuts ? yieldCutObs[i] : yieldNoCutObs[i]));
  datacard << endl;
  datacard << "---------------------" << endl;
}

void datacardSyst(bool syst, bool shape, ofstream& datacard, int k, unsigned int iMin, unsigned int iMax, 
		  const vector<string> & samples) {
  datacard << "lumi\tlnN";
  double lumi = 1.048;
  for(unsigned int i = iMin; i < iMax; ++i) {
    datacard << "\t" << lumi;
    for(unsigned int j = 0; j < nBkg; ++j) datacard << "\t" << lumi;
  }
  datacard << std::setprecision(4) << endl;
  if(syst) {
    if(shape) {
      const double scale = 1;
      for(unsigned int s = 0; s < nSyst; ++s) {
	string ss = systName[s];
	if(systName[s].find("mcstat") == 0 && systName[s].find("DMtt") != string::npos){
	  ss.replace(ss.find("DMtt"), 4, shortName[k]);
	}
	datacard << ss << "\tshape";
	for(unsigned int i = iMin; i < iMax; ++i) {
	  for(unsigned int j = 0; j < nBkg + 1; ++j) {
	    datacard << "\t";
	    bool skip = false;
	    if(j > 0)
	      for(vector<string>::const_iterator smp = samples.begin(); smp != samples.end(); ++smp) {
		if (shortName[j-1] == *smp) skip = true; break;
	      }
	    if(systName[s].substr(0, 6) == "mcstat") {
	      skip = true;
	      if(j > 0) {
		if(systName[s].find(catName[i] + "_" + shortName[j-1] + "_bin") == 7) { skip = false; } 
	      } else {
		if(systName[s].find(catName[i]+"_DMtt") != string::npos) { skip = false; }
	      }
	    }
	    if (skip) { datacard << "-"; }
	    else { datacard << scale; }
	  }
	}
	datacard << endl;
      }
    } else {
      for(unsigned int s = 0; s < nSyst; ++s) {
	datacard << systName[s] << "\tlnN";
	for(unsigned int i = iMin; i < iMax; ++i) {
	  datacard << "\t" << systCount[i][k][s];
	  for(unsigned int j = 0; j < nBkg; ++j) {
	    datacard << "\t" << systCount[i][j][s];
	  }
        }
	datacard << endl;
      }
    }
  }
}

void datacardShapes(ofstream& datacard, bool ctrl) {
  datacard << "shapes\t*\t*\t../ws/workspace" << (ctrl ? "_ctrl" : "") 
	   << ".root\tw:$PROCESS_$CHANNEL\tw:$PROCESS_$CHANNEL_$SYSTEMATIC" << endl;
  datacard << "shapes\tdata_obs\t*\t../ws/workspace" << (ctrl ? "_ctrl" : "")
	   << ".root\tw:data_obs_$CHANNEL" << endl;
  datacard << "---------------------" << endl;  
}

void datacardRate(ofstream & datacard, const vector<vector<double> > yield, 
		  unsigned int k, unsigned int iMin, unsigned int iMax, 
		  bool dummy, unsigned int cr) {
  datacard << "bin";
  for(unsigned int i = iMin; i < iMax; ++i) for(unsigned int j = 0; j < nBkg+1; ++j) {
      datacard << "\t" << catName[i];
      if(cr < 1000) datacard << "_" << crNames[iMin][cr];
    }
  datacard << endl;
  datacard << "process";
  for(unsigned int i = iMin; i < iMax; ++i) {
    datacard << "\t" << shortName[k];
    for(unsigned int j = 0; j < nBkg; ++j) datacard << "\t" << shortName[j];
  }
  datacard << endl;
  datacard << "process";
  for(unsigned int i = iMin; i < iMax; ++i)
    for(unsigned int j = 0; j < nBkg+1; ++j) datacard << "\t" << j;
  datacard << endl;
  datacard << "rate" << std::setprecision(4);
  for(unsigned int i = iMin; i < iMax; ++i) {
    datacard << "\t" << (dummy ? -1 : yield[i][k]);
    for(unsigned int j = 0; j < nBkg; ++j) {
      if(cr < 1000 && string(crSamples[i][cr]) == string(shortName[j])) {
	datacard << "\t" << 1;
      } else if(dummy) {
	datacard << "\t" << -1;
      } else {
	datacard << "\t" << yield[i][j];
      }
    }
  }
  datacard << endl;
  datacard << "---------------------" << endl;  
}

void datacards(bool syst, const char * title, unsigned int k, const vector<vector<double> > & yield, 
	       unsigned int iMin, unsigned int iMax) {
  bool shape = string(title) == "shape";
  bool ctrl = string(title) == "ctrl";
  bool count = string(title) == "count";
  string fileNamePre = outDir + "/" + shortName[k]; 
  system((string("mkdir -p ")+fileNamePre).c_str());
  fileNamePre += "/"; fileNamePre += shortName[k];
  if(iMax - iMin == 1) { fileNamePre += "_"; fileNamePre += catName[iMin]; }
  fileNamePre +="_"; fileNamePre += title;
  string fileName = fileNamePre + (ctrl ? "_sigCat" : "") + ".txt";
  //  cout << ">>> opening datacard file " << fileName << endl;
  ofstream datacard(fileName.c_str());
  datacard << ">>> datacard for " << shortName[k];
  if(iMax - iMin == 1)  datacard << " " << catName[iMin];
  datacard << ", " << title << endl << endl;
  datacardHeader(syst, datacard, iMin, iMax);
  if(shape||ctrl) datacardShapes(datacard, ctrl);
  datacardObs(datacard, count, iMin, iMax);
  datacardRate(datacard, yield, k, iMin, iMax, false);
  vector<string> s; 
  if(ctrl) {
    for(unsigned int cr = 0; cr < nCr[iMin]; ++cr) s.push_back(string(crSamples[iMin][cr]));
  }
  datacardSyst(syst, shape || ctrl, datacard, k, iMin, iMax, s);
  datacard.close();
  // datacards file for control region
  if(ctrl) {
    s.clear();
    if(iMax - iMin != 1) { cerr << "can't generate ctrl samples datacards for multiple categories" << endl; exit(1); }
    for(unsigned int cr = 0; cr < nCr[iMin]; ++cr) {
      string fileNameCR = fileNamePre + "_" + crNames[iMin][cr] + "_ctrlCat.txt";
      ofstream datacardCR(fileNameCR.c_str());
      datacardCR << "# datacard for " << shortName[k] << " (control region: " << crNames[iMin][cr] << " )" << " " << catName[iMin];
      datacardCR << ", " << title << endl << endl;
      datacardHeader(syst, datacardCR, iMin, iMax);
      datacardShapes(datacardCR, true); //, string("_") + crNames[iMin][cr]);
      datacardObs(datacardCR, shape, iMin, iMax, true, cr);
      datacardRate(datacardCR, yield, k, iMin, iMax, true, cr);
      s.push_back(crSamples[iMin][cr]);
      datacardSyst(syst, true, datacardCR, k, iMin, iMax, s);
      /*
      datacardCR << "# free floating parameters, we do not need to declare them, but its a good idea to (? ? ?)" << endl;
      for(unsigned int i = 0; i < nBkg; ++i) {
	for(int b = 0; b < maxBins; ++b) {
	  datacardCR << "bkg_SR_" << shortName[i] << "_" << crNames[iMin][cr] << "_bin" << b << "\t flatParam" << endl; 
	}
      } 
      */
      datacardCR.close();
    }
  }
}

