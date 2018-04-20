#ifndef STAT_CONFIG_H
#define STAT_CONFIG_H
#include <boost/program_options.hpp>
#include <stdexcept>
#include <string>
#include <climits>

class Config {
public:
  Config(int argc, char *argv[]);
  ~Config();
  const std::string & outputDirectory() const { return outputDirectory_; }
  const std::string & histoDirectory() const { return histoDirectory_; }
  // const std::string & channel() const { return channel_; }
  const std::vector<std::string> & categories() const { return categories_; }
  unsigned int numCategories() const { return categories_.size();  }
  const std::string & category(unsigned int i) const { return categories_[i]; }
  const std::vector<std::string> & categoryFileNames() const { return categoryFileNames_; }
  const std::string & categoryFileName(unsigned int i) const { return categoryFileNames_[i]; }
  const std::vector<std::string> & variableNames() const { return variableNames_; }
  const std::string & variableName(unsigned int i) const { return variableNames_[i]; }
  const std::vector<std::string> & systematics() const { return systematics_; }
  const std::vector<std::string> & normSystematics() const { return normSystematics_; }  
  const std::string & systematic(unsigned int i) const { return systematics_[i]; }
  const std::string & normSystematic(unsigned int i) const { return normSystematics_[i]; }
  const std::string & rateCategory(unsigned int i) const { return rateCategories_[i]; }
  const std::string & rateSystematic(unsigned int i) const { return rateSystematics_[i]; }
  const std::map< std::string, std::vector<std::pair<std::string, std::string> > > & rateParams() const  { return rateParams_; }
  const std::vector<std::pair<std::string, std::string> > & rateParam(std::string name) const { return rateParams_.at(name); }
  std::vector<std::string> & excludeFromCategory(std::string cat) { return excludeFromCategory_[cat]; }
  // Check whether one should include numNormSystematics
  unsigned int numSystematics() const { return systematics_.size(); }
  unsigned int numNormSystematics() const { return normSystematics_.size(); }
  unsigned int numRateSystematics() const { return rateSystematics_.size(); }
  unsigned int numRateParams() const { return rateParams_.size(); } 
  unsigned int numRateParamsRegions(std::string name) const { return rateParams_.at(name).size(); }
  bool hasSystematics() const { return numSystematics() > 0; }
  bool hasNormSystematics() const { return numNormSystematics() > 0; }
  bool hasRateSystematics() const { return numRateSystematics() > 0; }
  bool hasRateParams() const { return numRateParams() > 0; }
  const std::vector<int> & binCuts() const { return binCuts_; }
  int binCut(unsigned int i) const { return binCuts_[i]; }
  struct Sample {
  public:
    Sample(const std::string & s, const std::string & l) : 
      name(s), description(l) { }
    std::string name, description;
  };
  const std::vector<Sample> & backgroundSamples() const { return backgroundSamples_; }
  const Sample & backgroundSample(unsigned int i) const { return backgroundSamples_[i]; }
  unsigned int numBackgroundSamples() const { return backgroundSamples_.size(); }
  unsigned int findBackgroundSample(const std::string & name) const;
  bool excludedFromCategory(const std::string & cat,const std::string & sample) const;


  const std::vector<Sample> & signalSamples() const { return signalSamples_; }
  const Sample & signalSample(unsigned int i) const { return signalSamples_[i]; }
  unsigned int numSignalSamples() const { return signalSamples_.size(); }
  const std::vector<Sample> & signalPoints() const { return signalPoints_; }
  const Sample & signalPoint(unsigned int i) const { return signalPoints_[i]; }
  unsigned int numSignalPoints() const { return signalPoints_.size(); }
  const std::vector<Sample> & signalProcesses() const { return signalProcesses_; }
  const Sample & signalProcess(unsigned int i) const { return signalProcesses_[i]; }
  unsigned int numSignalProcesses() const { return signalProcesses_.size(); }
  unsigned int signalSampleIndex(unsigned int proc, unsigned int point) const { return point + proc * numSignalPoints(); }
  struct ControlRegion {
  public:
    ControlRegion(const std::string & n, const std::string & s, const std::string & c) : 
      name(n), sample(s), category(c) { }
    std::string name;
    std::string sample;
    std::string category;
  };
  unsigned int numControlRegions(unsigned int cat) const { 
    if(cat >= controlRegions_.size()) return 0;
    return controlRegions_[cat].size(); 
  }
  bool hasControlRegions() const { return controlRegions_.size() > 0; }
  const std::vector<ControlRegion> & controlRegions(unsigned int cat) const { 
    return controlRegions_[cat];
  }
  const ControlRegion & controlRegion(unsigned int cat, unsigned int i) const { 
    return controlRegions_[cat][i];
  }
  unsigned int findControlRegion(unsigned int cat, const std::string sample) const;
  enum { npos = UINT_MAX };
  double lumiUncertainty() const { return lumiUncertainty_; }
  const std::string & rooRealVar() const { return rooRealVar_; }
  int verbose() const { return verbose_; }
  bool addPdf() const {return addPdf_;}
  bool addDataHist() const {return addDataHist_;}

  static const Config & get() { return * instance_; }
  
private:
  Config() { }
  int verbose_;
  bool addPdf_,addDataHist_;
  double lumiUncertainty_;
  std::string rooRealVar_;
  //std::string channel_;
  std::string outputDirectory_;
  std::string histoDirectory_;
  std::vector<std::string> categories_;
  std::vector<std::string> categoryFileNames_;
  std::vector<std::string> variableNames_;
  std::vector<std::string> systematics_;
  std::vector<std::string> normSystematics_;
  std::vector<std::string> rateSystematics_;
  std::vector<std::string> rateCategories_;
  std::map< std::string, std::vector<std::string> > excludeFromCategory_;
  std::map< std::string, std::vector<std::pair<std::string, std::string> > > rateParams_;
  std::vector< std::string > excludedCat_;
  std::vector<int> binCuts_;
  std::vector<Sample> signalProcesses_, signalPoints_;
  std::vector<Sample> backgroundSamples_, signalSamples_;
  std::vector<std::vector<ControlRegion> > controlRegions_;
  void hints(const boost::program_options::options_description &) const;

  static Config * instance_;
};

#endif
