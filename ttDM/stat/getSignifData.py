#! /usr/bin/env python
import sys
import os
import commands
import string
import subprocess
import optparse

usage = 'usage: %prog [--cat N]'
parser = optparse.OptionParser(usage)
parser.add_option('-c', '--cat', dest='cat', type='int', default = -1, help='Set the flag to -1 to avoid the combination of semileptonic and fullhadronic channel')
(opt, args) = parser.parse_args()

#print sys.argv[1] 
masses= ["1", "10", "50", "200", "600", "1000"]

os.system("mkdir -p data");
categories = ["", "semileptonic", "fullhadronic"]
if(opt.cat == 0): categories = ["semileptonic"]
if(opt.cat == 1): categories = ["fullhadronic"]
for cat in categories:
   for method in ["count", "shape"]:
      post = "_" + cat + ("" if cat == "" else "_") + method;
      i = 0;
      for item in masses:  
         cmd = (("root -b -l -q 'signifBand.C+(\"ttDM%s/higgsCombinettDM" + post + ".ProfileLikelihood.mH%s.123456.root\")'") % (item,item))
         writer = open("signif" + post + ".log", 'w') 
         print cmd
         process = subprocess.call(cmd, shell = True, stdout=writer)
         os.system("cat signif" + post + ".log")
         os.system(("grep 'Significance' ttDM%s/profileLikelihood" + post + ".log | sed 's/)//' | sed 's/inf/8.0/' |awk '{print $3}' >> a%i") % (item, i));
         os.system(("grep \"r < \" signif" + post + ".log| awk '{print $6}' >> a%i") % i)
         os.system("rm -rf signif" + post + ".log")
         i = i + 1

      os.system("echo 'y_observed" + post + "        '>> left")
      os.system("echo 'y_down_points2" + post + "    '>> left")
      os.system("echo 'y_down_points1" + post + "    '>> left")
      os.system("echo 'y_vals" + post + "            '>> left")
      os.system("echo 'y_up_points1" + post + "      '>> left")
      os.system("echo 'y_up_points2" + post + "      '>> left")
      
      for i in range(0, 6):
         os.system("echo ' '>>commas")
         os.system("echo ' '>>right")
         
      os.system("paste left a0 commas a1 commas a2 commas a3 commas a4 commas a5 right > data/signif" + post + ".txt")
      os.system("cat data/signif" + post + ".txt")
      os.system("rm -rf left right commas a0 a1 a2 a3 a4 a5")
