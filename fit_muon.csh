#analyzeSamples -D datac_muon -d metadatacards_st/muon.txt
#combine -M MaxLikelihoodFit datac_muon/ST_tch_sd/ST_tch_sd_shape.txt --saveShapes --saveWithUncertainties --robustFit 1 --customStartingPoint --plot --out muon -t -1
#combine -M MaxLikelihoodFit datac_muon/ST_tch_sd/ST_tch_sd_shape.txt --saveNormalizations --saveShapes --saveWithUncertainties --plot --out muon --rMin 0.0 --rMax 0.9 --minos=all -t -1
#combine -M MaxLikelihoodFit datac_muon/ST_tch_sd/ST_tch_sd_shape.txt --saveShapes --saveWithUncertainties --plot --out muon
#combine -M Asymptotic -t 1 -n ST_tch_sd datac_muon/ST_tch_sd/ST_tch_sd_shape.txt 
#combine -M MultiDimFit datac_muon/ST_tch_sd/ST_tch_sd_shape.txt --algo=grid --points=80 --poi r --setPhysicsModelParameterRanges r=-2,2 -n .asimove -m 0
#python plot1DScan.py --main higgsCombine.asimove.MultiDimFit.mH0.root --POI r -o cms_scan_asimove --pub --main-label 'Observed'
#display cms_scan_asimove.png &


#Fit al t-channel_sd (TT_decay ST_decay)
analyzeSamples -D datac_muon -d metadatacards_st/ST_tch_sd_muon.txt
combine -M MaxLikelihoodFit datac_muon/ST_tch_sd/ST_tch_sd_shape.txt --saveShapes --saveWithUncertainties --plot --robustFit 1 --customStartingPoint --out muon
##echo "Making pull distributions ..."
./make_pulls.sh muon/mlfit.root
cat pulls.txt
python pulls.py -f pulls.txt --out muon
combine -M Asymptotic -t 1 -n ST_tch_sd_mu datac_muon/ST_tch_sd/ST_tch_sd_shape.txt
combine datac_muon/ST_tch_sd/ST_tch_sd_shape.txt -M MultiDimFit --poi r --algo=grid --points=80 --autoRange 1 -n ST_tch_sd_muon
combine datac_muon/ST_tch_sd/ST_tch_sd_shape.txt -M MultiDimFit --poi r
python plot1DScan.py --main higgsCombine.asimove.MultiDimFit.mH0.root --POI r -o cms_scan_asimove --pub --main-label 'Observed'

#Fit al t-channel
#analyzeSamples -D datac_muon -d metadatacards_st/ST_tch_muon.txt
#combine -M MaxLikelihoodFit datac_muon/ST_tch/ST_tch_shape.txt --saveShapes --saveWithUncertainties --plot --robustFit 1 --customStartingPoint --out muon
#echo "Making pull distributions ..."
#./make_pulls.sh muon/mlfit.root
#cat pulls.txt
#python pulls.py -f pulls.txt --out muon
#combine -M Asymptotic -t 1 -n ST_tch_muon datac_muon/ST_tch/ST_tch_shape.txt
#combine datac_muon/ST_tch/ST_tch_shape.txt -M MultiDimFit --poi r --algo=grid --points=80 --autoRange 1 -n ST_tch_muon
#combine datac_muon/ST_tch/ST_tch_shape.txt -M MultiDimFit --poi r
#python plot1DScan.py --main higgsCombine.asimove.MultiDimFit.mH0.root --POI r -o cms_scan_asimove --pub --main-label 'Observed' 
