#include "TStyle.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TGraphAsymmErrors.h"
#include "TLatex.h"
#include "TLine.h"
#include "TLegend.h"
#include "TAxis.h"
#include <string>
#include <fstream>

using namespace std;

const int n_points = 6;
double x_vals[n_points]={1, 10, 50, 200, 600, 1000};

void readArrays(const string& fileName,
		double * o, double * d2, double * d1, double * v, double * u1, double * u2) {
  ifstream f(fileName);
  string dummy;
  f >> dummy; for(unsigned int i = 0; i < n_points; ++i ) f >> o[i];
  f >> dummy; for(unsigned int i = 0; i < n_points; ++i ) f >> d2[i];
  f >> dummy; for(unsigned int i = 0; i < n_points; ++i ) f >> d1[i];
  f >> dummy; for(unsigned int i = 0; i < n_points; ++i ) f >> v[i];
  f >> dummy; for(unsigned int i = 0; i < n_points; ++i ) f >> u1[i];
  f >> dummy; for(unsigned int i = 0; i < n_points; ++i ) f >> u2[i];
}

void brazil_signif_v(const string & method = "count", const string & cat = "") {
  /* 
     gROOT->SetStyle("Plain");
     gROOT->LoadMacro(".L tdrstyle.C");
     setTDRStyle();   
  */
  
  gStyle->SetOptLogy(0);
  TCanvas * c1 = new TCanvas;
  c1->SetFillStyle(4000);
  c1->SetFrameFillStyle(1000);
  c1->SetFrameFillColor(0);
  string fileName("data/signif_");
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
  m_y_line_graph->SetFillColor(kWhite);
  m_y_line_graph->GetXaxis()->SetRangeUser(110, 150); 
  
  TGraph * m_y_lineObs_graph = new TGraph(n_points, x_vals, y_observed);
  m_y_lineObs_graph->SetLineWidth(2);
  m_y_lineObs_graph->SetFillColor(kWhite);
 
  // m_y_lineSI_graph = new TGraph(n_points, x_vals, y_injected);
  // m_y_lineSI_graph->SetLineWidth(2);
  // m_y_lineSI_graph->SetLineColor(kRed);
  // m_y_lineSI_graph->SetLineStyle(2);
  // m_y_lineSI_graph->SetFillColor(kWhite);

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
  m_y_band_graph_2sigma->GetXaxis()->SetTitle("m_{#chi} [GeV]");
  m_y_band_graph_2sigma->GetXaxis()->SetTitleOffset(0.8);
  m_y_band_graph_2sigma->GetXaxis()->SetTitleSize(0.05);
  m_y_band_graph_2sigma->GetYaxis()->SetTitle("Expected significance");
  m_y_band_graph_2sigma->GetYaxis()->SetTitleOffset(0.9);
  m_y_band_graph_2sigma->GetYaxis()->SetTitleSize(0.05);
  
  TLine * m_three_line = new TLine(0, 3, 1000, 3);
  TLine * m_five_line = new TLine(0, 5, 1000, 5);
  
  // The legend
  TLegend * m_legend = new TLegend(0.12,0.45,0.30,0.25);
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
    m_y_band_graph_1sigma->SetMaximum(7.5);
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
  
  m_three_line->SetLineColor(2);
  m_three_line->Draw("Same");
  m_five_line->SetLineColor(2);
  m_five_line->Draw("Same");
  
  // Legen
  m_y_band_graph_2sigma->SetMaximum(7.5);
  m_legend->Draw("Same");
  
  //  m_y_line_graph->GetXaxis()->SetRangeUser(115, 135); 
  
  TLatex l;
  l.SetNDC();
  l.SetTextAlign(31); // align right
  l.SetTextSize(0.04);
  // l.SetTextSizePixels(24);
  //l.SetTextFont(42);
  
  l.SetTextAlign(11); // align left
  l.DrawLatex(0.12,0.91,"CMS Simulation, #sqrt{s} = 13 TeV, L = 5.0 fb^{-1}");
  map<string, string> methodLabel, catLabel;
  methodLabel["count"] = "cut-and-count analysis";
  methodLabel["shape"] = "shape analysis";
  catLabel["semileptonic"] = "single-lepton channel";
  catLabel["fullhadronic"] = "hadronic channel";
  catLabel[""] = "combined";
  string label = methodLabel[method] + ", " + catLabel[cat];
  l.DrawLatex(0.12, 0.15, label.c_str());

  l.SetTextSize(0.04);
  m_y_line_graph->GetXaxis()->SetRangeUser(110, 150);

  c1->SaveAs((string("plots/ttDM_signif_") + cat + (cat == "" ? "" : "_") + method + ".pdf").c_str());
}

void brazil_signif() {
  system("mkdir -p plots");
  string method[] = { "count", "shape" };
  string cat[] = { "", "semileptonic", "fullhadronic" };
  for(int i = 0; i < 2; ++i)
    for(int j = 0; j < 3; ++j)
      brazil_signif_v(method[i], cat[j]);
}
