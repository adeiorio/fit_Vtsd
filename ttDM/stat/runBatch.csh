#!/bin/tcsh
#cd /afs/cern.ch/work/o/oiorio/b2g_test3/CMSSW_7_3_0/src/ttDM/stat
cd /afs/cern.ch/work/d/decosa/private/ttDM/CMSSW_7_3_0/src/ttDM/stat 
eval `scramv1 runtime -csh`
python combine_ttDM_simpl.py -M shape -D testNoSyst/ --mchi 1 --mphi 10 --interaction sc
