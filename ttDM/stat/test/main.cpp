#include "ttDM/stat/test/check.h"
#include <iostream>
#include <vector>
#include <string>
using namespace std;

bool testConfig();
bool testDatacard();
bool testHistoSet();
bool testHistoSetSyst();
bool testHistoCollection();
bool testDataMCHistoCollection();
bool testSkipSystematic();
bool testWorkspaceSystFiller();
bool testWorkspaceDataFiller();
bool testWorkspaceTFFiller();
bool testWorkspaceSystSigFiller();
bool testWorkspaceSystBkgFiller();
bool testWorkspaceCategory();
bool testWorkspaceTransferFactors();
bool testWorkspace();

int main() {
  bool pass = true;
  pass &= check(testConfig(), "=== Config class test ===");
  pass &= check(testHistoSet(), "=== HistoSet class test ===");
  pass &= check(testHistoSetSyst(), "=== HistoSetSyst class test ===");
  pass &= check(testHistoCollection(), "=== HistoCollection class test ===");
  pass &= check(testDataMCHistoCollection(), "=== DataMCHistoCollection class test ===");
  pass &= check(testDatacard(), "=== Datacard class test ===");
  pass &= check(testSkipSystematic(), "=== skipSystematic function ===");
  pass &= check(testWorkspaceSystFiller(), "=== WorkspaceSystFiller class ===");
  pass &= check(testWorkspaceDataFiller(), "=== WorkspaceDataFiller class ===");
  pass &= check(testWorkspaceTFFiller(), "=== WorkspaceTFFiller class ===");
  pass &= check(testWorkspaceSystSigFiller(), "=== WorkspaceSystSigFiller class ===");
  pass &= check(testWorkspaceSystBkgFiller(), "=== WorkspaceSystBkgFiller class ===");
  pass &= check(testWorkspaceTransferFactors(), "=== WorkspaceTransferFactors class ===");
  pass &= check(testWorkspaceCategory(), "=== WorkspaceCategory class ===");
  pass &= check(testWorkspace(), "=== Workspace class ===");

  if(pass) {
    cout << "ttDM: test passed!" << endl;
    return 0;
  } else {
    cerr << "ttDM: test *** FAILED! ***" << endl;
      return 1;
  }
}
