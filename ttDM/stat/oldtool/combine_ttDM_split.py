import os
import subprocess
import optparse

usage = 'usage: %prog -m massPoint -a analysisType -m method'
parser = optparse.OptionParser(usage)
parser.add_option('-m', '--mass', dest='mass', type='int', help='Mass hypothesis.')
parser.add_option('-a', '--analysis', dest='analysis', type='string', default = 'count', help='Analysis type. Options are: count or shape.')
parser.add_option('-c', '--cat', dest='cat', type='string', default = '', help='Indicate for which category run the limis has be computed. Options are: semileptonic, fullhadronic, or empty string, which corresponds to the combination of the categories.')

(opt, args) = parser.parse_args()

def runCombine(cmdStr, logFile):
    "run combine for a specific case"
    cmd = (cmdStr % (opt.mass, opt.mass))
    print cmd
    writer =open(logFile, 'w') 
    process = subprocess.call(cmd, shell = True, stdout=writer)
    return

print "evaluate limit for m = ", opt.mass, " GeV"
os.system("mkdir -p ttDM%i" % opt.mass)
os.chdir("ttDM%i" % opt.mass)

post = "_" + opt.cat + ("" if opt.cat == "" else "_") + opt.analysis
runCombine("combine -M Asymptotic -n ttDM" + post + " -m %i -t -1 ttDM%i" + post + ".txt",
           "asymptotic" + post + ".log")
runCombine("combine -M ProfileLikelihood -n ttDM" + post + " -m %i --signif --pvalue -t 1000 --toysFreq --expectSignal=1 ttDM%i" + post + ".txt",
           "profileLikelihood" + post + ".log")

os.chdir("..");
