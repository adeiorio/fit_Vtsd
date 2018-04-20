#include "ttDM/stat/interface/Config.h"
#include "ttDM/stat/interface/stringUtils.h"
#include "ttDM/stat/test/makeConfig.h"
#include "ttDM/stat/test/check.h"
#include <iostream>
#include <vector>
#include <string>
#include <memory>
using namespace std;

bool testConfig() {
  bool pass = true;
  bool exception = false;
  {
    try { makeWrongConfig(); }
    catch ( runtime_error& er ) { exception = true; }
    pass &= check(exception, "throw on wrong datacard filename");
  }
  {
    shared_ptr<Config> cfg(makeConfig());
    pass &= check(cfg->lumiUncertainty() == 0.048, "correct lumi uncertainty");
    pass &= check(cfg->rooRealVar() == "met", "correct lumi RooRealVar");
    pass &= check(cfg->histoDirectory() == "histos", "correct histo directory");
    
    vector<string> testCategories = { "semileptonic", "fullhadronic" };
    const vector<string> & categories = cfg->categories();
    pass &= check(testCategories == categories, "categories match datacard");
    
    const vector<string> & categoryFileNames = cfg->categoryFileNames();
    pass &= check(testCategories == categoryFileNames, "category file names match datacard");
    
    const vector<Config::Sample> & bkg = cfg->backgroundSamples();
    pass &= check(bkg.size() == 6, "correct number of background samples");
    
    pass &= check(bkg[0].name == "TT" && bkg[0].description == "ttbar", "correct bkg sample name");
    pass &= check(bkg[1].name == "SingleTop" && bkg[1].description == "single top", "correct bkg sample name");
    pass &= check(bkg[3].name == "ZToNuNu" && bkg[3].description == "Z -> nu nu",  "correct bkg sample name");
    
    const vector<Config::Sample> & sig = cfg->signalSamples();
    pass &= check(sig.size() == 2, "correct number of signal samples");
    pass &= check(sig[0].name == "DMtt_sc_Mchi1Mphi10" && sig[0].description == "t+DM, mchi = 1 GeV, mphi = 10 GeV",
		  "correct signal sample name");
    
    const vector<string> & sys = cfg->systematics();
    pass &= check(sys.size() == 4, "correct number of systematics");
    pass &= check(sys[0] == "btag" && sys[1] == "jes" && sys[2] == "mistag", "correct systematics names");
    
    const vector<string> & vars = cfg->variableNames();
    pass &= check(vars.size() == 2, "correct number of variable names");
    pass &= check(vars[0] == "metFinal", "correct variable name");
    pass &= check(vars[1] == "metFinal", "correct variable name");
    
    const vector<int> & cuts = cfg->binCuts();
    pass &= check(cuts.size() == 2, "correct number of bin cuts");
    pass &= check(cuts[0] == 5 && cuts[1] == 6, "correct cut values");
    
    pass &= check(cfg->hasControlRegions(), "has control regions");
    const vector<Config::ControlRegion> & ctrl1 = cfg->controlRegions(1);
    pass &= check(ctrl1.size() == 4, "correct number of control regions");
    pass &= check(ctrl1[0].name == "SR_1lep_mt" && ctrl1[0].sample == "TT" && ctrl1[0].category == "fullhadronic",
		  "correct control region name");
    pass &= check(ctrl1[1].name == "outtop" && ctrl1[1].sample == "QCD" && ctrl1[1].category == "fullhadronic",
		  "correct control region name");
    
    const vector<Config::ControlRegion> & ctrl2 = cfg->controlRegions(0);
    pass &= check(ctrl2.size() == 2, "correct number of control regions");
    pass &= check(ctrl2[0].name == "2lep" && ctrl2[0].sample == "TT" && ctrl2[0].category == "semileptonic",
		  "correct control region name");
  }
  {
    shared_ptr<Config> cfg(makeConfigMultiSig());
    pass &= check(cfg->numSignalPoints() == 2, "correct number of signal points");
    pass &= check(cfg->numSignalSamples() == 2, "correct number of signal samples (multiple)");
    pass &= check(cfg->numSignalProcesses() == 1, "correct number of signal processes");
  }

  return pass;
}
