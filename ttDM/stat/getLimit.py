#! /usr/bin/env python
import sys
import os
import commands
import string
import optparse

usage = 'usage: %prog [--cat N]'
parser = optparse.OptionParser(usage)
parser.add_option('-c', '--cat', dest='cat', type='int', default = -1, help='Set the flag to -1 to avoid the combination of semileptonic and fullhadronic channel')
(opt, args) = parser.parse_args()

#print sys.argv[1] 
masses = ["1", "10", "50", "200", "600", "1000"]

os.system("mkdir -p include")
categories = ["", "semileptonic", "fullhadronic"]
if(opt.cat == 0): categories = ["semileptonic"]
if(opt.cat == 1): categories = ["fullhadronic"]
for cat in categories:
   for method in ["count", "shape"]:
      post = "_" + cat + ("" if cat == "" else "_") + method;
      i = 0;
      print cat , " ", method
      for item in masses:  
         os.system(("grep \"r < \" ttDM%s/asymptotic" + post + ".log| awk '{print $5}' >> a%i") % (item, i))
         count=0
         with open (('a%i' % i), 'rb') as f:
            for line in f:
               count += 1
         if(count < 2):
            os.system("echo '0\n0\n0\n0\n0\n' >> a%i" % i)
         i = 1 + i
      os.system("echo 'double y_observed" + post + "[n_points]=        {'>> left")
      os.system("echo 'double y_down_points2" + post + "[n_points]=    {'>> left")
      os.system("echo 'double y_down_points1" + post + "[n_points]=    {'>> left")
      os.system("echo 'double y_vals" + post + "[n_points]=            {'>> left")
      os.system("echo 'double y_up_points1" + post + "[n_points]=      {'>> left")
      os.system("echo 'double y_up_points2" + post + "[n_points]=      {'>> left")
      
      for i in range(0, 6):
         os.system("echo ','>>commas")
         os.system("echo '};'>>right")
         
      os.system("paste left a0 commas a1 commas a2 commas a3 commas a4 commas a5 right > include/limit" + post + ".h")
      os.system("more include/limit" + post + ".h")
      os.system("rm -rf left right commas a0 a1 a2 a3 a4 a5")
      
