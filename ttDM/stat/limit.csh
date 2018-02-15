#limit(muVtb,sigmamuVtb,muVtq_95)
echo "Muon"
root -b -q 'limit.C(0.94,0.04,53.35)'
echo "Electron"
root -b -q 'limit.C(0.91,0.04,40.57)'
