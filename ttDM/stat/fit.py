import os, commands
import optparse
from profNLL import plotNLL

usage = 'python fit.py'
#python fit.py -P ST_tch -i metadatacards_st -D datac_muon -L muon 
parser = optparse.OptionParser(usage)
parser.add_option('-L','--lep', dest='lep', type='string', default = 'muon', help='Default make muon analysis')
parser.add_option('-S', '--syst', dest='syst', type='string', default = 'all', help='Default all systematics added')
parser.add_option('--imp', dest='impact', default = False, action='store_true', help='')
parser.add_option('--maxL', dest='maxL', default = False, action='store_true', help='')
parser.add_option('--asymp', dest='asymp', default = False, action='store_true', help='')
parser.add_option('--multi', dest='multi', default = False, action='store_true', help='')
parser.add_option('-i', '--datac_in', dest='datac_in', type='string', help='')
parser.add_option('-D', '--datac_out', dest='datac_out', type='string', help='')
parser.add_option('-P', '--process', dest='process', type='string', default = 'all', help="")
#parser.add_option('-s', '--stack', dest='stack', default = False, action='store_true', help='')
(opt, args) = parser.parse_args()

metadatacard_in = opt.datac_in + "/" + opt.process + "_" + opt.lep
metadatacard_out = opt.datac_out + "/" + opt.process + "/" + opt.process + "_shape" 
print "metadatacard_in: ", metadatacard_in
print "metadatacard_out: ", metadatacard_out

os.system("rm -rf "+ opt.datac_out + "/ws")
os.system("rm -rf "+ opt.datac_out + "/" + opt.process)
#os.system("rm -rf "+ opt.datac_out + "/"+ metadatacard_in)

os.system("analyzeSamples -D "+ opt.datac_out + " -d " + metadatacard_in + ".txt")
os.system("text2workspace.py " + metadatacard_out + ".txt -m 125")

if(opt.impact):
    os.system("combineTool.py -M Impacts -d  " + metadatacard_out + ".root -m 125 --autoBoundsPOIs r --startFromPreFit 1 --robustFit 1 --autoRange 1 --doInitialFit")
    os.system("combineTool.py -M Impacts -d  " + metadatacard_out + ".root -m 125 --autoBoundsPOIs r --startFromPreFit 1 --robustFit 1 --autoRange 1 --doFits")
    os.system("combineTool.py -M Impacts -d  " + metadatacard_out + ".root -m 125 --autoBoundsPOIs r --startFromPreFit 1 --autoRange 1 -o prova.json")
    os.system("plotImpacts.py -i prova.json -t rename.json -o impacts")
    os.system("mv impacts.pdf "+opt.lep+"/impacts_"+opt.process+"_"+opt.lep+".pdf")
    print opt.lep+"/impacts_"+opt.process+"_"+opt.lep+".pdf"

if(opt.maxL):
#    os.system("combine -M MaxLikelihoodFit " + metadatacard_out + ".txt --saveShapes --saveWithUncertainties --plot --robustFit 1 --customStartingPoint --out " + opt.lep + " --saveNLL --keepFailures --autoBoundsPOIs r")
#    os.system("combine -M MaxLikelihoodFit " + metadatacard_out + ".txt --saveShapes --saveWithUncertainties --plots --saveNormalizations --robustFit 1 --customStartingPoint --stepSize 0.004 --initFromBonly --out " + opt.lep + " --profilingMode all --keepFailures ")
    os.system("combine -M FitDiagnostics " + metadatacard_out + ".txt --saveShapes --saveWithUncertainties --plots --saveNormalizations --robustFit 1 --customStartingPoint --stepSize 0.004 --initFromBonly --out " + opt.lep + " --profilingMode all --keepFailures  --autoBoundsPOIs r")
# --minos=all slower but better
    print "Making pull distributions ..."
    os.system("./make_pulls.sh " + opt.lep +"/fitDiagnostics.root")
    os.system("cat pulls.txt")
    os.system("python pulls.py -f pulls.txt")
    os.system("mv pulls.txt "+opt.lep+"/pulls_"+opt.process+"_"+opt.lep+".txt") 
    os.system("mv pulls.tex "+opt.lep+"/pulls_"+opt.process+"_"+opt.lep+".tex") 
    os.system("mv pulls_no_binbybin.png "+opt.lep+"/pulls_"+opt.process+"_"+opt.lep+".png") 
    os.system("mv rho_no_binbybin.png "+opt.lep+"/rho_"+opt.process+"_"+opt.lep+".png")
    os.system("mv pulls_no_binbybin.pdf "+opt.lep+"/pulls_"+opt.process+"_"+opt.lep+".pdf") 
    os.system("mv rho_no_binbybin.pdf "+opt.lep+"/rho_"+opt.process+"_"+opt.lep+".pdf")
    os.system("python rateParam.py -L "+opt.lep+" "+opt.lep+"/fitDiagnostics.root >& "+opt.lep+"/infoPF_"+opt.process+"_"+opt.lep+".log ")
    os.system("cat "+opt.lep+"/infoPF_"+opt.process+"_"+opt.lep+".log")

if(opt.asymp):
    os.system("combine " +  metadatacard_out + ".txt -M AsymptoticLimits -S 1 -t 1 -n " + opt.process + opt.lep)

if(opt.multi):
#    os.system("combine " + metadatacard_out + ".txt -M MultiDimFit --poi q2ST_tch --startFromPreFit 1 --stepSize 0.004 --algo=grid --autoRange 3 --points=60  -n " + opt.process + opt.lep)
#    os.system("combine " + metadatacard_out + ".txt -M MultiDimFit --redefineSignalPOI q2ST_tch --freezeNuisances r --startFromPreFit 1 --stepSize 0.004 --algo=grid --autoRange 3 --points=60  -n " + opt.process + opt.lep)
    os.system("combine " + metadatacard_out + ".txt -M MultiDimFit --startFromPreFit 1 --stepSize 0.004 --algo=grid --autoRange 6 --points=200  -n " + opt.process + opt.lep)
    plotNLL(opt.process,opt.lep)

