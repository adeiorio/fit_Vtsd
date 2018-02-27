#limit(muVtb,sigmamuVtb,muVtq_95)
echo "Muon"
root -b -q 'limit.C(0.94,0.03,7.03)'
echo "Electron"
root -b -q 'limit.C(1.00,0.02,7.97)'
echo "Combined"
root -b -q 'limit.C(1.013,0.017,13.04)'
