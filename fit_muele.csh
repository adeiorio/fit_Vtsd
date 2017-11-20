#Fit al t-channel_sd
#analyzeSamples -D datac_muele -d metadatacards_st/ST_tch_sd_mu_ele.txt 
#combine -M MaxLikelihoodFit datac_muele/ST_tch_sd/ST_tch_sd_shape.txt --saveShapes --saveWithUncertainties --plot --robustFit 1 --customStartingPoint --out mu_ele --saveNLL --keepFailures
#echo "Making pull distributions ..."
#./make_pulls.sh mu_ele/mlfit.root
#cat pulls.txt
#python pulls.py -f pulls.txt --out mu_ele
#combine -M Asymptotic -S 1 -t 1 -n ST_tch_sd_muele datac_muele/ST_tch_sd/ST_tch_sd_shape.txt
#combine datac_muele/ST_tch_sd/ST_tch_sd_shape.txt -M MultiDimFit --poi r --algo=grid --points=80 --autoRange 1 -n ST_tch_sd_muele
#combine datac_muele/ST_tch_sd/ST_tch_sd_shape.txt -M MultiDimFit --poi r 
#python plot1DScan.py --main higgsCombine.asimove.MultiDimFit.mH0.root --POI r -o cms_scan_asimove --pub --main-label 'Observed'

#Fit al t-channel 
analyzeSamples -D datac_muele -d metadatacards_st/ST_tch_mu_ele.txt 
combine -M MaxLikelihoodFit datac_muele/ST_tch/ST_tch_shape.txt --saveShapes --saveWithUncertainties --plot --robustFit 1 --customStartingPoint --out mu_ele
echo "Making pull distributions ..."
./make_pulls.sh mu_ele/mlfit.root
#cat pulls.txt
python pulls.py -f pulls.txt --out mu_ele
combine -M Asymptotic -S 1 -t 1 -n ST_tch_sd datac_muele/ST_tch/ST_tch_shape.txt
combine datac_muele/ST_tch/ST_tch_shape.txt -M MultiDimFit --poi r --algo=grid --points=80 --autoRange 1 -n ST_tch_muele
#python plot1DScan.py --main higgsCombineTest.MultiDimFit.mH120.root --POI r -o cms_scan_asimove --pub --main-label 'Observed'
