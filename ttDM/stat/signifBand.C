#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include <algorithm>
#include <iostream>
using namespace std;

void signifBand(const char * fileName) {
  TFile f(fileName);
  TTree * tree = dynamic_cast<TTree*>(f.Get("limit"));
  double pValue;
  tree->SetBranchAddress("limit", &pValue);
  Long64_t nEntries = tree->GetEntries();
  vector<double> v(nEntries);
  for (Long64_t i = 0; i < nEntries; ++i) {
    tree->GetEntry(i);
    double signif = TMath::NormQuantile(1-TMath::Max(pValue, 1.0e-15));
    v[i] = signif;
  }
  sort(v.begin(), v.end());
  cout << "Expected signif:  2.5%: r < " << v[nEntries*0.025] << endl;
  cout << "Expected signif: 16.0%: r < " << v[nEntries*0.16] << endl;
  cout << "Expected signif: 50.0%: r < " << v[nEntries*0.5] << endl;
  cout << "Expected signif: 84.0%: r < " << v[nEntries*0.84] << endl;
  cout << "Expected signif: 97.5%: r < " << v[nEntries*0.975] << endl;
}
