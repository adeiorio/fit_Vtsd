#include "TH1.h"
#include "TFile.h"
#include <iostream>
#include <sstream>
using namespace std;

const string varName = "metFinal";

void statSys(const char * fileName, const char* crName, int ibin, double scale=1) {
  TFile * file = new TFile(fileName, "OPEN");
  if(file == 0) { cerr << "can't open " << fileName << endl; exit(1); }
  TH1 * var = dynamic_cast<TH1*>(file->Get(varName.c_str()));
  if(var == 0) { cerr << "cant' find " << varName << " in " << fileName << endl; exit(1); }
  double bin = var->GetBinContent(ibin);
  double err = var->GetBinError(ibin);
  cout << bin << " +/- " << err << endl;
  string s(fileName);
  stringstream o;
  o << s.substr(0, s.find("."))  << "_" << crName << "bin" << ibin;
  TFile * outUp = new TFile((o.str() + "Up.root").c_str(), "RECREATE");
  TH1 * cloneUp = dynamic_cast<TH1*>(var->Clone());
  cloneUp->SetBinContent(ibin,bin+err*scale);
  outUp->cd();
  cloneUp->Write();
  outUp->Close();
  TFile * outDown = new TFile((o.str() + "Down.root").c_str(), "RECREATE");
  TH1 * cloneDown = dynamic_cast<TH1*>(var->Clone());
  cloneDown->SetBinContent(ibin,bin-err*scale);
  outDown->cd();
  cloneDown->Write();
  outDown->Close();
}
