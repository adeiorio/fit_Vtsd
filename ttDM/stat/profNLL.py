#Reads a rootfile and prints out a scale factor, the upgrade would be to print also the nuisance parameter. For now it does it with Combine only
from array import array
#import ROOT
import time
import sys
#sys.argv.append('-b')
import ROOT
import commands, os
import numpy
import optparse

def plotNLL(process, lep):#, channel, sys='', histodir="./histos/", format="combine"):
    rootfile = "higgsCombine"+ process + lep +".MultiDimFit.mH120.root"
    fileFit = ROOT.TFile.Open(str(rootfile), "READ")
    c1 = ROOT.TCanvas("c1", "NLL_"+process +"_"+ lep)#, 1600, 800)
    c1.cd()
    c1.SetTopMargin(0.06)
    c1.SetRightMargin(0.05)
    c1.SetTicks(1, 1)
    # Draw
    (fileFit.Get("limit")).Draw("2*deltaNLL:r","quantileExpected<1 && 2*deltaNLL<10 && 2*deltaNLL!=0", "L")
    c1.Print(lep +"/NLL"+process+".png")
    c1.Print(lep +"/NLL"+process+".pdf")

    c2 = ROOT.TCanvas("c2", "NLL_"+process +"_"+ lep)#, 1600, 800)
    c2.cd()
    c2.SetTopMargin(0.06)
    c2.SetRightMargin(0.05)
    c2.SetTicks(1, 1)
    # Draw
    (fileFit.Get("limit")).Draw("2*deltaNLL:r","quantileExpected<1 && 2*deltaNLL!=0", "L")
    c2.Print(lep +"/NLL"+process+"_nocut.png")
    c2.Print(lep +"/NLL"+process+"_nocut.pdf")

#    (fileFit.Get("limit")).Draw("2*deltaNLL:r","quantileExpected<1", "L")
    '''
    leg = TLegend(0.35, 0.95, 0.65, 0.995)
    leg.SetBorderSize(0)
    leg.SetFillStyle(0)
    leg.SetFillColor(0)
    leg.SetNColumns(2)
    leg.AddEntry(b_pulls, "bkg-only fit", "fp")
    '''
#    fileout =  ROOT.TFile.Open( lep +"/NLL"+process+".root", "UPDATE")
