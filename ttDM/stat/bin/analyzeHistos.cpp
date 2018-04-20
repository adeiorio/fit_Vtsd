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
#include <boost/program_options.hpp>
#include "common.h"
using namespace std;

void getHisto(TH1 * & var, TFile * & file, const string & fileName, const string & vName);
void getHistoCR(vector<TH1*> & cr, TFile * & file, const string & fileName, const string & vName, unsigned int iCat);
int parseArgs(int argc, char* argv[]);

int main(int argc, char *argv[]) {
  int err = parseArgs(argc, argv);
  if(err != 0) exit(err);
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
      getHistoCR(h_var_data, dataFile[iCat], histoDir+"/Data_" + fcatn + ".root", varn, iCat);
    } else {
      getHisto(h_var_data[0], dataFile[iCat], histoDir+"/Data_" + fcatn + ".root", varn);
    }
    cout << ">>> retrieving histograms files" << endl;
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
      const string & sn = shortName[i];
      const string path = histoDir+"/" + sn + "_" + fcatn;
      if(ctrlRegions) {
	getHistoCR(h_var[i], files[iCat][i], path + ".root", varn, iCat);
      } else {
	getHisto(h_var[i][0], files[iCat][i], path + ".root", varn);
      }
      if(syst)
	for(unsigned int s = 0; s < nSyst; ++s) {
	  const string & systn = systName[s];
	  const string p1 = path + "_" + systName[s];
	  if(ctrlRegions) {
	    getHistoCR(h_var_syst_up[i][s], files_syst_up[iCat][i][s], p1 + "Up.root", varn, iCat);
	    getHistoCR(h_var_syst_down[i][s], files_syst_down[iCat][i][s], p1 + "Down.root", varn, iCat);
	  } else {
	    if(systName[s].substr(0, 6) == "mcstat") {
	      string varn_syst_up = varn, varn_syst_down = varn;
	      //istringstream is(catn);
	      //string catn_short;
	      //getline(is, catn_short, '_');
	      if(systn.find(catn + "_" + sn + "_bin") == 7 || (sn.find("DMtt") == 0 && systn.find(catn + "_DMtt") == 7)) {
		string ss = systn;
		//ss.replace(systn.find(catn_short), catn_short.size(), catn);
		if(sn.find("DMtt") == 0 ) ss.replace(systn.find("DMtt"), 4, sn);
		varn_syst_up = varn + "_" + ss + "_Up";
		varn_syst_down = varn + "_" + ss + "_Down";
		std::cout<<"+++ MC statistics: "<<varn_syst_up<<std::endl;

	      }
	      getHisto(h_var_syst_up[i][s][0], files_syst_up[iCat][i][s], path + ".root", varn_syst_up);
	      getHisto(h_var_syst_down[i][s][0], files_syst_down[iCat][i][s], path + ".root", varn_syst_down);
	    } else {
	      getHisto(h_var_syst_up[i][s][0], files_syst_up[iCat][i][s], p1 + "Up.root", varn);
	      getHisto(h_var_syst_down[i][s][0], files_syst_down[iCat][i][s], p1 + "Down.root", varn);
	    }
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
    cout << ">>> bins in histograms: " << nBins << endl;
    vector<RooRealVar *> ratioCRSR[nBins], ratioCRSR_slope[nBins][nSyst];
    vector<RooRealVar *> systParms;
    vector<RooArgList *> bkg_SR_bins, bkg_CR_bins;
    vector<vector<RooRealVar*> > srBins; //, srBins_up[nCr[iCat]][nSyst], srBins_down[nCr[iCat]][nSyst];
    vector<vector<RooFormulaVar*> > crBins; //, crBins_up[nCr[iCat]][nSyst], crBins_down[nCr[iCat]][nSyst];
    if(syst) {
      for(unsigned int s = 0; s < nSyst; ++s) {
	systParms.push_back(new RooRealVar(systName[s].c_str(), systName[s].c_str(), -5, 5));
      }
    }
    if(ctrlRegions) {
      for(int b = 0; b < nBins; ++b) { 
	ratioCRSR[b].resize(nCr[iCat]); 
	if(syst) 
	  for(unsigned int s = 0; s < nSyst; ++s) {
	    ratioCRSR_slope[b][s].resize(nCr[iCat]);
	  }
      }
      bkg_SR_bins.resize(nCr[iCat]); bkg_CR_bins.resize(nCr[iCat]);
      srBins.resize(nCr[iCat]); crBins.resize(nCr[iCat]);
    }
    cout << ">>> importing data samples" << endl;
    for(unsigned int crs = 0; crs < numCr; ++crs) {
      string crN = (crs == 0 ? "" : string("_") + crNames[iCat][crs - 1]);	
      cout << ">>> importing data sample in " << (crs == 0 ? "signal region" : string(" control region ") + crNames[iCat][crs - 1]) << endl;
      data[crs] = new RooDataHist((string("data_obs") + "_" + catn + crN).c_str(), "observed data sample", met, h_var_data[crs]);
      w->import(*data[crs]);
    }
    if(ctrlRegions) {
      cout << ">>> creating bin utilities for RooStats/combine" << endl;
      cout << "CR/SR ratio: ";
      for(unsigned int cr = 0; cr < nCr[iCat]; ++cr) {
	unsigned int is = 1000;
	for(unsigned int i = 0; i < nBkg; ++i) {
	  if (shortName[i] == crSamples[iCat][cr]) { is = i; break; }
	}
	const string & sn = shortName[is], & crn = crNames[iCat][cr];
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
	      double delta_up = h_var_syst_up[is][s][cr + 1]->GetBinContent(b + 1) - h_var_syst_up[is][s][0]->GetBinContent(b + 1);
	      double delta_down = h_var_syst_up[is][s][0]->GetBinContent(b + 1) - h_var_syst_down[is][s][cr + 1]->GetBinContent(b + 1);
	      double ratio_slope = 0.5*(delta_up + delta_down);
	      ratioCRSR_slope[b][s][cr] = new RooRealVar(("ratioCRSR_" + catn + "_" + sn + "_" + crn + "_" + syn + "Slope_bin" + ss.str()).c_str(), 
							 ("Ratio CR/SR, " + catn + " => " + sn + ", " + syn + "Slope, " + crn).c_str(), ratio_slope);
	      ratioCRSR_slope[b][s][cr]->setConstant();
	    }
	}
	cout << endl;
	bkg_SR_bins[cr] = new RooArgList; bkg_CR_bins[cr] = new RooArgList; 
	srBins[cr].resize(nBins); crBins[cr].resize(nBins);
	/*	if(syst)
		for(unsigned int s = 0; s < nSyst; ++s) {
		srBins_up[cr][s].resize(nBins); srBins_down[cr][s].resize(nBins);
		crBins_up[cr][s].resize(nBins); crBins_down[cr][s].resize(nBins);
		bkg_SR_bins_up[cr][s] = new RooArgList; bkg_SR_bins_down[cr][s] = new RooArgList;
		bkg_CR_bins_up[cr][s] = new RooArgList; bkg_CR_bins_down[cr][s] = new RooArgList;
		} */
	for(int b = 0; b < nBins; ++b) {
	  ostringstream ss; ss << b + 1;
	  srBins[cr][b] =  new RooRealVar(("bkg_SR_" + catn + "_" + crn +"_bin" + ss.str()).c_str(), 
					  ("Bkg. SR " + sn + " yield in SR " + crn + ", bin " + ss.str()).c_str(), 10, 0, 10000);
	  if(syst) {
	    string formula = "@0*(";
	    RooArgList args;
	    args.add(*srBins[cr][b]);
	    unsigned int sc = 1;
	    for(unsigned int s = 0; s < nSyst; ++s) {
	      ostringstream ssc1; ssc1 << sc; formula += "@" + ssc1.str(); 
	      ostringstream ssc2; ssc2 << ++sc; formula += "*@" + ssc2.str();
	      args.add(*systParms[s]);
	      args.add(*ratioCRSR_slope[b][s][cr]);
	      if(s < nSyst - 1) formula += " + ";
	    }
	    formula += ")";
	    crBins[cr][b] = new RooFormulaVar(("bkg_CR_" + sn + "_"+ crn +"_bin" + ss.str()).c_str(),
					      ("Bkg. CR " + sn + " yield in SR " + crn + ", bin " + ss.str()).c_str(),
					      formula.c_str(), args);
	  } else {
	    crBins[cr][b] = new RooFormulaVar(("bkg_CR_" + sn + "_"+ crn +"_bin" + ss.str()).c_str(),
					      ("Bkg. CR " + sn + " yield in SR " + crn + ", bin " + ss.str()).c_str(),
					      "@0*@1", RooArgList(* srBins[cr][b], *ratioCRSR[b][cr]));
	  }
	  bkg_SR_bins[cr]->add(*srBins[cr][b]);
	  bkg_CR_bins[cr]->add(*crBins[cr][b]);
	}
      }
    }
    
    // import background processes from MC or data in control regions
    for(unsigned int i = 0; i < nSamples; ++i) {
      const string & sn = shortName[i];
      const string name = sn + "_" + catn;
      cout << ">>> importing sample " << name << endl;
      hist[i].resize(numCr, 0);
      pdf[i].resize(numCr, 0);
      if(syst) for(unsigned int s = 0; s < nSyst; ++s) {
	  hist_syst_up[i][s].resize(numCr); hist_syst_down[i][s].resize(numCr);
	  pdf_syst_up[i][s].resize(numCr, 0); pdf_syst_down[i][s].resize(numCr, 0);
	}
      for(unsigned int crs = 0; crs < numCr; ++crs) { // crs == 0 => signal region, else control regions
	string crN = (crs == 0 ? "": string("_") + crNames[iCat][crs - 1]);
	string sncr = sn + crN;
	cout << ">>> importing samples in " << (crs == 0 ? "signal region" : string("control region ") + crNames[iCat][crs - 1]) << endl;
     	hist[i][crs] = new RooDataHist((name + crN /*+ "_hist" */).c_str(), longName[i].c_str(), met, h_var[i][crs]);
	w->import(*hist[i][crs]);
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
	  //	  pdf[i][crs] = new RooHistPdf((name + crN).c_str(), longName[i].c_str(), met, *(hist[i][crs]), 0);
	}
	// this can be moved up in the if cycle
	if(pdf[i][crs] != 0)
	  w->import(*pdf[i][crs], RooFit::RecycleConflictNodes());
	// systematic samples
	if(syst && ! dataDriven) {
	  for(unsigned int s = 0; s < nSyst; ++s) {
	    bool skip = false;
	    string ss = systName[s];
	    //istringstream is(catn);
	    //string catn_short;
	    //getline(is, catn_short, '_');
	    if(ss.substr(0, 6) == "mcstat") {
	      skip = true;
	      std::cout<<"---Category: "<<catn<<std::endl;

	      if(ss.find(catn + "_" + sn + "_bin") == 7 || (sn.find("DMtt") == 0 && ss.find(catn + "_DMtt") == 7)) {
		skip = false;
		if(sn.find("DMtt") == 0) {std::cout<<"++++++++ ss: "<<ss<<std::endl; ss.replace(ss.find("DMtt"), 4, sn); std::cout<<"++++++++++ ss after replace: "<<ss<<std::endl;}
	      }
	    }
	    if(!skip) {
	      //string ss = systn;
	      //	      ss.replace(ss.find(catn), catn_short.size(), catn);
	      hist_syst_up[i][s][crs] = new RooDataHist((name + crN + "_" + ss + "Up" /*_hist"*/).c_str(), longName[i].c_str(), met, h_var_syst_up[i][s][crs]);
	      hist_syst_down[i][s][crs] = new RooDataHist((name + crN + "_" + ss + "Down" /*_hist"*/).c_str(), longName[i].c_str(), met, h_var_syst_down[i][s][crs]);
	      w->import(*hist_syst_up[i][s][crs]);
	      w->import(*hist_syst_down[i][s][crs]);
	    }
	    //	    pdf_syst_up[i][s][crs] = new RooHistPdf((name + crN + "_" + systName[s] + "Up").c_str(), longName[i].c_str(), met, *(hist_syst_up[i][s][crs]), 0);
	    //	    pdf_syst_down[i][s][crs] = new RooHistPdf((name + crN + "_" + systName[s] + "Down").c_str(), longName[i].c_str(), met, *(hist_syst_down[i][s][crs]), 0);
	    //	    w->import(*pdf_syst_up[i][s][crs], RooFit::RecycleConflictNodes());
	    //	    w->import(*pdf_syst_down[i][s][crs], RooFit::RecycleConflictNodes());
	  }
	}
      }
    }
  }
  if(!system((string("mkdir -p ")+outDir+"/ws").c_str())) { cerr <<"info: directory ws already exists" << endl; }
  string wfname = string(outDir+"/ws/workspace");
  if(ctrlRegions) wfname += "_ctrl";
  wfname += ".root";
  cout << ">>> creating workspace file " << wfname << endl;
  TFile fWorkspace(wfname.c_str(),"RECREATE");
  fWorkspace.cd();
  cout << ">>> writing to workspace" << endl;
  w->Write();
  cout << ">>> closing workspace file" << endl;
  fWorkspace.Close();

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
