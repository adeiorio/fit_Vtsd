#include "TStyle.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TGraphAsymmErrors.h"
#include "TLatex.h"
#include "TLine.h"
#include "TLegend.h"
#include "TAxis.h"
#include <string>

using namespace std;

void brazil_limit_v(const string & method = "count", const string & cat = "") {
  /* 
     Groot->SetStyle("Plain");
     gROOT->LoadMacro(".L tdrstyle.C");
     setTDRStyle();   
  */
  
  const int n_points = 6;
  double x_vals[n_points]={1, 10, 50, 200, 600, 1000};
  gStyle->SetOptLogy(1);
  TCanvas * c1 = new TCanvas;
  c1->SetFillStyle(4000);
  c1->SetFrameFillStyle(1000);
  c1->SetFrameFillColor(0);

#include "include/limit_semileptonic_count.h"
#include "include/limit_semileptonic_shape.h"

  unsigned int imethod = 0; if (method == "shape") imethod = 1;

#define VARRAY(name) \
  double * v_##name[2] = { y_##name##_semileptonic_count, y_##name##_semileptonic_shape }; \
  double * y_##name = v_##name[imethod]
    
  VARRAY(observed);
  VARRAY(down_points2);
  VARRAY(down_points1);
  VARRAY(vals);
  VARRAY(up_points1);
  VARRAY(up_points2);
  
  // Prepare errorbars
  double* y_down_bars2 = new double[n_points];
  double* y_down_bars1 = new double[n_points];
  double* y_up_bars1 = new double[n_points];
  double* y_up_bars2 = new double[n_points];
  
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
  m_y_band_graph_2sigma->GetXaxis()->SetTitle("m_{DM} [GeV]");
  m_y_band_graph_2sigma->GetXaxis()->SetTitleOffset(0.8);
  m_y_band_graph_2sigma->GetXaxis()->SetTitleSize(0.05);
  m_y_band_graph_2sigma->GetYaxis()->SetTitle("95% Asymptotic CL Limit on #sigma/#sigma_{th}");
  m_y_band_graph_2sigma->GetYaxis()->SetTitleOffset(0.9);
  m_y_band_graph_2sigma->GetYaxis()->SetTitleSize(0.05);
  
  TLine * m_one_line = new TLine(0, 1, 1000,1);
  
  // The legend
  TLegend * m_legend = new TLegend(0.65,0.15,0.85,0.40);
  m_legend->SetName("DM exclusion");
  // uncomment for ob as well     
  // remove obs for now:  m_legend->AddEntry(m_y_lineObs_graph,"CL_{S} Observed");
  //  m_legend->AddEntry(m_y_lineSI_graph,"CL_{S} sign. injected");
  m_legend->AddEntry(m_y_line_graph,"CL_{S} Expected");
  m_legend->AddEntry(m_y_band_graph_1sigma,"CL_{S} Expected #pm 1 #sigma");
  m_legend->AddEntry(m_y_band_graph_2sigma,"CL_{S} Expected #pm 2 #sigma");

  m_legend->SetFillColor(0);  
  delete[] y_down_bars2;
  delete[] y_down_bars1;
  delete[] y_up_bars2;
  delete[] y_up_bars1;
  
  c1->cd();
  int opt=4;
  // Bands
  if (opt==4){
    m_y_band_graph_2sigma->Draw("A3");
    m_y_band_graph_1sigma->Draw("3");
    m_y_band_graph_1sigma->SetMaximum(20);
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
  m_one_line->Draw("Same");
  
  // Legen
  m_y_band_graph_2sigma->SetMaximum(20);
  m_legend->Draw("Same");
  
  //  m_y_line_graph->GetXaxis()->SetRangeUser(115, 135); 
  
  TLatex l;
  l.SetNDC();
  l.SetTextAlign(31); // align right
  l.SetTextSize(0.04);
  // l.SetTextSizePixels(24);
  //l.SetTextFont(42);
  
  l.SetTextAlign(11); // align left
  l.DrawLatex(0.20,0.91,"CMS Simulation, ttDM analysis");
  //l.DrawLatex(0.17,0.85,"CMS Preliminary, M_{JJ} analysis");
  l.DrawLatex(0.20,0.80,"#sqrt{s} = 13 TeV, L = 1.0 fb^{-1}");
  l.SetTextSize(0.04);
  m_y_line_graph->GetXaxis()->SetRangeUser(110, 150);

  c1->SaveAs((string("plots/ttDM_limit_") + cat + (cat == "" ? "" : "_") + method + ".pdf").c_str());
} 

void brazil_limit_semileptonic() {
  system("mkdir -p plots");
  string method[] = { "count", "shape" };
  for(int i = 0; i < 2; ++i) 
    brazil_limit_v(method[i], "semileptonic");
}
