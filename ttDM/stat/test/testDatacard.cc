#include "ttDM/stat/interface/Datacard.h"
#include "ttDM/stat/interface/DataMCHistoCollection.h"
#include "ttDM/stat/interface/Config.h"
#include "ttDM/stat/interface/stringUtils.h"
#include "ttDM/stat/test/check.h"
#include "ttDM/stat/test/makeConfig.h"
#include <iostream>
#include <sstream>

using namespace std;

bool testDatacard() {
  bool pass = true;
  {
    shared_ptr<Config> cfg = makeConfigMCStat();
    vector<shared_ptr<DataMCHistoCollection> > hc;
    hc.push_back(make_shared<DataMCHistoCollection>(0));
    hc.push_back(make_shared<DataMCHistoCollection>(1));
    hc[0]->fill();
    hc[1]->fill();
    
    cout << "+++ pass: initialized configuration..." << endl;
    string line;
    ostringstream header;
    Datacard * data = new Datacard(hc);
    cout << "+++ pass: Datacard constructor..." << endl;
    data->header(header);
    //  cout << header.str() << endl;
    istringstream iheader(header.str());
    getline(iheader, line);
    cout << line << endl;
    pass &= check(beginsWith(line, "imax 2 number of channels ("),
		  "correct header for channels");
    getline(iheader, line);
    cout << line << endl;
    pass &= check(beginsWith(line,"jmax 1 number of backgrounds ("),
		  "correct header for backgrounds");
    getline(iheader, line);
    cout << line << endl;
    pass &= check(beginsWith(line, "kmax 2 number of nuisance parameters ("),
		  "correct header for systamtics");
    
    ostringstream obs;
    data->observation(obs, false, false);
    //  cout << obs.str() << endl;
    istringstream iobs(obs.str());
    getline(iobs, line);
    cout << line << endl;
    pass &= check(beginsWith(line, "bin\tsemileptonic_CR_TT"), "correct bin line");
    getline(iobs, line);
    cout << line << endl;
    pass &= check(beginsWith(line, "observation\t32\t214"), "correct obs line");
    
    Datacard * data2 = new Datacard(hc, 0);
    ostringstream obsCtrl;
    data2->observation(obsCtrl, true, false, 0);
    //  cout << obsCtrl.str() << endl;
    istringstream iobsCtrl(obsCtrl.str());
    getline(iobsCtrl, line);
    cout << line << endl;
    pass &= check(beginsWith(line, "bin\tsemileptonic_CR_TT_2lep"), "correct bin line");
    getline(iobsCtrl, line);
    cout << line << endl;
    getline(iobsCtrl, line);
    cout << line << endl;
    pass &= check(beginsWith(line, "observation\t9"), "correct obs line");
    
    ostringstream syst;
    data->systematics(syst, true, 0);
    istringstream ioSyst(syst.str());
    getline(ioSyst, line);
    pass &= check(beginsWith(line, "lumi\tlnN\t1.048"), "correct lumi syst");
    getline(ioSyst, line);
    cout << line << endl;
    pass &= check(beginsWith(line, "mcstat_semileptonic_CR_TT_TT_bin1\tshape\t-"), "correct shape syst");
    
    ostringstream rate;
    data->rate(rate, true, 0, false);
    //  cout << rate.str() << endl;
    istringstream ioRate(rate.str());
    getline(ioRate, line);
    cout << line << endl;
    pass &= check(beginsWith(line, "bin\tsemileptonic_CR_TT\tsemileptonic_CR_TT"), "correct bin line");
    getline(ioRate, line);
    cout << line << endl;
    pass &= check(beginsWith(line, "process\tDMtt_sc_Mchi1Mphi10\tTT"), "correct process line");
    getline(ioRate, line);
    cout << line << endl;
    pass &= check(beginsWith(line, "process\t0\t1"), "correct process line");
    getline(ioRate, line);
    cout << line << endl;
    pass &= check(beginsWith(line, "rate\t5.879\t13.85"), "correct rate line");
    
    bool exception = false;
    try {
      data->save();
    } catch(runtime_error & er) {
      exception = true;
    }
    pass &= check(!exception, "save datacard");
  }
  {
    shared_ptr<Config> cfg = makeConfigMultiSig();
    vector<shared_ptr<DataMCHistoCollection> > hc;
    hc.push_back(make_shared<DataMCHistoCollection>(0));
    hc.push_back(make_shared<DataMCHistoCollection>(1));
    hc[0]->fill();
    hc[1]->fill();
    cout << "+++ pass: initialized configuration..." << endl;
    string line;
    ostringstream header;
    Datacard * data = new Datacard(hc);
    cout << "+++ pass: Datacard constructor..." << endl;
    data->header(header);

    ostringstream rate;
    data->rate(rate, true, 0, false);
    //  cout << rate.str() << endl;
    istringstream ioRate(rate.str());
    getline(ioRate, line);
    cout << line << endl;
    pass &= check(beginsWith(line, "bin\tsemileptonic\tsemileptonic"), "correct bin line");
    getline(ioRate, line);
    cout << line << endl;
    pass &= check(beginsWith(line, "process\tDMtt_sc_Mchi1Mphi10\tTT"), "correct process line");
    getline(ioRate, line);
    cout << line << endl;
    pass &= check(beginsWith(line, "process\t0\t1"), "correct process line");
    getline(ioRate, line);
    cout << line << endl;
    pass &= check(beginsWith(line, "rate\t5.879\t13.85"), "correct rate line");
  }
  return pass;
}
