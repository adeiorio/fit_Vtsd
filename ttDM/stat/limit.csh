#limit(muVtb,sigmamuVtb,muVtq_95)
echo "Muon"
root -b -q 'limit.C(0.98,0.07,11.5)'
echo "Electron"
root -b -q 'limit.C(0.97,0.08,10.8)'
echo "Combined"
root -b -q 'limit.C(0.99,0.06,18.7)'
