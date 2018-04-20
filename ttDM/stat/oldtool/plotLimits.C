#include "TH2.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TGraph2D.h"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <cmath>

using namespace std;

void plotLimits() {
  TCanvas canv("canv", "", 800, 800);
  gStyle->SetOptStat(0);
  gStyle->SetPalette(55);
  const unsigned int xbins = 200, ybins = 200;
  const double xmax = 1000, ymax = 500;
  TH2D lim("lim", "lim", xbins, 0, xmax, ybins, 0, ymax);
  lim.SetTitle("");
  lim.GetXaxis()->SetTitle("m_{#phi} (GeV)");
  lim.GetXaxis()->SetTitleOffset(1.30);
  lim.GetYaxis()->SetTitle("m_{#chi} (GeV)");
  lim.GetYaxis()->SetTitleOffset(1.47);
  lim.GetZaxis()->SetTitle("#mu^{up} = #sigma^{up}/#sigma^{th} (95% CL)  ");
  lim.GetZaxis()->SetTitleSize(0.04);
  lim.GetZaxis()->SetTitleOffset(-0.3);
  lim.GetZaxis()->SetLabelSize(0.02);
  lim.SetMaximum(1000);
  lim.SetContour(100);
  TH2* lim0 = dynamic_cast<TH2*>(lim.Clone("lim0"));
  TH2* lim1 = dynamic_cast<TH2*>(lim.Clone("lim1"));
  ifstream file("limit.txt");
  string line;
  bool header = true;
  vector<double> mPhis, mChis;
  vector<vector<double> > limits;
  unsigned int count = 0;
  while(getline(file, line)) {
    bool comment = true;
    for(string::const_iterator c = line.begin(); c != line.end(); ++c) {
      if(*c == '%') {  break; }
      if(*c != ' ' || *c != '\t') { comment = false; break; }
    }
    if(!comment) {
      istringstream ss(line);
      string ll;
      if(header) {
	bool firstBlank = true;
	while(getline(ss, ll, '\t')) {
	  if(firstBlank) { firstBlank = false; }
	  else { 
	    istringstream sl(ll);
	    double l; sl >> l;
	    mPhis.push_back(l); 
	  }
	}
	header = false;
      } else {
	bool mPhiPlace = true;
	while(getline(ss, ll, '\t')) {
	  istringstream sl(ll);
	  if(mPhiPlace) { 
	    double mChi;
	    sl >> mChi;
	    mChis.push_back(mChi);
	    limits.resize(++count);
	    mPhiPlace = false; 
	  }
	  else { 
	    double limit;
	    if(ll == " - ") { limit = 1000; } 
	    else if(sl >> limit) { } 
	    else { limit = -1; }
	    limits[count - 1].push_back(limit);
	  }
	}
      }
    }
  }
  cout << "mPhi: ";
  for(vector<double>::const_iterator i = mPhis.begin(); i != mPhis.end(); ++i) {
    cout << *i << "; "; 
  }
  cout << endl;
  for(unsigned int i = 0; i < count; ++i) {
    cout << "mChi = " << mChis[i] << "; ";
    for(unsigned int j = 0; j < limits[i].size(); ++j) {
      cout << limits[i][j] << "; ";
      lim1->Fill(mPhis[j], mChis[i], limits[i][j]);
    }
    cout << endl;
  }

  /* comment home-made extrapolation: doesn't work well
  for(unsigned int i = 0; i < xbins; ++i) {
    for(unsigned int j = 0; j < ybins; ++j) {
      double x = (double(i) + 0.5) * (xmax / xbins);
      double y = (double(j) + 0.5) * (ymax / ybins);
      double limit = 1000;
      if(x > 2*y) {
	double d1, d2, d3; d1 = d2 = d3 = 10000;
	unsigned int i1 = 0, j1 = 0, i2 = 0, j2 = 0, i3 = 0, j3 = 0;
	for(unsigned int ii = 0; ii < mPhis.size(); ++ii) {
	  for(unsigned int jj = 0; jj < mChis.size(); ++jj) {
	    if(limits[jj][ii] > 0) {
	      double x0 = mPhis[ii], y0 = mChis[jj];
	      double d = std::sqrt(std::pow(x0 - x, 2) + std::pow(y0 - y, 2));
	      if(d1 > d) {
		d3 = d2; i3 = i2; j3 = j2;
		d2 = d1; i2 = i1; j2 = j1;
		d1 = d; i1 = ii; j1 = jj;
	      } else if(d2 > d) {
		d3 = d2; i3 = i2; j3 = j2;
		d2 = d; i2 = ii; j2 = jj;
	      } else if(d3 > d) {
		d3 = d; i3 = ii; j3 = jj;
	      }
	    }
	  }
	}
	double x1 = mPhis[i1], x2 = mPhis[i2], x3 = mPhis[i3];
	double y1 = mChis[j1], y2 = mChis[j2], y3 = mChis[j3];
	double l1 = limits[j1][i1], l2 = limits[j2][i2], l3 = limits[j3][i3];
	double delta = (x2 - x1)*(y3 - y1) - (y2 - y1)*(x3 - x1);
	if(delta != 0) {
	  double a11 = y3 - y1, a12 = x1 - x3, a21 = y1 - y2, a22 = x2 - x1;
	  double v1 = x - x1, v2 = y - y1;
	  double A = (a11*v1 + a12*v2)/delta;
	  double B = (a21*v1 + a22*v2)/delta;
	  limit = l1 + A*(l2 - l1) + B*(l3 - l1);
	  if(limit > 1000) limit = 1000;
	  if(limit < 0) limit = 0;
	} else {
	  
	}
	//	limit = l1;
	//cout << "(" << x << ", " << y << ") => (" << x1 << ", " << y1 << "), (" 
	//	     << x2 << ", " << y2 << "), (" << x3 << ", " << y3 << "): ";
	//	cout << l1 << ", " << l2 << ", " << l3 << " => " << limit << endl;
      }
      lim.SetBinContent(i + 1, j + 1, limit);
    }
    //    cout << endl;
  }

  lim.Draw("colz");
  canv.SaveAs("limPhiChi.pdf");
  lim1->Draw("colz");
  canv.SaveAs("limPhiChi1.pdf");
  */

  TGraph2D gr;
  count = 0;
  for(unsigned int ii = 0; ii < mPhis.size(); ++ii) {
    for(unsigned int jj = 0; jj < mChis.size(); ++jj) {
      double x = mPhis[ii], y = mChis[jj], z = limits[jj][ii];
      gr.SetPoint(count++, x, y, z);
    }
  }
  
  gr.SetHistogram(lim0);
  gr.Draw("TRI2");
  gr.GetHistogram();
  for(unsigned int i = 0; i < xbins; ++i) {
    for(unsigned int j = 0; j < ybins; ++j) {
      double x = (double(i) + 0.5) * (xmax / xbins);
      double y = (double(j) + 0.5) * (ymax / ybins);
      if(x < 2*y) {
	lim0->SetBinContent(i + 1, j + 1, 1000);
      }
    }
  }
  lim0->SetMaximum(20);
  lim0->SetMinimum(0);
  lim0->Draw("colz");
  canv.SaveAs("limPhiChiGraph.pdf");
}
