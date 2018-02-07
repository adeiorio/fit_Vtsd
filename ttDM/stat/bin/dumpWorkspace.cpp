#include "TSystem.h"
#include "TFile.h"
#include "RooWorkspace.h"
#include <boost/program_options.hpp>
#include <iostream>
using namespace std;

int main(int argc, char *argv[]) {
  using namespace boost;
  namespace po = boost::program_options;
  string fileName;
  po::options_description desc("Main options");
  desc.add_options()
    ("file,f", po::value<string>(&fileName), "root file with workspace")
    ("help,h", "Produce help message")
    ;
  po::positional_options_description p;
  p.add("file", -1);

  po::variables_map vm;
  try{
    po::store(po::command_line_parser(argc, argv).options(desc).positional(p).allow_unregistered().run(), vm);
    po::notify(vm);
  } catch(std::exception &ex) {
    cerr << "Invalid options: " << ex.what() << endl;
    cout << "Use combine --help to get a list of all the allowed options"  << endl;
    return 999;
  } catch(...) {
    cerr << "Unidentified error parsing options." << endl;
    return 1000;
  }
  // if help, print help
  if(vm.count("help")) {
    cout << "Usage: " << argv[0] <<" [options]\n";
    cout << desc;
    exit(0);
  }
  if(fileName == "") {
    cerr << "Missing fileName" << endl;
    return 1002;
  }

  gSystem->Load("libHiggsAnalysisCombinedLimit.so");
  TFile f(fileName.c_str());
  RooWorkspace * w = dynamic_cast<RooWorkspace*>(f.Get("w"));
  w->Print();
  return 0;
}
