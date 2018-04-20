#ifndef STAT_WORKSPACESYSTFILLER_H
#define STAT_WORKSPACESYSTFILLER_H

class WorkspaceSystFiller {
 public:
 WorkspaceSystFiller(unsigned int nSyst) : nSyst_(nSyst), verbose_(0) { }
  virtual ~WorkspaceSystFiller();
  virtual void fill();
  void setVerbose(int v) { verbose_ = v; }

 protected: 
  unsigned int nSyst_;
  int verbose_;

 private:
  virtual void fillNominal() = 0;
  virtual void prepare(unsigned int syst);
  virtual bool skip(unsigned int syst) = 0;
  virtual void fillUp(unsigned int syst) = 0;
  virtual void fillDown(unsigned int syst)= 0;
};

#endif
