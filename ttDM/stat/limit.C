#include <cstdlib>

using namespace TMath;
void limit(Double_t muVtb, Double_t sigmamuVtb, Double_t muVtq_95){
  Double_t Vtb_exp = TMath::Power(0.998,.5);
  Double_t Vtq2_exp = 0.002;
  Double_t Vtb_obs = TMath::Power(muVtb, 0.25)*Vtb_exp;
  Double_t sigmaVtb_obs = 0.25 * TMath::Power(muVtb, -0.75) * sigmamuVtb;
  Double_t Vtb2_obs = TMath::Power(muVtb, .5) * TMath::Power(Vtb_exp,2.);
  Double_t sigmaVtb2_obs = 0.5 * TMath::Power(muVtb, -.5) * sigmamuVtb;
  Double_t Vtq2_obstrue = Vtq2_exp * muVtq_95 * 0.998/Vtb2_obs;
  cout <<"|V_tb| = " << Vtb_obs << "+/-" << sigmaVtb_obs <<endl;
  cout <<"|V_tb|^2 = " << Vtb2_obs << "+/-" << sigmaVtb2_obs <<endl;
  cout <<"|V_td|^2+|V_tu|^2 = " << Vtq2_obstrue <<endl;
}
