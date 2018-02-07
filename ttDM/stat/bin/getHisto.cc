#include "TFile.h"
#include "TH1.h"
#include <string>
#include <vector>
#include <iostream>
#include "common.h"

using namespace std;

void getHisto(TH1 * & var, TFile * & file, const string & fileName, const string & vName) {
  //  cout << ">>> opening file: " << fileName << endl;                                                                                                             
  file = new TFile(fileName.c_str(), "OPEN");
  if(file == 0) { cerr << "can't open " << fileName << endl; exit(1); }
  file->GetObject(vName.c_str(), var);
  if(var == 0) { cerr << "cant' find " << vName << " in " << fileName << endl; exit(1); }
  var->SetDirectory(0);
  file->Close();
  delete file;
}

void getHistoCR(vector<TH1*> & cr, TFile * & file, const string & fileName, const string & vName, unsigned int iCat) {
  //  cout << ">>> opening file: " << fileName << endl;
  file = new TFile(fileName.c_str(), "OPEN");
  if(file == 0) { cerr << "can't open " << fileName << endl; exit(1); }
  file->GetObject(vName.c_str(), cr[0]);
  if(cr[0] == 0) { cerr << "cant' find " << vName << " in " << fileName << endl; exit(1); }
  cr[0]->SetDirectory(0);
  for(unsigned int i = 0; i < nCr[iCat]; ++i) {
    file->GetObject((vName+"_"+crNames[iCat][i]).c_str(), cr[i+1]);
    if(cr[i+1] == 0) { cerr << "cant' find " << (vName+"_"+crNames[iCat][i]) << " in " << fileName << endl; exit(1); }
    cr[i+1]->SetDirectory(0);
  }
  file->Close();
  delete file;
}
