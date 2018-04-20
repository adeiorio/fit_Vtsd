//#include <cstdlib>
//
//using namespace std;
//void limit(Double_t mutb, Double_t sigmamutb, Double_t mutq_UL, Double_t plus_mutq_UL, Double_t minus_mutq_UL){
{
  using namespace TMath;
  //  #include "TMath.h";

  Double_t mutq_UL_obs = 5.02;
  Double_t mutq_UL = 4.70;
  Double_t mutb = 1.05;

  Double_t sigmamutb = 0.02;
  Double_t plus_mutq_UL = 7.23-mutq_UL_obs;
  Double_t minus_mutq_UL = mutq_UL_obs - 3.18;

  Double_t Vtb_exp = 0.998;
  Double_t Vtq_exp = 0.002;
  Double_t two = 2.0;
  Double_t half = 0.5;

  Double_t Vtb_obs = TMath::Power(mutb, half*half) * TMath::Power(Vtb_exp,1.);
  Double_t sigmaVtb_obs = 0.25*TMath::Power(mutb, -0.75) * sigmamutb;

  Double_t Vtb2_obs = TMath::Power(mutb, half) * TMath::Power(Vtb_exp,two);
  Double_t sigmaVtb2_obs = 0.5 * TMath::Power(mutb, -half) * sigmamutb;
  Double_t Vtq2_obstrue = TMath::Power(Vtq_exp,1.) * mutq_UL_obs/Vtb2_obs;
  Double_t Vtq2_obs = TMath::Power(Vtq_exp,1.) * mutq_UL/Vtb2_obs; // expected!!!!
  Double_t Vtq2_plus_obs = TMath::Power(Vtq_exp,1.) * plus_mutq_UL/Vtb2_obs;
  Double_t Vtq2_minus_obs = TMath::Power(Vtq_exp,1.) * minus_mutq_UL/Vtb2_obs;
  cout <<"|V_tb| = " << Vtb_obs << "+/-" << sigmaVtb_obs <<endl;
  cout <<"|V_tb|^2 = " << Vtb2_obs << "+/-" << sigmaVtb2_obs <<endl;
  cout <<"|V_td|^2+|V_tu|^2 = " << Vtq2_obs << " + " << Vtq2_plus_obs<< " - " << Vtq2_minus_obs <<endl;
  cout <<"|V_td|^2+|V_tu|^2 = " << Vtq2_obstrue <<endl;
}
