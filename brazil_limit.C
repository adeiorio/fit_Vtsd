#include "TStyle.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TGraphAsymmErrors.h"
#include "TLatex.h"
#include "TLine.h"
#include "TLegend.h"
#include "TAxis.h"
#include "TPad.h"
//#include "CMS_lumi.C"
//#include "tdrstyle.C"
#include <string>
#include <map>
#include <iostream>
#include <fstream>

using namespace std;

const int n_points = 7;
double x_vals[n_points]={10, 20, 50, 100, 200, 300, 500};

void readArrays(const string& fileName,
		double * o, double * d2, double * d1, double * v, double * u1, double * u2) {
  ifstream f(fileName.c_str());
  string dummy;
  f >> dummy; for(unsigned int i = 0; i < n_points; ++i ) f >> o[i];
  f >> dummy; for(unsigned int i = 0; i < n_points; ++i ) f >> d2[i];
  f >> dummy; for(unsigned int i = 0; i < n_points; ++i ) f >> d1[i];
  f >> dummy; for(unsigned int i = 0; i < n_points; ++i ) f >> v[i];
  f >> dummy; for(unsigned int i = 0; i < n_points; ++i ) f >> u1[i];
  f >> dummy; for(unsigned int i = 0; i < n_points; ++i ) f >> u2[i];
}

void brazil_limit_v(const string & method = "shape", const string & cat = "") {
  /* 
     Groot->SetStyle("Plain");
     gROOT->LoadMacro(".L tdrstyle.C");
     setTDRStyle();   
  */
  
  gStyle->SetOptLogy(1);
  gStyle->SetOptLogx(1);
  TCanvas * c1 = new TCanvas;
  c1->SetFillStyle(4000);
  c1->SetFrameFillStyle(1000);
  c1->SetFrameFillColor(0);

  c1->SetLogy(1);
  c1->SetGridx();
  c1->SetGridy();
  // CMS_lumi(c1, 4, 10);

  //1s->Print(Form("%s_limit.pdf",name.Data()));

  string fileName("data/limit_");
  if(cat != "") { fileName += cat; fileName += "_"; }
  fileName += method; 
  fileName += ".txt";

  double y_observed[n_points];
  double y_down_points2[n_points];
  double y_down_points1[n_points];
  double y_vals[n_points];
  double y_up_points1[n_points];
  double y_up_points2[n_points];
  
  readArrays(fileName, 
	     y_observed, y_down_points2, y_down_points1, y_vals, y_up_points1, y_up_points2);
  // Prepare errorbars
  double y_down_bars2[n_points];
  double y_down_bars1[n_points];
  double y_up_bars1[n_points];
  double y_up_bars2[n_points];
  
  for (int i=0;i<n_points;++i){
    y_down_bars2[i]=y_vals[i]-y_down_points2[i];
    y_down_bars1[i]=y_vals[i]-y_down_points1[i];   
    y_up_bars2[i]=y_up_points2[i]-y_vals[i];
    y_up_bars1[i]=y_up_points1[i]-y_vals[i];
  }
  
  // bline
  TGraph * m_y_line_graph = new TGraph(n_points, x_vals, y_vals);
  m_y_line_graph->SetLineWidth(2);
  m_y_line_graph->SetLineStyle(2);
  m_y_line_graph->SetFillColor(0);
  m_y_line_graph->GetXaxis()->SetRangeUser(110, 150); 
  
  TGraph * m_y_lineObs_graph = new TGraph(n_points, x_vals, y_observed);
  m_y_lineObs_graph->SetLineWidth(2);
  m_y_lineObs_graph->SetFillColor(kWhite);
 
  // y band 1 sigma
  TGraphAsymmErrors * m_y_band_graph_1sigma = new TGraphAsymmErrors(n_points, x_vals, y_vals, 0, 0, y_down_bars1, y_up_bars1);
  m_y_band_graph_1sigma->SetFillColor(kGreen);
  m_y_band_graph_1sigma->SetLineColor(kGreen);
  m_y_band_graph_1sigma->SetMarkerColor(kGreen);
  // y band 2 sigma
  TGraphAsymmErrors * m_y_band_graph_2sigma = new TGraphAsymmErrors(n_points, x_vals, y_vals, 0, 0,	y_down_bars2, y_up_bars2);
  m_y_band_graph_2sigma->SetTitle(" ");
  m_y_band_graph_2sigma->SetFillColor(kYellow);
  m_y_band_graph_2sigma->SetFillColor(kYellow);
  m_y_band_graph_2sigma->SetLineColor(kYellow);
  m_y_band_graph_2sigma->SetMarkerColor(kYellow);
  m_y_band_graph_2sigma->GetXaxis()->SetTitle("m_{MED} [GeV]");
  m_y_band_graph_2sigma->GetXaxis()->SetTitleOffset(0.8);
  m_y_band_graph_2sigma->GetXaxis()->SetTitleSize(0.04);
  m_y_band_graph_2sigma->GetYaxis()->SetTitle("95% CL upper limit on #sigma/#sigma_{th}");
  m_y_band_graph_2sigma->GetYaxis()->SetTitleOffset(0.65);
  m_y_band_graph_2sigma->GetYaxis()->SetTitleSize(0.04);
  m_y_band_graph_2sigma->GetYaxis()->SetTitleFont(42);
  
  TLine * m_one_line = new TLine(0, 1, 1000,1);
  
  // The legend
  TLegend * m_legend = new TLegend(0.57,0.54,0.77,0.74);
  m_legend->SetFillStyle(0);
  m_legend->SetBorderSize(0);
  m_legend->SetName("DM exclusion");
  // uncomment for ob as well     
  // remove obs for now:  m_legend->AddEntry(m_y_lineObs_graph,"CL_{S} Observed");
  //  m_legend->AddEntry(m_y_lineSI_graph,"CL_{S} sign. injected");
  m_legend->AddEntry(m_y_line_graph,"Expected");
  m_legend->AddEntry(m_y_band_graph_1sigma,"Expected #pm 1 #sigma");
  m_legend->AddEntry(m_y_band_graph_2sigma,"Expected #pm 2 #sigma");

  m_legend->SetFillColor(0);  
  
  c1->cd();
  int opt=4;
  // Bands
  if (opt==4){
    m_y_band_graph_2sigma->Draw("A3");
    m_y_band_graph_1sigma->Draw("3");
    m_y_band_graph_1sigma->SetMaximum(200);
  }
  else{
    m_y_band_graph_2sigma->Draw("A4");
    m_y_band_graph_1sigma->Draw("4");
  }
  
  // Lines
  if (opt==4){
    m_y_line_graph->Draw("L");
  }
  else{
    m_y_line_graph->Draw("C");
  }
     
  m_y_lineObs_graph->SetMarkerStyle(20);
  m_y_lineObs_graph->SetMarkerSize(1.1);
  
  // uncommnet for obs as well
  // remove obs for now:  m_y_lineObs_graph->Draw("LP");
  m_y_lineObs_graph->SetTitle(" ");
    
  // m_y_lineSI_graph->Draw("same");
  // m_y_lineSI_graph->SetTitle(" ");
  
  m_one_line->SetLineColor(2);
  // m_one_line->Draw("Same");
  
  // Legen
  m_y_band_graph_2sigma->SetMaximum(200);
  m_legend->Draw("Same");
  
  //  m_y_line_graph->GetXaxis()->SetRangeUser(115, 135); 
  
  float lumiTextSize     = 0.6;
  float lumiTextOffset   = 0.2;
  //float cmsTextSize      = 0.75;
  //float cmsTextOffset    = 0.1;  // only used in outOfFrame version

  TPad* pad = new TPad("pad","pad",0, 0 , 1, 1);

  pad->SetBorderMode(0);

  pad->SetTickx(0);
  pad->SetTicky(0);
  // pad->Draw();
  //pad->cd();

  float t = pad->GetTopMargin();
  float r = pad->GetRightMargin();

  TString cmsText     = "CMS";
  float cmsTextFont   = 61;  // default is helvetic-bold
  float cmsTextSize      = 0.75;
  float cmsTextOffset    = 0.1;  // only used in outOfFrame version


  // writeExtraText = false;
  // extraText   = "Preliminary";
  // lumi_sqrtS = "2.2 fb^{-1} (13 TeV)";
  // //float extraTextFont = 52;  // default is helvetica-italics
  // int iPeriod = 0; 
  // int iPos = 11; 
  // int W = 800;
  // int H = 600;
  // int H_ref = 600; 
  // int W_ref = 800; 

  // float T = 0.08*H_ref;
  // float B = 0.12*H_ref; 
  // float L = 0.12*W_ref;
  // float R = 0.04*W_ref;

  // CMS_lumi(pad, iPeriod, iPos);

  // pad1= ROOT.TPad("pad1", "pad1", 0, 0 , 1, 1)

   

  TString lumiText = "2.2 fb^{-1} (13 TeV)";
  TLatex latex_lumi;
  latex_lumi.SetNDC();
  latex_lumi.SetTextAngle(0);
  latex_lumi.SetTextColor(kBlack);    

  ///float extraTextSize = extraOverCmsTextSize*cmsTextSize;

  latex_lumi.SetTextFont(42);
  latex_lumi.SetTextAlign(31); 
  latex_lumi.SetTextSize(lumiTextSize*t*0.55);    
  latex_lumi.DrawLatex(1-r,1-t+lumiTextOffset*t,lumiText);

  TLatex latex_cms;

  latex_cms.SetTextFont(cmsTextFont);
  latex_cms.SetTextSize(cmsTextSize*t);
  latex_cms.SetTextAlign(11);
  // latex_cms.DrawLatex(0.12,0.91,cmsText);

 
  TLatex l_ch;
  l_ch.SetNDC();
  l_ch.SetTextSize(lumiTextSize*t*0.55);
  // l.SetTextSizePixels(24);
  l_ch.SetTextFont(62);
  l_ch.SetTextAlign(11); // align left
  // l.DrawLatex(0.13,1-t+lumiTextOffset*t*,"CMS");
  l_ch.DrawLatex(0.13,0.705,"DM + tt");

  TLatex l;
  l.SetNDC();
  l.SetTextSize(lumiTextSize*t*0.8);
  // l.SetTextSizePixels(24);
  //l.SetTextFont(42);
  l.SetTextAlign(11); // align left
  // l.DrawLatex(0.13,1-t+lumiTextOffset*t*,"CMS");
  l.DrawLatex(0.13,0.85,"CMS");

  TLatex l_preliminary;
  l_preliminary.SetNDC();
  l_preliminary.SetTextAlign(31); // align right
  l_preliminary.SetTextSize(lumiTextSize*t*0.65);
  l_preliminary.SetTextFont(52);
  l_preliminary.SetTextAlign(11); // align left
  //  l_preliminary.DrawLatex(0.17,1-t+lumiTextOffset*t,"Preliminary");
  l_preliminary.DrawLatex(0.13, 0.81,"Preliminary");

  TLatex l_label;
  l_label.SetNDC();
  l_label.SetTextAlign(31); // align right
  l_label.SetTextSize(lumiTextSize*t*0.5);
  l_label.SetTextFont(52);
  l_label.SetTextAlign(11); // align left
  l_label.DrawLatex(0.13,0.66,"0 + 1 lepton categories");
  l_label.SetTextFont(42);
  //  l_label.DrawLatex(0.13,0.6,"Scalar mediator");
  l_label.DrawLatex(0.13,0.6,"Pseudoscalar mediator");
  l_label.DrawLatex(0.13,0.55,"g_{#chi} = g_{q} = 1, m_{#chi} = 1 GeV");


  map<string, string> methodLabel, catLabel;
  methodLabel["count"] = "cut-and-count analysis";
  methodLabel["shape"] = "shape analysis";
  catLabel["semileptonic"] = "single-lepton channel";
  catLabel["fullhadronic"] = "hadronic channel";
  catLabel[""] = "combined";
  //  string label = methodLabel[method] + ", " + catLabel[cat];
  string label_1 = "0 + 1 lepton categories";
  string label_2 = "Scalar mediator";
  // string label_3 = "g_{\chi} = g_{q} = 1, m_{\chi} = 1~GeV";
  // l.DrawLatex(0.12, 0.85, label_1.c_str());

  // l.SetTextSize(0.04);
  m_y_line_graph->GetXaxis()->SetRangeUser(110, 150);

  c1->cd();
  c1->Update();
  c1->SaveAs((string("plots/ttDM_limit_") + cat + (cat == "" ? "" : "_") + method + ".pdf").c_str());

  // compute limit interpolating
  unsigned int imin = 0, imax = n_points - 1;
  double xmin = x_vals[imin], xmax = x_vals[imax], ymin = y_vals[imin], ymax = y_vals[imax];
  cout << label_1 << ": upper limit." << endl;
  if(ymin > 1) { cout << "Upper limit on m_chi < " << xmin << " GeV" << endl; }
  else if(ymax < 1) { cout << "Upper limit on m_chi > " << xmax << " GeV" << endl; }
  else {
    double xlim = 0;
    do {
      if(imax - imin == 1) {
	xlim = xmin + (xmax - xmin)/(ymax - ymin)*(1.0 - ymin);
	imax = imin;
      } else {
	unsigned int i = (imax + imin)/2;
	if (y_vals[i] < 1) {
	  imin = i; xmin = x_vals[i]; ymin = y_vals[i];
	} else {
	  imax = i; xmax = x_vals[i]; ymax = y_vals[i];
	}
      }
    } while(imax != imin);
    cout << "Upper limit on m_chi = " << xlim << " GeV" << endl;
  }
} 

void brazil_limit() {
  system("mkdir -p plots");
  string method[] = { "shape" };
  string cat[] = { ""};
  brazil_limit_v(method[0], cat[0]);
}
