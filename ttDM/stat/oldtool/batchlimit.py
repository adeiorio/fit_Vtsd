#!/usr/bin/env python
import os, commands
import string
import optparse 

usage = 'usage: %prog -r runNum'
parser = optparse.OptionParser(usage)

parser.add_option("-D","--dir",dest="odir",type="string",default="",help="Outdir created by the analyseSamples")
parser.add_option("-i","--interaction",dest="int",type="string",default="sc",help="Kind of interaction: sc or ps")
parser.add_option('-M', '--method', dest='method', type='string', default = 'shape', help='Run a single method (ctrl, shape, count)')
(opt, args) = parser.parse_args()

filename = "runBatch"
ext = ".csh"





#masses = { 100: [1]}

masses = { 10: [1, 10, 50, 150, 500, 1000],
           20: [1],
           15: [10],
           50: [1, 10, 50],
           95: [50],
           100: [1, 10],
           200: [1, 50, 150],
           295: [150],
           300: [1, 50],
           500: [1, 150, 500],
           995: [500],
           1000: [1, 150, 1000],
           1995: [1000],
           2000: [1, 500],
           10000: [1, 10, 50, 150, 500, 1000]
           }

if(not os.path.isdir("txt")): os.makedirs("txt")


for mPhi in masses.keys():
    for mChi in masses[mPhi]:
        print mPhi, mChi
        f = open(filename+ext, "r")
        nf = "txt/"+filename+"_mchi_"+str(mChi)+"_mPhi_"+str(mPhi)+"_"+opt.int + "_" + opt.odir+"_"+opt.method+ext
        newfile = open(nf, "w")
#        print newfile
        lines = f.readlines()
        for l in lines:
            newline = l
            if(l.startswith("python")):
                newline = string.replace(l, "--mchi 1", "--mchi " + str(mChi))
                newline = string.replace(newline, "--mphi 10", "--mphi " + str(mPhi))
                newline = string.replace(newline, "-D testNoSyst/", "-D " + opt.odir)
                newline = string.replace(newline, "--interaction sc", "--interaction " + opt.int)
                newline = string.replace(newline, "-M shape", "-M  " + opt.method)

            #print newline
            newfile.write(newline)
                        
        cmd = "chmod 775 "+ nf
        #print cmd
        os.system(cmd)
        cmd = "bsub -q 8nm "+ os.getcwd()+"/" + nf
        print cmd
        os.system(cmd)

cmd = "bjobs"
os.system(cmd)
