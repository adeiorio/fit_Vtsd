import os
import subprocess
import optparse
import sys

usage = 'usage: %prog [--dir <dir>]'
parser = optparse.OptionParser(usage)
parser.add_option('-D', '--dir', dest='dir', type='string', default=".", help='Directory with limit subdirs')
parser.add_option('-M', '--method', dest='method', type='string', default="ount", help='Analysis method')
parser.add_option('-f', '--format', dest='format', type='string', default="latex", help='Output format')
parser.add_option('-i', '--interaction', dest='inter', type='string', default="sc", help='Interaction type')
parser.add_option('-o', '--observed', dest='obs', type='string', default="expected", help='Observed or expected limit')
(opt, args) = parser.parse_args()

print "% *** limit extraction table, automatically generated   ***"
print "% *** don't modify this LaTeX code, but rerun the macro ***"
mPhis = [10, 15, 20, 50, 95, 100, 200, 295, 300, 500, 995, 1000]
mChis = [1, 10, 50, 150, 500, 1000]
prefix = "DMtt_"+opt.inter+"_"
limPrefix = "asymptotic"+opt.inter+"_"
obs = opt.obs
limString = "Expected 50.0%"
if obs=="observed": limString = "Observed"

print "% limit directory: ", opt.dir
print "% limit method: ", opt.method

if(opt.format == "latex"):
    header = ("\\begin{tabular}{|r|*{%i}{r}|}\hline\n" % len(mPhis))
    header += ("$m_{\chi}$ (GeV) & \multicolumn{%i}{c|}{$m_{\phi}$ (GeV)}\\\\ \\hline" % len(mPhis))
    separator = " & "
    linebreak = "\\\\"
    linesep = "\\hline"
else:
    header = "% upper limits table "
    separator = "\t"
    linebreak = ""
    linesep = ""
print header

for mPhi in mPhis:
    sys.stdout.write("%s%i" % (separator, mPhi))
print linebreak, linesep
for mChi in mChis:
    sys.stdout.write("%i%s" % (mChi, separator))
    for mPhi in mPhis:
        massString = ("Mchi%iMphi%i" % (mChi, mPhi))
        dirName = opt.dir + "/" + prefix + massString
        limitStr = " "
        #        print "+++ working directory: ", os.getcwd()
        if(os.path.exists(dirName)):
            limFile = dirName + "/" + limPrefix + massString + "_" + opt.method + ".log"
            if(os.path.exists(limFile)):
                limitStr = " - "
                for line in open(limFile):
                    if limString in line:
                        limitStr = ("%.4f" % float(line.split('<')[1]))
            else: 
                limStr = " * " 
                print "--- File "+ limFile + " does not exist"
        #else: print dirName+" not found"
        sys.stdout.write(limitStr)
        if(mPhi == mPhis[-1]):
            sys.stdout.write(linebreak + "\n")
        else:
            sys.stdout.write(separator)
print linesep
if(opt.format == "latex"):
    print "\\end{tabular}"
