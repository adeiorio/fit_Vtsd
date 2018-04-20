#include "ttDM/stat/interface/WorkspaceSystFiller.h"
#include "ttDM/stat/test/check.h"
#include <string>
#include <iostream>
using namespace std;

namespace test {
  class TestWorkspaceSystFiller : public WorkspaceSystFiller {
  public:
    TestWorkspaceSystFiller(unsigned int nSyst) : WorkspaceSystFiller(nSyst) { }
    virtual ~TestWorkspaceSystFiller() { }
    const string & test() const { return test_; }
  private:
    string test_;
    virtual void fillNominal() { test_ += "a"; }
    virtual void prepare(unsigned int syst) { test_ += "x"; }
    virtual bool skip(unsigned int syst) { return false; }
    virtual void fillUp(unsigned int syst) { test_ += "b"; }
    virtual void fillDown(unsigned int syst) { test_ += "c"; }
  };
}


bool testWorkspaceSystFiller() {
  bool pass = true;
  test::TestWorkspaceSystFiller f(3);
  f.fill();
  check(f.test() == "axbcxbcxbc", "working template method");
  return pass;
}
