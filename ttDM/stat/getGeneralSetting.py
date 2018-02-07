#! /usr/bin/env python
import sys
import os
import commands
import string

#print sys.argv[1] 
#masses = ["1", "10", "50", "200", "600", "1000"]

os.system("echo 'const int n_points = 6;'>> npoints")
os.system("echo 'double x_vals[n_points]={1, 10, 50, 200, 600, 1000};'>> masses")
os.system("echo 'double thXS[n_points] = {1.32,1.32,1.187,0.6301,0.1038,0.01585};'>> thXS")

os.system("paste npoints masses thXS > include/settings.h")
os.system("more include/settings.h")
os.system("rm -rf npoints masses thXS")
