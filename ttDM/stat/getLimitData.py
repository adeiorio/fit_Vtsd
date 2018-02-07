#! /usr/bin/env python
import sys
import os
import commands
import string
import optparse

usage = 'usage: %prog [--cat N]'
parser = optparse.OptionParser(usage)
parser.add_option('-c', '--cat', dest='cat', type='int', default = -1, help='Set the flag to -1 to avoid the combination of semileptonic and fullhadronic channel')
parser.add_option('', '--mchi', dest='mchi', type='int', default = 1, help='DM mass')
parser.add_option('-i', '--interaction', dest='interaction', type='string', default='sc', help='Interaction model (sc, ps)')
parser.add_option('-M', '--method', dest='method', type='string', default = 'shape', help='Run a single method (ctrl, shape, count)')
parser.add_option('-D', '--dir', dest='dir', type='string', default = '.', help='datacards direcotry')

(opt, args) = parser.parse_args()

#print sys.argv[1] 
masses = ["10", "20", "50", "100", "200", "300", "500"]

os.system("mkdir -p data")
categories = [""]
if(opt.cat == 0): categories = ["semileptonic"]
if(opt.cat == 1): categories = ["fullhadronic"]
for cat in categories:
   for method in ["shape"]:
      post = "_" + cat + ("" if cat == "" else "_") + method;
      i = 0;
      print cat , " ", method
      for item in masses:

         os.system(("grep \"r < \" %s/DMtt_%s_Mchi%sMphi%s/asymptotic%s_Mchi%sMphi%s" + post + ".log| awk '{print $5}' >> a%i") % (opt.dir, str(opt.interaction), str(opt.mchi), item, opt.interaction, str(opt.mchi), item, i))

         print ("grep \"r < \" %s/DMtt_%s_Mchi%sMphi%s/asymptotic%s_Mchi%sMphi%s" + post + ".log| awk '{print $5}' >> a%i") % (opt.dir, str(opt.interaction), str\
(opt.mchi), item, opt.interaction, str(opt.mchi), item, i)
         count=0
         with open (('a%i' % i), 'rb') as f:
            for line in f:
               count += 1
         if(count < 2):
            os.system("echo '0\n0\n0\n0\n0\n' >> a%i" % i)
         i = 1 + i
      os.system("echo 'y_observed" + post + "        '>> left")
      os.system("echo 'y_down_points2" + post + "    '>> left")
      os.system("echo 'y_down_points1" + post + "    '>> left")
      os.system("echo 'y_vals" + post + "            '>> left")
      os.system("echo 'y_up_points1" + post + "      '>> left")
      os.system("echo 'y_up_points2" + post + "      '>> left")
      
      for i in xrange(len(masses)):
         os.system("echo ' '>>commas")
         os.system("echo ' '>>right")
         
      #os.system("paste left a0 commas a1 commas a2 commas a3 commas a4 commas a5  right > "+opt.dir+"/limit" + post + ".txt")
      #os.system("more " + opt.dir + "/limit"+post + ".txt")
         
      os.system("rm data/limit"+post + ".txt")
      os.system("paste left a0 commas a1 commas a2 commas a3 commas a4 commas a5 commas a6 right > data/limit" + post + ".txt")
      os.system("more data/limit"+post + ".txt")

      os.system("rm -rf left right commas a0 a1 a2 a3 a4 a5 a6")
      
