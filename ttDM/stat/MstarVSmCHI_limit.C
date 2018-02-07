#include "TStyle.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TGraphAsymmErrors.h"
#include "TLatex.h"
#include "TLine.h"
#include "TLegend.h"
#include "TAxis.h"
#include <TROOT.h>
#include <string>
#include <iostream>
#include <TLatex.h>
using namespace std;

double getMstar_fromXS(double thMstar, double expXS, double thXS) {
  return thMstar/pow(expXS/thXS,1./6);
}

double getEFT(double x) {
  return pow( pow(x/(2*acos(-1)),2) * 173.3, 1./3);
}

TGraph* get8TeVexpected() {

  const size_t NPOINTS = 7;

  float  mass[NPOINTS] = {
    1,10,
    50,
    100,200,600,1000
  };

  double eff[10] = {
    0.0100922,
    0.010136,
    0.0120235,
    0.0146044,
    0.0172842,
    0.0240019,
    0.0276004
  };

  double XS[10] = {
    0.80320E-06 * 0.240 ,
    0.78744E-06 * 0.240 ,
    0.65276E-06 * 0.227,
    0.49881E-06 * 0.209, 
    0.28221E-06 * 0.187, 
    0.18052E-07 * 0.148, 
    0.86130E-09 * 0.133
  };
  
  double exp[NPOINTS] = {
    0.935302,
    0.918515,
    0.915708,
    0.919305,
    0.916604,
    0.916469,
    0.913618
  };

  float MstarExp[NPOINTS];
  for(size_t j=0; j< NPOINTS; j++){
    exp[j]  = exp[j] * 10 / (19712 * eff[j] );
    MstarExp[j] = getMstar_fromXS(1000.,exp[j],XS[j]);
  }
  
  TGraph *gr3 = new TGraph(NPOINTS,mass,MstarExp);
  return gr3;
}



void MstarVSmCHI_limit_v(const string & method = "count", const string & cat = "", TGraph* m_y_line_graph_8TeV = NULL) {
  /* 
     Groot->SetStyle("Plain");
     gROOT->LoadMacro(".L tdrstyle.C");
     setTDRStyle();   
  */
  
  //  const int n_points = 6;
  //  double x_vals[n_points]={1, 10, 50, 200, 600, 1000};
  //  double thXS[n_points]={1.32,1.32,1.187,0.6301,0.1038,0.01585}; // x-sec in pb (M*=100 GeV)
  gROOT->SetStyle("Plain");
  gStyle->SetFillColor(0);  //white fill color
  gStyle->SetFrameBorderMode(0);  //no frame border
  gStyle->SetCanvasBorderMode(0);  //no canvas border
  gStyle->SetOptTitle(0);  //no title
  gStyle->SetOptStat(0);  //no statistics _or_

  //  gStyle->SetOptLogy(1);
  gStyle->SetOptLogx(1);
  TCanvas * c1 = new TCanvas("c1","c1",900,900);
  c1->SetFillStyle(4000);
  c1->SetFrameFillStyle(1000);
  c1->SetFrameFillColor(0);

#include "include/settings.h"
#include "include/limit_count.h"
#include "include/limit_semileptonic_count.h"
#include "include/limit_fullhadronic_count.h"
#include "include/limit_shape.h"
#include "include/limit_semileptonic_shape.h"
#include "include/limit_fullhadronic_shape.h"

  unsigned int imethod = 0; if (method == "shape") imethod = 1;
  unsigned int icat = 0; if(cat == "semileptonic") { icat = 1; } else if(cat == "fullhadronic") { icat = 2; }

#define VARRAY(name) \
  double * v_##name[3][2] = { { y_##name##_count, y_##name##_shape },	\
			      { y_##name##_semileptonic_count, y_##name##_semileptonic_shape }, \
			      { y_##name##_fullhadronic_count, y_##name##_fullhadronic_shape } }; \
  double * y_##name = v_##name[icat][imethod]
    
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
  
  //  std::cout << "cat: " << cat << " method: " << method << std::endl;
  for (int i=0;i<n_points;++i){

    double y_obs_point   = getMstar_fromXS(100.,y_observed[i],    thXS[i]);
    double y_up_point2   = getMstar_fromXS(100.,y_down_points2[i],thXS[i]);
    double y_up_point1   = getMstar_fromXS(100.,y_down_points1[i],thXS[i]);
    double y_exp_point   = getMstar_fromXS(100.,y_vals[i],        thXS[i]);  
    double y_down_point1 = getMstar_fromXS(100.,y_up_points1[i],  thXS[i]);
    double y_down_point2 = getMstar_fromXS(100.,y_up_points2[i],  thXS[i]);

    y_observed[i] = y_obs_point;
    y_vals[i]     = y_exp_point;

    //    std::cout << "y_vals[" << i << "]: " << y_vals[i] << std::endl;

    y_up_bars2[i]   = y_up_point2  -y_exp_point;
    y_up_bars1[i]   = y_up_point1 - y_exp_point;
    y_down_bars1[i] = y_exp_point - y_down_point1;
    y_down_bars2[i] = y_exp_point - y_down_point2;

  }
  
  // bline
  TGraph * m_y_line_graph = new TGraph(n_points, x_vals, y_vals);
  m_y_line_graph->SetTitle(" ");
  m_y_line_graph->SetLineWidth(2);
  m_y_line_graph->SetLineStyle(2);
  m_y_line_graph->SetFillColor(kWhite);
  m_y_line_graph->GetXaxis()->SetRangeUser(110, 150); 
  
  TGraph * m_y_lineObs_graph = new TGraph(n_points, x_vals, y_observed);
  m_y_lineObs_graph->SetTitle(" ");
  m_y_lineObs_graph->SetLineWidth(2);
  m_y_lineObs_graph->SetFillColor(kWhite);
 
  // y band 1 sigma
  TGraphAsymmErrors * m_y_band_graph_1sigma = new TGraphAsymmErrors(n_points, x_vals, y_vals, 0, 0, y_down_bars1, y_up_bars1);
  m_y_band_graph_1sigma->SetTitle(" ");
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
  m_y_band_graph_2sigma->GetXaxis()->SetTitleOffset(1.0);
  m_y_band_graph_2sigma->GetXaxis()->SetTitleSize(0.04);
  m_y_band_graph_2sigma->GetXaxis()->SetLabelSize(0.03);
  m_y_band_graph_2sigma->GetYaxis()->SetTitle("Upper limit on #sigma/#sigma_{th}");
  m_y_band_graph_2sigma->GetYaxis()->SetTitleOffset(1.1);
  m_y_band_graph_2sigma->GetYaxis()->SetTitleSize(0.04);
  m_y_band_graph_2sigma->GetYaxis()->SetLabelSize(0.03);

  // 8TeV results
  if (m_y_line_graph_8TeV != NULL) {
    m_y_line_graph_8TeV->SetLineWidth(4);
    m_y_line_graph_8TeV->SetLineStyle(2);
    m_y_line_graph_8TeV->SetLineColor(kRed);
    m_y_line_graph_8TeV->SetFillColor(kWhite);
    
  }
  

  const int n_EFTpoints = 1000;
  double EFTx_vals[n_EFTpoints];
  double EFTy_vals[n_EFTpoints];
  
  for (size_t i=0; i<n_EFTpoints; i++) {
    EFTx_vals[i] = i+1;
    EFTy_vals[i] = getEFT(EFTx_vals[i]);
  }

  TGraph *EFT = new TGraph(1000,EFTx_vals,EFTy_vals);
  EFT->SetTitle(" ");
  //  EFT->GetYaxis()->SetNdivisions(6,5,0);
  EFT->GetYaxis()->SetTitleOffset(1);
  EFT->GetXaxis()->SetTitleOffset(1);

  EFT->SetMarkerColor(kGray);
  EFT->SetLineColor(kGray);
  EFT->SetLineWidth(-20001);
  EFT->SetFillStyle(3001);
  EFT->SetFillColor(kGray);
  EFT->SetMarkerStyle(1);
  EFT->GetXaxis()->SetRangeUser(1.,1000.);
  EFT->GetYaxis()->SetRangeUser(1.,300.);
  EFT->GetXaxis()->SetTitle("m_{#chi} [GeV]");
  EFT->GetXaxis()->SetTitleSize(0.04);
  EFT->GetXaxis()->SetTitleOffset(1.0);
  EFT->GetXaxis()->SetLabelSize(0.03);
  EFT->GetYaxis()->SetTitle("95% CLs lower limits on M_{*} [GeV]");
  EFT->GetYaxis()->SetTitleSize(0.04);
  EFT->GetYaxis()->SetTitleOffset(1.1);
  EFT->GetYaxis()->SetLabelSize(0.03);

  // The legend
  TLegend * m_legend = new TLegend(0.6,0.62,0.85,0.82);
  m_legend->SetFillColor(0);  
  //  m_legend->SetLineColor(0);
  m_legend->SetName("DM exclusion");
  // uncomment for ob as well     
  // remove obs for now:  m_legend->AddEntry(m_y_lineObs_graph,"CL_{S} Observed");
  //  m_legend->AddEntry(m_y_lineSI_graph,"CL_{S} sign. injected");
  m_legend->AddEntry(m_y_line_graph,"Expected");
  m_legend->AddEntry(m_y_band_graph_1sigma,"Expected #pm 1 #sigma");
  m_legend->AddEntry(m_y_band_graph_2sigma,"Expected #pm 2 #sigma");
  if (m_y_line_graph_8TeV != NULL)
    m_legend->AddEntry(m_y_line_graph_8TeV,"Expected, 8 TeV");

  delete[] y_down_bars2;
  delete[] y_down_bars1;
  delete[] y_up_bars2;
  delete[] y_up_bars1;
  
  c1->cd();
  EFT->Draw("AL");
 
  int opt=4;

  // Bands
  if (opt==4){
    //    m_y_band_graph_2sigma->Draw("A3same");

    m_y_band_graph_2sigma->Draw("3Same");
    m_y_band_graph_1sigma->Draw("3Same");
    m_y_band_graph_1sigma->SetMaximum(20);
  }
  else{
    //    m_y_band_graph_2sigma->Draw("A4same");

    m_y_band_graph_2sigma->Draw("4Same");
    m_y_band_graph_1sigma->Draw("4Same");
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
  
  // Legen
  m_y_band_graph_2sigma->SetMaximum(20);
  m_legend->Draw("Same");

  if (m_y_line_graph_8TeV != NULL) {
    m_y_line_graph_8TeV->Draw("Lsame");
  }

  //  m_y_line_graph->GetXaxis()->SetRangeUser(115, 135); 
  
  TLatex* tex = new TLatex(0.7,0.15,"EFT not valid");
  tex->SetNDC();
  tex->SetTextSize(0.03);
  tex->SetLineWidth(2);
  tex->Draw();
  
  TLatex l;
  l.SetNDC();
  l.SetTextAlign(11); // align left
  l.SetTextSize(0.03);
  l.DrawLatex(0.12,0.91,"CMS Simulation, #sqrt{s} = 13 TeV, L = 5.0 fb^{-1}");
  map<string, string> methodLabel, catLabel;
  methodLabel["count"] = "cut-and-count analysis";
  methodLabel["shape"] = "shape analysis";
  catLabel["semileptonic"] = "single-lepton channel";
  catLabel["fullhadronic"] = "hadronic channel";
  catLabel[""] = "combined";
  string label = methodLabel[method] + ", " + catLabel[cat];
  l.DrawLatex(0.12, 0.86, label.c_str());

  m_y_line_graph->GetXaxis()->SetRangeUser(110, 150);

  c1->SaveAs((string("plots/ttDM_limit_MstarVSmCHI_") + cat + (cat == "" ? "" : "_") + method + ".pdf").c_str());
  c1->SaveAs((string("plots/ttDM_limit_MstarVSmCHI_") + cat + (cat == "" ? "" : "_") + method + ".png").c_str());
  c1->SaveAs((string("plots/ttDM_limit_MstarVSmCHI_") + cat + (cat == "" ? "" : "_") + method + ".C").c_str());
} 

void MstarVSmCHI_limit() {
  system("mkdir -p plots");
  string method[] = { "count", "shape" };
  string cat[] = { "", "semileptonic", "fullhadronic" };

  TGraph* exp_8TeV = get8TeVexpected();

  for(int i = 0; i < 2; ++i) 
    for(int j = 0; j < 3; ++j) {
      TGraph* m_y_line_graph_8TeV = exp_8TeV;
      if (cat[j].find("fullhadronic") != std::string::npos)
	m_y_line_graph_8TeV = NULL;

      MstarVSmCHI_limit_v(method[i], cat[j], m_y_line_graph_8TeV);
    }
}
