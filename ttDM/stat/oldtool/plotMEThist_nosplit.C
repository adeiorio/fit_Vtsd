#include "TFile.h"
#include "TH1.h"
#include "TTree.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TMath.h"
#include "TStyle.h"
#include "TLegend.h"
#include "RooWorkspace.h"
#include "TSystem.h"
#include "RooHistPdf.h"
#include "RooRealVar.h"
#include "RooDataHist.h"
#include "RooRandom.h"
#include "RooAddition.h"
#include "RooFormulaVar.h"
#include "RooMsgService.h"
#include "HiggsAnalysis/CombinedLimit/interface/RooParametricHist.h"
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <sstream>
using namespace std;

const unsigned int nBkg =  6;
const unsigned int nSig = 14;
const unsigned int nSamples = nBkg + nSig;
const unsigned int nCat = 2;
const string catName[nCat] = { "semileptonic", "fullhadronic" };
const string fileCatName[nCat] = { "semileptonic", "fullhadronic" };
const string varName[nCat] = { "metFinal", "metFinal" };
// Cuts are expressed as bin number
const double metBinCut[nCat] = { 5, 5 };
const string directory = "histos";
//PUT BACKGROUND FIRST!
const string longName[nSamples] = { "ttbar", "single top", "Drell-Yan", "Z to #nu #nu", "W+jets", "QCD",
				    "tt+DM, mchi=1 GeV, mphi=10GeV", "tt+DM, mchi=10 GeV, mphi=10GeV", 
				    "tt+DM, mchi=1 GeV, mphi=20GeV",
				    "tt+DM, mchi=1 GeV, mphi=50GeV", "tt+DM, mchi=50 GeV, mphi=50GeV", 
				    "tt+DM, mchi=1 GeV, mphi=200GeV", "tt+DM, mchi=50 GeV, mphi=200GeV",  "tt+DM, mchi=150 GeV, mphi=200GeV", 
				    "tt+DM, mchi=50 GeV, mphi=300GeV",
				    "tt+DM, mchi=1 GeV, mphi=500GeV", "tt+DM, mchi=150 GeV, mphi=500GeV",  "tt+DM, mchi=500 GeV, mphi=500GeV", 
				    "tt+DM, mchi=1 GeV, mphi=1000GeV", "tt+DM, mchi=150 GeV, mphi=1000GeV" };
const string shortName[nSamples] = { "TT", "SingleTop", "DY", "ZToNuNu", "WJets", "QCD",
				     "DMtt_ps_Mchi1Mphi10", "DMtt_ps_Mchi10Mphi10",
				     "DMtt_ps_Mchi1Mphi20",
				     "DMtt_ps_Mchi1Mphi50", "DMtt_ps_Mchi50Mphi50",
				     "DMtt_ps_Mchi1Mphi200", "DMtt_ps_Mchi50Mphi200", "DMtt_ps_Mchi150Mphi200", 
				     "DMtt_ps_Mchi50Mphi300",
				     "DMtt_ps_Mchi1Mphi500", "DMtt_ps_Mchi150Mphi500", "DMtt_ps_Mchi500Mphi500",
				     "DMtt_ps_Mchi1Mphi1000", "DMtt_ps_Mchi150Mphi1000" };
Int_t lineCol[nBkg+1] = { kRed + 2,  kMagenta + 4, kGray + 2,
			  kBlue };
Int_t fillCol[nBkg+1] = { kRed - 7,  kMagenta - 3, kGray + 1,
			  kBlue };

const string crNames_0[] = { /* "2lep", "met_0btag" */ };
const string crSamples_0[] = { /* "TT", "WJets" */ };
const string crNames_1[] = { /* "outtop", */ "SR_1lep"  };
const string crSamples_1[] = { /* "QCD", */ "TT" };
const unsigned int nCr[nCat] = { sizeof(crNames_0)/sizeof(string), sizeof(crNames_1)/sizeof(string) };
const string * crNames[nCat] = { crNames_0, crNames_1 };
const string * crSamples[nCat] = { crSamples_0, crSamples_1 };

const unsigned int nSyst = 4;
const string systName[nSyst] = { "btag", "jes", "mistag", "pu" };

const bool savePNG = false;

//WARNING: met cuts are given as bin number, not in GeV!

double yieldCutObs[nCat], yieldNoCutObs[nCat];
double yieldCut[nCat][nSamples], yieldNoCut[nCat][nSamples];
double yieldCut_up[nCat][nSamples][nSyst], yieldNoCut_up[nCat][nSamples][nSyst];
double yieldCut_down[nCat][nSamples][nSyst], yieldNoCut_down[nCat][nSamples][nSyst];

double systCount[nCat][nSamples][nSyst];

/*
TH1 * plot(bool syst, int iCat, int iSig, vector<TH1*> hh[nSamples], vector<TH1*> hhup[nSamples][nSyst], vector<TH1*> hhdown[nSamples][nSyst],
	   double x1, double y1, double x2, double y2, const string & plotName) {
  const int k = nBkg+iSig;
  TH1* h[nBkg+1], *hup[nBkg+1][nSyst], *hdown[nBkg+1][nSyst];
  for(unsigned int i = 0; i < nBkg; ++i) {
    string name = string(hh[i][0]->GetTitle()) + "_" + shortName[i] + "_" + shortName[k] +
      "_" + catName[iCat] + "_tmp";
    h[i] = dynamic_cast<TH1*>(hh[i][0]->Clone(name.c_str()));
    if(syst) {
      for(unsigned int s = 0; s < nSyst; ++s) {
	name += "_"; name += systName[s];
	hup[i][s] = dynamic_cast<TH1*>(hhup[i][s][0]->Clone((name+"Up").c_str()));
	hdown[i][s] = dynamic_cast<TH1*>(hhdown[i][s][0]->Clone((name+"Down").c_str()));
      }
    }
  }
  string name = string(hh[k][0]->GetTitle()) + "_" + "_" + shortName[k] +
    "_" + catName[iCat] + "_sig_tmp";
  h[nBkg] = dynamic_cast<TH1*>(hh[k][0]->Clone(name.c_str()));
  if(syst) {
    for(unsigned int s = 0; s < nSyst; ++s) {
      name += "_"; name += systName[s];
      hup[nBkg][s] = dynamic_cast<TH1*>(hhup[k][s][0]->Clone((name+"Up").c_str()));
      hdown[nBkg][s] = dynamic_cast<TH1*>(hhdown[k][s][0]->Clone((name+"Down").c_str()));
    }
  }
  for(unsigned int i = 0; i < nBkg; ++i) {
    for(unsigned int j = i + 1; j < nBkg; ++j) {
      h[i]->Add(h[j]);
      if(syst) {
	for(unsigned int s = 0; s < nSyst; ++s) {
          hup[i][s]->Add(hup[j][s]);
	  hdown[i][s]->Add(hdown[j][s]);
        }
      }
    }
    h[i]->Add(h[nBkg]);
    h[i]->SetLineWidth(2);
    h[i]->SetLineStyle(kSolid);
    h[i]->SetFillStyle(1001);
    h[i]->SetLineColor(lineCol[i]);
    h[i]->SetFillColor(fillCol[i]);
    if(syst) {
      for(unsigned int s = 0; s < nSyst; ++s) {
	hup[i][s]->Add(hup[nBkg][s]);
	hdown[i][s]->Add(hdown[nBkg][s]);
	hup[i][s]->SetFillStyle(0);
	hdown[i][s]->SetFillStyle(0);
	hup[i][s]->SetLineColor(lineCol[i]);
	hdown[i][s]->SetLineColor(lineCol[i]);
	hup[i][s]->SetLineStyle(2);
	hdown[i][s]->SetLineStyle(2);
	hup[i][s]->SetLineWidth(2);
        hdown[i][s]->SetLineWidth(2);
      }
    }
  }
  h[nBkg]->SetLineWidth(2);
  h[nBkg]->SetLineStyle(kSolid);
  h[nBkg]->SetFillStyle(1001);
  h[nBkg]->SetLineColor(lineCol[nBkg]);
  h[nBkg]->SetFillColor(fillCol[nBkg]);
  if(syst) {
    for(unsigned int s = 0; s < nSyst; ++s) {
      hup[nBkg][s]->SetFillStyle(0);
      hdown[nBkg][s]->SetFillStyle(0);
      hup[nBkg][s]->SetLineColor(lineCol[nBkg]);
      hdown[nBkg][s]->SetLineColor(lineCol[nBkg]);
      hup[nBkg][s]->SetLineStyle(2);
      hdown[nBkg][s]->SetLineStyle(2);
      hup[nBkg][s]->SetLineWidth(2);
      hdown[nBkg][s]->SetLineWidth(2);
    }
  }
  for(int logy = 0; logy <= 1; ++logy) {
    gStyle->SetOptLogy(logy);
    TCanvas * c = new TCanvas((string("c_")+shortName[k]).c_str());  
    h[0]->Draw("HIST");
    for(unsigned int i = 1; i < nBkg; ++i) h[i]->Draw("HIST SAME");
    h[nBkg]->Draw("HIST SAME");
    if(syst) {
      for(unsigned int s = 0; s < nSyst; ++s) {
	hup[0][s]->Draw("HIST SAME");
	hdown[0][s]->Draw("HIST SAME");
	hup[nBkg][s]->Draw("HIST SAME");
	hdown[nBkg][s]->Draw("HIST SAME");
      }
    }
    TLegend l(x1, y1, x2, y2);
    l.SetTextSize(0.04);
    for(unsigned int i = 0; i < nBkg; ++i) l.AddEntry(h[i], longName[i].c_str(), "l");
    l.AddEntry(h[nBkg], longName[k].c_str(), "l");
    l.Draw();
    if(!system("mkdir -p plots")) { cerr << "info: directory plots already exists" << endl; }
    string pname = string("plots/") + plotName+"_"+shortName[k]+"_"+catName[iCat];
    if(logy == 0) pname += "_lin";
    c->SaveAs((pname+".pdf").c_str());
    if(savePNG) c->SaveAs((pname+".png").c_str());
    delete c;
  }
  return h[0];
}
*/ 

void datacardHeader(bool syst, ofstream& datacard, unsigned int iMin = 0, unsigned int iMax = nCat) {
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

void datacardObs(ofstream& datacard, bool cuts, unsigned int iMin = 0, unsigned int iMax = nCat, 
		 bool dummy = false, unsigned int cr = 1000) {
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

void datacardSyst(bool syst, bool shape, ofstream& datacard, int k = -1, unsigned int iMin = 0, unsigned int iMax = nCat) {
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
	datacard << systName[s] << "\tshape";
	for(unsigned int i = iMin; i < iMax; ++i) {
	  for(unsigned int j = 0; j < nBkg + 1; ++j)
	    datacard << "\t" << scale;
	}
	datacard << endl;
      }
    } else {
      for(unsigned int s = 0; s < nSyst; ++s) {
	datacard << systName[s] << "\tlnN";
	for(unsigned int i = iMin; i < iMax; ++i) {
	  datacard << "\t" << systCount[i][k][s];
	  for(unsigned int j = 0; j < nBkg ; ++j) 
	    datacard << "\t" << systCount[i][j][s];
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

void datacardRate(ofstream & datacard, double yield[nCat][nSamples], unsigned int k, unsigned int iMin = 0, unsigned int iMax = nCat, 
		  bool dummy = false, unsigned int cr = 1000) {
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

void datacards(bool syst, const char * title, unsigned int k, double yield[nCat][nSamples], unsigned int iMin = 0, unsigned int iMax = nCat) {
  bool shape = string(title) == "shape";
  bool ctrl = string(title) == "ctrl";
  bool count = string(title) == "count";
  string fileNamePre = shortName[k]; 
  if(!system((string("mkdir -p ")+fileNamePre).c_str())) { cerr <<"info: directory " << fileNamePre << " already exists" << endl; }
  fileNamePre += "/"; fileNamePre += shortName[k];
  if(iMax - iMin == 1) { fileNamePre += "_"; fileNamePre += catName[iMin]; }
  fileNamePre +="_"; fileNamePre += title;
  string fileName = fileNamePre + (ctrl ? "_sigCat" : "") + ".txt";
  cout << ">>> opening datacard file " << fileName << endl;
  ofstream datacard(fileName.c_str());
  datacard << ">>> datacard for " << shortName[k];
  if(iMax - iMin == 1)  datacard << " " << catName[iMin];
  datacard << ", " << title << endl << endl;
  datacardHeader(syst, datacard, iMin, iMax);
  if(shape||ctrl) datacardShapes(datacard, ctrl);
  datacardObs(datacard, count, iMin, iMax);
  datacardRate(datacard, yield, k, iMin, iMax, false);
  datacardSyst(syst, shape || ctrl, datacard, k, iMin, iMax);
  datacard.close();
  // datacards file for control region
  if(ctrl) {
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
      datacardSyst(syst, true, datacardCR, k, iMin, iMax);
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

void getHisto(TH1 * & var, TFile * & file, const string & fileName, const string & vName) {
  cout << ">>> opening file: " << fileName << endl;
  file = new TFile(fileName.c_str(), "OPEN");
  if(file == 0) { cerr << "can't open " << fileName << endl; exit(1); }
  var = dynamic_cast<TH1*>(file->Get(vName.c_str()));
  if(var == 0) { cerr << "cant' find " << vName << " in " << fileName << endl; exit(1); }
}

void getHistoCR(vector<TH1*> & cr, TFile * & file, const string & fileName, const string & vName, unsigned int iCat) {
  cout << ">>> opening file: " << fileName << endl;
  file = new TFile(fileName.c_str(), "OPEN");
  if(file == 0) { cerr << "can't open " << fileName << endl; exit(1); }
  cr[0] = dynamic_cast<TH1*>(file->Get(vName.c_str()));
  if(cr[0] == 0) { cerr << "cant' find " << vName << " in " << fileName << endl; exit(1); }
  for(unsigned int i = 0; i < nCr[iCat]; ++i) {
    cr[i+1] = dynamic_cast<TH1*>(file->Get((vName+"_"+crNames[iCat][i]).c_str()));
    if(cr[i+1] == 0) { cerr << "cant' find " << (vName+"_"+crNames[iCat][i]) << " in " << fileName << endl; exit(1); }
  }
}

void plotMEThist(bool syst, int cat, bool ctrlRegions) {
  gSystem->Load("libRooFit");
  using namespace RooFit;
  RooMsgService::instance().setStreamStatus(1,false);
  //  RooRandom::randomGenerator()->SetSeed(3001);
  unsigned int iCatMin = 0, iCatMax = nCat;
  if(cat >= 0) { iCatMin = cat; iCatMax = cat + 1; }
  RooRealVar met("met", "met", 0, 3000);
  cout << ">>> creating workspace " << endl;
  RooWorkspace * w = new RooWorkspace("w") ;
  TFile * dataFile[nCat];
  TFile * files[nCat][nSamples], * files_syst_up[nCat][nSamples][nSyst], * files_syst_down[nCat][nSamples][nSyst];
  cout << ">>> importing histograms" << endl;
  for(unsigned int iCat = iCatMin; iCat < iCatMax; ++iCat) {  
    const string & catn = catName[iCat], & varn = varName[iCat], & fcatn = fileCatName[iCat];
    cout << ">>> importing category: " << catn << endl;
    vector<TH1 *> h_var[nSamples], h_var_syst_up[nSamples][nSyst], h_var_syst_down[nSamples][nSyst];
    vector<TH1 *> h_var_data;
    unsigned int numCr = 1 + (ctrlRegions ? nCr[iCat] : 0);
    cout << ">>> control regions: " << numCr - 1 << endl;
    h_var_data.resize(numCr);
    if(ctrlRegions) {
      getHistoCR(h_var_data, dataFile[iCat], directory+"/Data_" + fcatn + ".root", varn, iCat);
    } else {
      getHisto(h_var_data[0], dataFile[iCat], directory+"/Data_" + fcatn + ".root", varn);
    }
    for(unsigned int i = 0; i < nSamples; ++i) {
      h_var[i].resize(numCr); 
      h_var[i][0] = 0;
      if(syst) 
	for(unsigned int s = 0; s < nSyst; ++s) {
	  h_var_syst_up[i][s].resize(numCr); h_var_syst_down[i][s].resize(numCr);
	  h_var_syst_up[i][s][0] = h_var_syst_down[i][s][0] = 0;
	}
    }
    for(unsigned int i = 0; i < nSamples; ++i) {
      const string path = directory + "/" + shortName[i] + "_" + fcatn;
      if(ctrlRegions) {
	getHistoCR(h_var[i], files[iCat][i], path + ".root", varn, iCat);
      } else {
	getHisto(h_var[i][0], files[iCat][i], path + ".root", varn);
      }
      if(syst)
	for(unsigned int s = 0; s < nSyst; ++s) {
	  const string p1 = path + "_" + systName[s];
	  if(ctrlRegions) {
	    getHistoCR(h_var_syst_up[i][s], files_syst_up[iCat][i][s], p1 + "Up.root", varn, iCat);
	    getHistoCR(h_var_syst_down[i][s], files_syst_down[iCat][i][s], p1 + "Down.root", varn, iCat);
	  } else {
	    getHisto(h_var_syst_up[i][s][0], files_syst_up[iCat][i][s], p1 + "Up.root", varn);
	    getHisto(h_var_syst_down[i][s][0], files_syst_down[iCat][i][s], p1 + "Down.root", varn);
	  }
	}
    }
    cout << ">>> compute yields for cut and count datacards" << endl;
    TAxis * a = h_var[0][0]->GetXaxis();
    Int_t bins = a->GetNbins();
    Double_t xmin = a->GetXmin(), xmax = a->GetXmax();
    double metCut = xmin + metBinCut[iCat] * (xmax - xmin)/bins;
    yieldCutObs[iCat] = h_var_data[0]->Integral(metBinCut[iCat]+1, bins);
    for(unsigned int i = 0; i < nSamples; ++i) {
      yieldCut[iCat][i] = h_var[i][0]->Integral(metBinCut[iCat]+1, bins);
      cout << ">>> yield, " << longName[i] << ", MET > " << metCut << ", " << " = " << std::setprecision(5) << yieldCut[iCat][i] << endl;
      if(syst) {
        for(unsigned int s = 0; s < nSyst; ++s) {
	  yieldCut_up[iCat][i][s]  = h_var_syst_up[i][s][0]->Integral(metBinCut[iCat]+1, bins);
	  yieldCut_down[iCat][i][s]  = h_var_syst_down[i][s][0]->Integral(metBinCut[iCat]+1, bins);
	  if(yieldCut[iCat][i] > 0) {
	    systCount[iCat][i][s] = 1.0 + 
	      0.5 * (fabs(yieldCut_up[iCat][i][s] - yieldCut[iCat][i]) + fabs(yieldCut_down[iCat][i][s] - yieldCut[iCat][i])) / yieldCut[iCat][i];
	  } else {
	    systCount[iCat][i][s] = 1.0;
	  }
	  cout << ">>> syst [down-avg-up]: (" << systName[s] << ") " << yieldCut_down[iCat][i][s] << " - " << yieldCut[iCat][i] << " - " << yieldCut_up[iCat][i][s] 
	       << " -> " << systCount[iCat][i][s] << endl;
	}
      }
    }
    cout << ">>> compute total yields for normalization: no cut applied" << endl;
    yieldNoCutObs[iCat] = h_var_data[0]->Integral();
      cout << ">>> yield (obs): " << yieldNoCutObs[iCat] << ", w/ cuts: " << yieldCutObs[iCat] << endl;
    for(unsigned int i = 0; i < nSamples; ++i) {
      yieldNoCut[iCat][i] = h_var[i][0]->Integral();
      cout << ">>> total yield, " << longName[i] << " = " << std::setprecision(5) << yieldNoCut[iCat][i] << endl;
      if(syst) {
	for(unsigned int s = 0; s < nSyst; ++s) {
	  yieldNoCut_up[iCat][i][s] = h_var_syst_up[i][s][0]->Integral();
	  yieldNoCut_down[iCat][i][s] = h_var_syst_down[i][s][0]->Integral();
	  cout << ">>> syst [down-avg-up]: (" << systName[s] << ") " << yieldNoCut_down[iCat][i][s] << " - " << yieldNoCut[iCat][i] << " - " << yieldNoCut_up[iCat][i][s] << endl;
	}
      }
    }
    cout << ">>> create RooStats PDF model" << endl;
    vector<RooDataHist *> hist[nSamples], hist_syst_up[nSamples][nSyst], hist_syst_down[nSamples][nSyst];
    vector<RooAbsPdf *> pdf[nSamples], pdf_syst_up[nSamples][nSyst], pdf_syst_down[nSamples][nSyst];
    vector<RooDataHist *> data(numCr);
    int nBins = h_var[0][0]->GetNbinsX();
    cout << ">>> bins for signal and ctrl regions: " << nBins << endl;
    vector<RooRealVar *> ratioCRSR[nBins], ratioCRSR_up[nBins][nSyst], ratioCRSR_down[nBins][nSyst];
    for(int b = 0; b < nBins; ++b) { 
      ratioCRSR[b].resize(nCr[iCat]); 
      if(syst) for(unsigned int s = 0; s < nSyst; ++s) {
	  ratioCRSR_up[s][b].resize(nCr[iCat]); ratioCRSR_down[s][b].resize(nCr[iCat]); 
	}
    }
    RooArgList * bkg_SR_bins[nCr[iCat]], * bkg_CR_bins[nCr[iCat]];
    RooArgList * bkg_SR_bins_up[nCr[iCat]][nSyst], * bkg_SR_bins_down[nCr[iCat]][nSyst];
    RooArgList * bkg_CR_bins_up[nCr[iCat]][nSyst], * bkg_CR_bins_down[nCr[iCat]][nSyst];
    vector<RooRealVar*> srBins[nCr[iCat]], srBins_up[nCr[iCat]][nSyst], srBins_down[nCr[iCat]][nSyst];
    vector<RooFormulaVar*> crBins[nCr[iCat]], crBins_up[nCr[iCat]][nSyst], crBins_down[nCr[iCat]][nSyst];
    cout << ">>> importing data samples" << endl;
    for(unsigned int crs = 0; crs < numCr; ++crs) {
      string crN = (crs == 0 ? "" : string("_") + crNames[iCat][crs - 1]);	
      cout << ">>> importing data sample in " << (crs == 0 ? "signal region" : string(" control region ") + crNames[iCat][crs - 1]) << endl;
      data[crs] = new RooDataHist((string("data_obs") + "_" + catn + crN).c_str(), "observed data sample", met, h_var_data[crs]);
      w->import(*data[crs]);
    }
    if(ctrlRegions) {
      cout << ">>> creating bin utilities for RooStats/combine" << endl;
      for(unsigned int cr = 0; cr < nCr[iCat]; ++cr) {
	unsigned int is = 1000;
	for(unsigned int i = 0; i < nBkg; ++i) {
	  if (shortName[i] == crSamples[iCat][cr]) { is = i; break; }
	}
	const string & sn = shortName[is], & crn = crNames[iCat][cr];
	cout << ">>> CR/SR ratio [" << catn << "-" << crn << "-" << sn << "]: ";
	for(int b = 0; b < nBins; ++b) {
	  ostringstream ss; ss << b + 1;
	  double ratio = 1, d = h_var[is][0]->GetBinContent(b + 1);
	  if(d > 0) ratio = h_var[is][cr + 1]->GetBinContent(b + 1) / d;
	  cout << ratio << " ";
	  ratioCRSR[b][cr] = new RooRealVar(("ratioCRSR_" + catn + "_" + sn + "_" + crn + "_bin" + ss.str()).c_str(), 
					    ("Ratio CR/SR, " + catn + " => " + sn + ", " + crn).c_str(), ratio);
	  ratioCRSR[b][cr]->setConstant();
	  //	  w->import(*ratioCRSR[b][cr]);
	  if(syst) for(unsigned int s = 0; s < nSyst; ++s) {
	      const string & syn = systName[s];
	      double ratio_up = 1, d_up = h_var_syst_up[is][s][0]->GetBinContent(b + 1);
	      if(d > 0) ratio_up = h_var_syst_up[is][s][cr + 1]->GetBinContent(b + 1) / d_up;
	      double ratio_down = 1, d_down = h_var_syst_up[is][s][0]->GetBinContent(b + 1);
	      if(d_down > 0) ratio_down = h_var_syst_up[is][s][cr + 1]->GetBinContent(b + 1) / d_down;
	      ratioCRSR_up[b][s][cr] = new RooRealVar(("ratioCRSR_" + catn + "_" + sn + "_" + crn + "_" + syn + "Up_bin" + ss.str()).c_str(), 
						      ("Ratio CR/SR, " + catn + " => " + sn + ", " + syn + "Up, " + crn).c_str(), ratio_up);
	      ratioCRSR_down[b][s][cr] = new RooRealVar(("ratioCRSR_" + catn + "_" + sn + "_" + crn + "_" + syn + "Down_bin" + ss.str()).c_str(), 
							("Ratio CR/SR, " + catn + " => " + sn + ", " + syn + "Down, " + crn).c_str(), ratio_down);
	      ratioCRSR_up[b][s][cr]->setConstant();
	      ratioCRSR_down[b][s][cr]->setConstant();
	      //	      w->import(*ratioCRSR_up[b][s][cr]);
	      //	      w->import(*ratioCRSR_down[b][s][cr]);
	    }
	}
	cout << endl;
	bkg_SR_bins[cr] = new RooArgList; bkg_CR_bins[cr] = new RooArgList; 
	srBins[cr].resize(nBins); crBins[cr].resize(nBins);
	if(syst)
	  for(unsigned int s = 0; s < nSyst; ++s) {
	    srBins_up[cr][s].resize(nBins); srBins_down[cr][s].resize(nBins);
	    crBins_up[cr][s].resize(nBins); crBins_down[cr][s].resize(nBins);
	    bkg_SR_bins_up[cr][s] = new RooArgList; bkg_SR_bins_down[cr][s] = new RooArgList;
	    bkg_CR_bins_up[cr][s] = new RooArgList; bkg_CR_bins_down[cr][s] = new RooArgList;
	  }
	for(int b = 0; b < nBins; ++b) {
	  ostringstream ss; ss << b + 1;
	  srBins[cr][b] =  new RooRealVar(("bkg_SR_" + catn + "_" + crn +"_bin" + ss.str()).c_str(), 
					  ("Bkg. SR " + sn + " yield in SR " + crn + ", bin " + ss.str()).c_str(), 10, 0, 10000);
	  crBins[cr][b] = new RooFormulaVar(("bkg_CR_" + sn + "_"+ crn +"_bin" + ss.str()).c_str(),
					    ("Bkg. CR " + sn + " yield in SR " + crn + ", bin " + ss.str()).c_str(),
					    "@0*@1", RooArgList(*ratioCRSR[b][cr], * srBins[cr][b]));
	  bkg_SR_bins[cr]->add(*srBins[cr][b]);
	  bkg_CR_bins[cr]->add(*crBins[cr][b]);
	  if(syst)
	    for(unsigned int s = 0; s < nSyst; ++s) {
	      const string & syn = systName[s];
	      srBins_up[cr][s][b] =  new RooRealVar(("bkg_SR_" + catn + "_" + crn + "_" + sn + "Up_bin" + ss.str()).c_str(), 
						    ("Bkg. SR " + sn + " yield in SR " + crn + " " + sn +"Up, bin " + ss.str()).c_str(), 10, 0, 10000);
	      crBins_up[cr][s][b] = new RooFormulaVar(("bkg_CR_" + sn + "_" + crn + "_" + sn + "Up_bin" + ss.str()).c_str(),
						      ("Bkg. CR " + sn + " yield in SR " + crn + " " + sn + "Up, bin " + ss.str()).c_str(),
						      "@0*@1", RooArgList(*ratioCRSR_up[b][s][cr], * srBins_up[cr][s][b]));
	      srBins_down[cr][s][b] =  new RooRealVar(("bkg_SR_" + catn + "_" + crn + "_" + syn + "Down_bin" + ss.str()).c_str(), 
						      ("Bkg. SR " + sn + " yield in SR " + crn + " " + syn + "Down, bin " + ss.str()).c_str(), 10, 0, 10000);
	      crBins_down[cr][s][b] = new RooFormulaVar(("bkg_CR_" + sn + "_"+ crn + "_" + syn + "Down_bin" + ss.str()).c_str(),
							("Bkg. CR " + sn + " yield in SR " + crn + " " + syn + "Down, bin " + ss.str()).c_str(),
							"@0*@1", RooArgList(*ratioCRSR_down[b][s][cr], * srBins_up[cr][s][b]));
	      bkg_SR_bins_up[cr][s]->add(*srBins_up[cr][s][b]); bkg_SR_bins_down[cr][s]->add(*srBins_down[cr][s][b]);
	      bkg_CR_bins_up[cr][s]->add(*crBins_up[cr][s][b]); bkg_CR_bins_down[cr][s]->add(*crBins_down[cr][s][b]);
	    }
	}
      }	    
    }
    
    // import background processes from MC or data in control regions
    for(unsigned int i = 0; i < nSamples; ++i) {
      const string & sn = shortName[i];
      const string name = sn + "_" + catn;
      cout << ">>> importing sample " << name << endl;
      hist[i].resize(numCr);
      pdf[i].resize(numCr);
      if(syst) for(unsigned int s = 0; s < nSyst; ++s) {
	  hist_syst_up[i][s].resize(numCr); hist_syst_down[i][s].resize(numCr);
	  pdf_syst_up[i][s].resize(numCr); pdf_syst_down[i][s].resize(numCr);
	}
      for(unsigned int crs = 0; crs < numCr; ++crs) { // crs == 0 => signal region, else control regions
	string crN = (crs == 0 ? "": string("_") + crNames[iCat][crs - 1]);
	string sncr = sn + crN;
	cout << ">>> importing samples in " << (crs == 0 ? "signal region" : string("control region ") + crNames[iCat][crs - 1]) << endl;
     	hist[i][crs] = new RooDataHist((name + crN + "_hist").c_str(), longName[i].c_str(), met, h_var[i][crs]);
	bool dataDriven = false;
	unsigned int crSample = 0;
	if(ctrlRegions && crs == 0) { // signal region
	  for(unsigned int cr = 0; cr < nCr[iCat]; ++cr) {
	    if(crSamples[iCat][cr] == sn) {
	      dataDriven = true; crSample = cr; break;
	    }
	  }
	}
	if(dataDriven) {
	  cout << ">>> importing data-driven shape for " << name + crN << endl;
	  pdf[i][crs] = new RooParametricHist((name + crN).c_str(),
					      ("Background PDF in signal region - " + sncr).c_str(), 
					      met, * bkg_SR_bins[crSample], * h_var_data[crs]);
	  RooAddition p_bkg_norm((name + crN + "_norm").c_str(), ("Tot number of events from bkg in SR - " + crNames[iCat][crSample]).c_str(), 
				 * bkg_SR_bins[crSample]);
	  w->import(p_bkg_norm, RooFit::RecycleConflictNodes());
	} else if(crs > 0 && crSamples[iCat][crs - 1] == sn) { // control region
	  cout << ">>> importing control region shape for " << name + crN << endl;
	  pdf[i][crs] = new RooParametricHist((name + crN).c_str(),
					      ("Background PDF in control region - " + sncr).c_str(),
					      met, * bkg_CR_bins[crs - 1], * h_var_data[crs]);
	  RooAddition p_CRbkg_norm((name + crN + "_norm").c_str(), ("Total Number of events from bkg in CR - " + crN).c_str(),
				   * bkg_CR_bins[crs - 1]);
	  cout << ">>> pdf name: " << pdf[i][crs]->GetName() << endl;
	  w->import(p_CRbkg_norm, RooFit::RecycleConflictNodes());
	} else { // all other cases
	  cout << ">>> importing MC simulation shape for " << name + crN << endl;
	  pdf[i][crs] = new RooHistPdf((name + crN).c_str(), longName[i].c_str(), met, *(hist[i][crs]), 0);
	}
	w->import(*pdf[i][crs], RooFit::RecycleConflictNodes());
	// systematic samples
	if(syst) {
	  for(unsigned int s = 0; s < nSyst; ++s) {
	    hist_syst_up[i][s][crs] = new RooDataHist((name + crN + "_" + systName[s] + "Up_hist").c_str(), longName[i].c_str(), met, h_var_syst_up[i][s][crs]);
	    hist_syst_down[i][s][crs] = new RooDataHist((name + crN + "_" + systName[s] + "Down_hist").c_str(), longName[i].c_str(), met, h_var_syst_down[i][s][crs]);
	    if(dataDriven) {
	      pdf_syst_up[i][s][crs] = new RooParametricHist((name + crN + "_" + systName[s] + "Up").c_str(),
							     ("Background PDF in signal region - " + sncr  + "_" + systName[s] + "Up").c_str(), 
							     met, * bkg_SR_bins_up[crSample][s], * h_var_data[crs]);
	      pdf_syst_down[i][s][crs] = new RooParametricHist((name + crN + "_" + systName[s] + "Down").c_str(),
							       ("Background PDF in signal region - " + sncr + "_" + systName[s] + "Down").c_str(), 
							       met, * bkg_SR_bins_down[crSample][s], * h_var_data[crs]);
	      RooAddition p_bkg_norm_up((name + crN + "_" + systName[s] + "Up_norm").c_str(), 
					("Tot number of events from bkg in SR - " + crNames[iCat][crSample] + "_" + systName[s] + "Up").c_str(), 
					* bkg_SR_bins_up[crSample][s]);
	      RooAddition p_bkg_norm_down((name + crN + "_" + systName[s] + "Down_norm").c_str(), 
					  ("Tot number of events from bkg in SR - " + crNames[iCat][crSample] + "_" + systName[s] + "Down").c_str(), 
					  * bkg_SR_bins_down[crSample][s]);
	      w->import(p_bkg_norm_up, RooFit::RecycleConflictNodes());
	      w->import(p_bkg_norm_down, RooFit::RecycleConflictNodes());
	    } else if(crs > 0 && crSamples[iCat][crs - 1] == sn) { // control region
	      pdf_syst_up[i][s][crs] = new RooParametricHist ((name + crN + "_" + systName[s] + "Up").c_str(),
							      ("Background PDF in control region - " + sncr + "_" + systName[s] + "Up").c_str(),
							      met, * bkg_CR_bins_up[crs - 1][s], * h_var_data[crs]);
	      pdf_syst_down[i][s][crs] = new RooParametricHist ((name + crN + "_" + systName[s] + "Down").c_str(),
								("Background PDF in control region - " + sncr + "_" + systName[s] + "Down").c_str(),
								met, * bkg_CR_bins_down[crs - 1][s], * h_var_data[crs]);
	      RooAddition p_CRbkg_norm_up((name + crN + "_" + systName[s] + "Up_norm").c_str(), 
					  ("Total Number of events from bkg in CR - " + crN + "_" + systName[s] + "Up").c_str(),
					  * bkg_CR_bins_up[crs - 1][s]);
	      RooAddition p_CRbkg_norm_down((name + crN + "_" + systName[s] + "Down_norm").c_str(), 
					    ("Total Number of events from bkg in CR - " + crN + "_" + systName[s] + "Down").c_str(),
					    * bkg_CR_bins_down[crs - 1][s]);
	      w->import(p_CRbkg_norm_up, RooFit::RecycleConflictNodes());
	      w->import(p_CRbkg_norm_down, RooFit::RecycleConflictNodes());
	    } else { // all other cases
	      pdf_syst_up[i][s][crs] = new RooHistPdf((name + crN + "_" + systName[s] + "Up").c_str(), longName[i].c_str(), met, *(hist_syst_up[i][s][crs]), 0);
	      pdf_syst_down[i][s][crs] = new RooHistPdf((name + crN + "_" + systName[s] + "Down").c_str(), longName[i].c_str(), met, *(hist_syst_down[i][s][crs]), 0);
	    }
	    w->import(*pdf_syst_up[i][s][crs], RooFit::RecycleConflictNodes());
	    w->import(*pdf_syst_down[i][s][crs], RooFit::RecycleConflictNodes());
	  }
	}
      }
    }
  }
  if(!system("mkdir -p ws")) { cerr <<"info: directory ws already exists" << endl; }
  string wfname = string("ws/workspace");
  if(ctrlRegions) wfname += "_ctrl";
  wfname += ".root";
  cout << ">>> creating workspace file " << wfname << endl;
  TFile fWorkspace(wfname.c_str(),"RECREATE");
  fWorkspace.cd();
  w->Write();
  fWorkspace.Close();
  for(unsigned int iCat = iCatMin; iCat < iCatMax; ++iCat) {  
    for(unsigned int i = 0; i < nSamples; ++i) {
      files[iCat][i]->Close();
      if(syst)
	for(unsigned int s = 0; s < nSyst; ++s) {
	  files_syst_up[iCat][i][s]->Close();
	  files_syst_down[iCat][i][s]->Close();
	}
    }
  }
  
  // make datacards
  cout << ">>> creating datacard files" << endl;
  for(unsigned int j = 0; j < nSig; ++j) {
    const unsigned int k = nBkg+j;
    if(!ctrlRegions) {
      datacards(syst, "count", k, yieldCut);
      datacards(syst, "shape", k, yieldNoCut);
    }
    // control region datacards should be merged using combineCards.py
    // don't attempt to merge them in this script
    // datacards(syst, "ctrl", k, yieldNoCut);
    for(unsigned int i = 0; i < nCat; ++i) {
      if(ctrlRegions)
	datacards(syst, "ctrl", k, yieldNoCut, i, i + 1);
      else {
	datacards(syst, "count", k, yieldCut, i, i + 1);
	datacards(syst, "shape", k, yieldNoCut, i, i + 1);
      }
    }
  }
}
