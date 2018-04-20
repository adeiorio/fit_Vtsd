import os
import subprocess
import optparse

usage = 'usage: %prog [--cat N]'
parser = optparse.OptionParser(usage)
parser.add_option('-c', '--cat', dest='cat', type='int', default = -1, help='Set the flag to -1 to avoid the combination of semileptonic and fullhadronic channel')
parser.add_option('-S', '--syst', dest='syst', type='string', default = '1', help='Set the flag to 0 to remove systematics')
parser.add_option('-s', '--sig', dest='sig', type='int', default = 0, help='Set the flag to 1 to enable significance computation')
(opt, args) = parser.parse_args()

def runCombine(cmdStr, logFile):
    "run combine for a specific case"
    cmd = (cmdStr % (mass, mass))
    print cmd
    writer =open(logFile, 'w') 
    process = subprocess.call(cmd, shell = True, stdout=writer)
    return

for mass in [1, 10, 50, 200, 600, 1000]:
    print "evaluate limit for m = ", mass, " GeV"
    os.system("mkdir -p ttDM%i" % mass)
    os.chdir("ttDM%i" % mass)
    categories = ["", "semileptonic", "fullhadronic"]
    if(opt.cat == 0): categories = ["semileptonic"]
    if(opt.cat == 1): categories = ["fullhadronic"]
    for cat in categories:
        for method in ["count", "shape"]:
            post = "_" + cat + ("" if cat == "" else "_") + method
            runCombine("combine -M Asymptotic -S " + opt.syst + " -t -1 -n ttDM" + post + " -m %i ttDM%i" + post + ".txt",
                       "asymptotic" + post + ".log")
            if(opt.sig > 0): 
                runCombine("combine -M ProfileLikelihood -S " + opt.syst + " -n ttDM" + post + " -m %i --signif --pvalue -t 1000 --toysFreq --expectSignal=1 ttDM%i" + post + ".txt",
                           "profileLikelihood" + post + ".log")
            
    os.chdir("..");
