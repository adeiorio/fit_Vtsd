import os
import subprocess
import optparse

usage = 'usage: %prog [--cat N]'
parser = optparse.OptionParser(usage)
parser.add_option('', '--mchi', dest='mchi', type='int', default = 1, help='DM mass')
parser.add_option('', '--mphi', dest='mphi', type='int', default = 1, help='Mediator mass')
parser.add_option('-c', '--cat', dest='cat', type='int', default = -1, help='Set the flag to -1 to avoid the combination of semileptonic and fullhadronic channel')
parser.add_option('-S', '--syst', dest='syst', type='string', default = '1', help='Set the flag to 0 to remove systematics')
parser.add_option('-M', '--method', dest='method', type='string', default = 'shape', help='Run a single method (ctrl, shape, count)')
parser.add_option('-s', '--sig', dest='sig', type='int', default = 0, help='Set the flag to 1 to enable significance computation')
parser.add_option('-i', '--interaction', dest='interaction', type='string', default='sc', help='Interaction model (sc, ps)')
parser.add_option('-D', '--dir', dest='dir', type='string', default = '.', help='datacards direcotry')
(opt, args) = parser.parse_args()
interaction = opt.interaction

def runCombine(cmdStr, logFile):
    "run combine for a specific case"
    
    cmd = (cmdStr % mChi)
    print cmd
    writer = open(logFile, 'w') 
    process = subprocess.call(cmd, shell = True, stdout=writer)
    return

#mass grid points
# mPhi:mChi

masses = { opt.mphi : [opt.mchi]}

print masses

#masses = { 10: [1, 10, 50, 150, 500, 1000], 
#           20: [1],
#           15: [10],
#           50: [1, 10, 50],
#           95: [50],
#           100: [1, 10],
#           200: [1, 50, 150],
#           295: [150],
#           300: [1, 50],
#           500: [1, 150, 500],
#           995: [500],
#           1000: [1, 150, 1000], 
#           1995: [1000],
#           2000: [1, 500],
#           10000: [1, 10, 50, 150, 500, 1000]
#           }


os.chdir(opt.dir)
for mPhi in masses.keys():
    for mChi in masses[mPhi]:
        print "evaluate limit for mPhi = ", mPhi, " GeV, ", mChi, " GeV" 
        path = ("DMtt_%s_Mchi%iMphi%i" % (interaction, mChi, mPhi) )
        print path
        if(os.path.exists(path)):
            os.chdir(path)
            categories = [""]#, "semileptonic", "fullhadronic"]
            if(opt.cat == 0): categories = ["semileptonic"]
            if(opt.cat == 1): categories = ["fullhadronic"]
            methods = ["shape", "count"] # ctrl not in by default. Should specify explicitly.
            if(opt.method != ""): 
                methods = [opt.method]
                for cat in categories:
                    print "category: " + ("all" if cat=="" else cat)
                    for method in methods:
                        if(method == "ctrl"):
                            cmd = "combineCards.py *_ctrl_*.txt > DMtt_%s_Mchi%iMphi%i_ctrl.txt" % (interaction, mChi, mPhi)
                            print cmd
                            os.system(cmd)
                        post = ("%s_Mchi%iMphi%i_" % (interaction, mChi, mPhi)) + cat + ("" if cat == "" else "_") + method
                        runCombine("combine -M Asymptotic -S " + opt.syst + " -n DMtt_" + post + " -m %i DMtt_" + post + ".txt",
                                   "asymptotic" + post + ".log")
#                        runCombine("combine -M Asymptotic -S " + opt.syst + " -t -1 -n DMtt_" + post + " -m %i DMtt_" + post + ".txt",
#                                   "asymptotic" + post + ".log")
                        if(opt.sig > 0): 
                            runCombine("combine -M ProfileLikelihood -S " + opt.syst + " -n DMtt_" + post + 
                                       " -m %i --signif --pvalue -t 1000 --toysFreq --expectSignal=1 DMtt_" + post + ".txt",
                                       "profileLikelihood" + post + ".log")
                    
            os.chdir("..");
