#ifndef STAT_DATACARD_H
#define STAT_DATACARD_H
#include <iostream>
#include <climits>
#include <memory>
#include <vector>
#include <string>

class DataMCHistoCollection;

class Datacard {
public:
  explicit Datacard(const std::vector<std::shared_ptr<DataMCHistoCollection> > & datamc, 
		    unsigned int catMin = UINT_MAX);
  void header(std::ostream &) const;
  void observation(std::ostream&, bool cuts, bool dummy, unsigned int ctrlRegion = UINT_MAX) const;
  void shapes(std::ostream&, bool ctrl) const;
  void systematics(std::ostream& datacard, bool shape, int signalSample) const;
  void rate(std::ostream & out, bool noCut, unsigned int signalSample, bool dummy, 
	    unsigned int ctrlRegion = UINT_MAX) const;
  void save(const std::string & title, unsigned int signalSample, bool noCut, 
	    unsigned int catMin = UINT_MAX);
  void save();
  void verbose(int i) { verbose_ = i; }

private:
  int verbose_;
  std::vector<std::shared_ptr<DataMCHistoCollection> > datamc_;
  unsigned int catMin_, catMax_;
  void fixSystName(std::string &, unsigned int) const;
 };

#endif
