#include <cstdlib>

using namespace std;
void limit(float mutb, float sigmamutb, float mutq_UL, float plus_mutq_UL, float minus_mutq_UL){
  float mutq_UL=mutq_UL;
  float plus_mutq_UL=plus_mutq_UL;
  float minus_mutq_UL = minus_mutq_UL;
  float Vtb_exp = 0.998;
  float Vtq_exp = 0.002;
  float Vtb2_obs = pow(mutb, 0.5)*pow(Vtb_exp,2);
  float sigmaVtb2_obs = 0.5*pow(mutb, -0.5)*sigmamutb;
  float Vtq2_obs = pow(Vtq_exp,2)*mutq/Vtb2_obs;
  float Vtq2_plus_obs = pow(Vtq_exp,2)*plus_mutq/Vtb2_obs-Vtq2_obs;
  float Vtq2_minus_obs = pow(Vtq_exp,2)*minus_mutq/Vtb2_obs+Vtq2_obs;
  cout <<"|V_tb|^2 = " << Vtb2_obs << "+/-" << sigmaVtb2_obs <<endl;
  cout <<"|V_td|^2+|V_tu|^2 = " << Vtq2_obs << " + " << Vtq2_plus_obs<< " - " << Vtq2_minus_obs <<endl;
}
