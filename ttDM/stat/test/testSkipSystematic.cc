#include "ttDM/stat/interface/skipSystematic.h"
#include "ttDM/stat/test/makeConfig.h"
#include "ttDM/stat/test/check.h"
#include <memory>
using namespace std;

bool testSkipSystematic() {
  bool pass = true;
  shared_ptr<Config> cfg(makeConfig());
  pass &= check(skipSystematic("semileptonic", "mcstat_semileptonic_TT_bin5", ""), 
		"skip mcstat_semileptonic_TT_bin5 for semileptonic in signal (empty)");
  pass &= check(skipSystematic("semileptonic", "mcstat_semileptonic_TT_bin5", "DMtt_sc_Mchi10Mphi10"), 
		"skip mcstat_semileptonic_TT_bin5 for semileptonic in signal");
  pass &= check(!skipSystematic("semileptonic", "mcstat_semileptonic_TT_bin5", "TT"), 
		"don't skip mcstat_semileptonic_TT_bin5 for semileptonic in TT");  
  pass &= check(skipSystematic("semileptonic", "mcstat_semileptonic_TT_bin5", "DMtt_sc_Mchi10Mphi10"), 
		"skip mcstat_semileptonic_TT_bin5 for semileptonic in signal");
  pass &= check(!skipSystematic("semileptonic", "mcstat_semileptonic_DMtt_bin5", ""), 
		"don't skip mcstat_semileptonic_DMtt_bin5 for semileptonic in signal (empty)");
  pass &= check(!skipSystematic("semileptonic", "mcstat_semileptonic_DMtt_bin5", "DMtt_sc_Mchi10Mphi10"), 
		"don't skip mcstat_semileptonic_DMtt_bin5 for semileptonic in signal");
  pass &= check(skipSystematic("fullhadronic", "mcstat_semileptonic_DMtt_bin5", ""), 
		"skip mcstat_semileptonic_DMtt_bin5 for fullhadronic in signal(empty)");
  pass &= check(skipSystematic("fullhadronic", "mcstat_semileptonic_TT_bin5", "TT"), 
		"skip mcstat_semileptonic_TT_bin5 for fullhadronic in TT");
  return pass;
}
