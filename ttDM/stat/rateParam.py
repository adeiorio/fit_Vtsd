#Reads a rootfile and prints out a scale factor, the upgrade would be to print also the nuisance parameter. For now it does it with Combine only
from array import array
#import ROOT
import time
import sys
#sys.argv.append('-b')
import ROOT
import commands, os
import numpy
import optparse

usage = 'python makeplot.py'
parser = optparse.OptionParser(usage)
parser.add_option('-L','--lep', dest='lep', type='string', default = 'muon', help='Default make muon')
(opt, args) = parser.parse_args()

def readSF(rootfile,  process, region):#, channel, sys='', histodir="./histos/", format="combine"):
    #           fileFit = ROOT.TFile.Open(rootfile)
    fileFit = ROOT.TFile.Open(str(rootfile), "READ")
    option = ""
    err = array('d', [1.0])
    if region == "no_region":
        #        print "returning 1.0 for region not in the fit."                                                                                                    
        return 1.0
    hdirprefit= 'shapes_prefit'
                #               print hdirprefit+'/'+channel+"_"+region+'/'+process
    hprefit = fileFit.Get(hdirprefit+'/'+region+'/'+process).Clone("prefit_"+region+"_"+process)
    hdirpostfit= 'shapes_fit_s'
    hpostfit = fileFit.Get(hdirpostfit+'/'+region+'/'+process).Clone("postfit_"+region+"_"+process)
    neventspre = hprefit.IntegralAndError(1, hprefit.GetNbinsX(), err,option)
    neventspost = hpostfit.IntegralAndError(1, hpostfit.GetNbinsX(), err,option)
    sf = neventspost/neventspre
    return sf

process = ["TT", "DDQCD", "ST_tch", "WJets","VV","DYJets","ST_tW","ST_sch"]# "ST_tch_sd",
regions = {
"muon":["muon_2j1t_central","muon_2j1t_forward","muon_3j1t_central","muon_3j1t_forward","muon_3j2t"],
"electron":["electron_2j1t_central","electron_2j1t_forward","electron_3j1t_central","electron_3j1t_forward","electron_3j2t"],
"muele":["muon_2j1t_central","muon_2j1t_forward","muon_3j1t_central","muon_3j1t_forward","muon_3j2t","electron_2j1t_central","electron_2j1t_forward","electron_3j1t_central","electron_3j1t_forward","electron_3j2t"]
}

lepton = opt.lep
rootfile=lepton+"/fitDiagnostics.root"

for p in process:
    print "***************************************************"
    print "*******              %-9s             ********" %(p)
    print "***************************************************"
    for region in regions[lepton]:
        print "scale factor for %-21s  is: %7.3f" % (region, readSF(rootfile,p,region))

fileFit = ROOT.TFile.Open(str(rootfile), "READ")
print (fileFit.Get("fit_b")).Print()
print (fileFit.Get("fit_s")).Print()
