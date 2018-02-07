#include <string>
#include <vector>
using namespace std;

unsigned int nBkg;
unsigned int nSig;
unsigned int nSamples;
unsigned int nCat;

vector<string> catName, fileCatName, varName;
vector<int> metBinCut;
string histoDir, outDir;

vector<string> longName, shortName;
vector<unsigned int> nCr;
vector<vector<string> > crNames, crSamples;

unsigned int nSyst;
vector<string> systName;
vector<double> yieldCutObs, yieldNoCutObs;
vector<vector<double> > yieldCut, yieldNoCut;
vector<vector<vector<double> > > yieldCut_up, yieldNoCut_up;
vector<vector<vector<double> > > yieldCut_down, yieldNoCut_down;
vector<vector<vector<double> > > systCount;

bool syst;
int cat;
bool ctrlRegions;

