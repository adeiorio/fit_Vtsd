#include <vector>
#include <string>
using namespace std;

extern unsigned int nBkg;
extern unsigned int nSig;
extern unsigned int nSamples;
extern unsigned int nCat;
extern vector<string> catName, fileCatName, varName;
extern vector<int> metBinCut;
extern string histoDir, outDir;

extern vector<string> longName, shortName;
extern vector<unsigned int> nCr;
extern vector<vector<string> > crNames, crSamples;

extern unsigned int nSyst;
extern vector<string> systName;

extern vector<double> yieldCutObs, yieldNoCutObs;
extern vector<vector<double> > yieldCut, yieldNoCut;
extern vector<vector<vector<double> > > yieldCut_up, yieldNoCut_up;
extern vector<vector<vector<double> > > yieldCut_down, yieldNoCut_down;
extern vector<vector<vector<double> > > systCount;

extern bool syst;
extern int cat;
extern bool ctrlRegions;

void datacardHeader(bool syst, ofstream& datacard, unsigned int iMin = 0, unsigned int iMax = nCat);
void datacardObs(ofstream& datacard, bool cuts, unsigned int iMin = 0, unsigned int iMax = nCat, 
		 bool dummy = false, unsigned int cr = 1000);
void datacardSyst(bool syst, bool shape, ofstream& datacard, int k, unsigned int iMin, unsigned int iMax, 
		  const vector<string> & samples);
void datacardShapes(ofstream& datacard, bool ctrl);
void datacardRate(ofstream & datacard, const vector<vector<double> > yield, 
		  unsigned int k, unsigned int iMin = 0, unsigned int iMax = nCat, 
		  bool dummy = false, unsigned int cr = 1000);
void datacards(bool syst, const char * title, unsigned int k, const vector<vector<double> > & yield, 
	       unsigned int iMin = 0, unsigned int iMax = nCat);

