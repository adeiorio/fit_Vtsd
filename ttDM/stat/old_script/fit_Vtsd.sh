analyzeSamples -D Vtsd -d Vtsd/Vtsd.txt
#combine -M MaxLikelihoodFit Vtsd/ST_tch/ST_tch_shape.txt --saveShapes --saveWithUncertainties --plot --out Vtsd --skipBOnlyFit
echo "Setting the signal strength to zero -t -1"
combine -M MaxLikelihoodFit Vtsd/ST_tch_sd/ST_tch_sd_shape.txt --saveNormalizations --saveShapes --saveWithUncertainties --plot --out Vtsd --rMin 0.0 --rMax 0.9 --minos=all -t -1

combine Vtsd/ST_tch_sd/ST_tch_sd_shape.txt -M MultiDimFit --algo=grid --points=80 --poi r --setPhysicsModelParameterRanges r=-2,2 -n .asimove -m 0
python plot1DScan.py --main higgsCombine.asimove.MultiDimFit.mH0.root --POI r -o cms_scan_asimove --pub --main-label 'Observed'
display cms_scan_asimove.png &

echo "Making pull distributions ..."
./make_pulls.sh Vtsd/mlfit.root
cat pulls.txt
python pulls.py -f pulls.txt


echo "Setting the signal strength to float"
combine -M MaxLikelihoodFit Vtsd/ST_tch_sd/ST_tch_sd_shape.txt --saveNormalizations --saveShapes --saveWithUncertainties --plot --out Vtsd --rMin 0.0 --rMax 0.9 --minos=all 

echo "Cross Checks for the JES"
combine Vtsd/ST_tch_sd/ST_tch_sd_shape.txt -M MultiDimFit --algo=grid --points=80 --poi jes --setPhysicsModelParameterRanges jes=-2,2 -n .scanjes -m 0
python plot1DScan.py --main higgsCombine.scanjes.MultiDimFit.mH0.root --POI jes -o cms_scan_jes --pub --main-label 'Observed'
display cms_scan_jes.png &


echo "Cross Checks for the lumi"
combine Vtsd/ST_tch_sd/ST_tch_sd_shape.txt -M MultiDimFit --algo=grid --points=80 --poi lumi --setPhysicsModelParameterRanges lumi=-2,2 -n .scanlumi -m 0
python plot1DScan.py --main higgsCombine.scanlumi.MultiDimFit.mH0.root --POI lumi -o cms_scan_lumi --pub --main-label 'Observed'
display cms_scan_lumi.png &
