#include "ttDM/stat/interface/WorkspaceSystFiller.h"
#include "ttDM/stat/interface/Config.h"
#include <iostream>
using namespace std;

WorkspaceSystFiller::~WorkspaceSystFiller() { }

void WorkspaceSystFiller::prepare(unsigned int syst) {
}

void WorkspaceSystFiller::fill() {
  fillNominal();
  for(unsigned int syst = 0; syst < nSyst_; ++syst) {
    if(!skip(syst)) {
      if(verbose_ > 1) {
	cout << "*** filling for syst. " << Config::get().systematic(syst) << endl;
	cout << "*** prepare to fill" << endl;
      }
      prepare(syst);
      if(verbose_ > 1)
	cout << "*** filling Up variation for syst. " << Config::get().systematic(syst) << endl;
      fillUp(syst);
      if(verbose_ > 1)
	cout << "*** filling Down variation for syst. " << Config::get().systematic(syst) << endl;
      fillDown(syst);
    } else {
      if(verbose_ > 1) 
	cout << "*** skipping syst. " << Config::get().systematic(syst) << endl;
    }
  }
}
