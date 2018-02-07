void plotMEThist_wrap(bool syst = true, int cat = -1, bool ctrl = true) {
  gSystem->Load("libHiggsAnalysisCombinedLimit.so");
  gSystem->AddIncludePath(" -I/afs/cern.ch/cms/slc6_amd64_gcc481/lcg/roofit/5.34.18-cms12/include -I../../");
  gROOT->ProcessLine(".L plotMEThist.C+");
  plotMEThist(syst, cat, ctrl);
}
