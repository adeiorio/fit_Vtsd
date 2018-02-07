#Fit al t-channel_sd (TT_decay+ST_decay)
#analyzeSamples -D datac_electron -d metadatacards_st/ST_tch_sd_electron.txt
#combine -M MaxLikelihoodFit datac_electron/ST_tch_sd/ST_tch_sd_shape.txt --saveShapes --saveWithUncertainties --plot --robustFit 1 --customStartingPoint --out electron --autoBoundsPOIs r
##echo "Making pull distributions ..."
#./make_pulls.sh electron/mlfit.root
##cat pulls.txt
#python pulls.py -f pulls.txt --out electron
#combine -M Asymptotic -t 1 -n ST_tch_sd_ele datac_electron/ST_tch_sd/ST_tch_sd_shape.txt
#combine datac_electron/ST_tch_sd/ST_tch_sd_shape.txt -M MultiDimFit --poi r --algo=grid --points=80 --autoRange 1 -n ST_tch_sd_ele --autoBoundsPOIs r
#combine datac_electron/ST_tch_sd/ST_tch_sd_shape.txt -M MultiDimFit --poi r
#python plot1DScan.py --main higgsCombine.asimove.MultiDimFit.mH0.root --POI r -o cms_scan_asimove --pub --main-label 'Observed'

#Fit al t-channel
analyzeSamples -D datac_electron -d metadatacards_st/ST_tch_electron.txt
combine -M MaxLikelihoodFit datac_electron/ST_tch/ST_tch_shape.txt --saveShapes --saveWithUncertainties --plot --robustFit 1 --customStartingPoint --out electron --autoBoundsPOIs r
#echo "Making pull distributions ..."
#./make_pulls.sh electron/mlfit.root
#cat pulls.txt
#python pulls.py -f pulls.txt --out electron
#combine -M Asymptotic -S 1 -t 1 -n ST_tch_ele datac_electron/ST_tch/ST_tch_shape.txt
#combine datac_electron/ST_tch/ST_tch_shape.txt -M MultiDimFit --poi r --algo=grid --points=80 --autoRange 1 -n ST_tch_ele --autoBoundsPOIs "*"
#combine datac_electron/ST_tch/ST_tch_shape.txt -M MultiDimFit --poi r
#python plot1DScan.py --main higgsCombine.asimove.MultiDimFit.mH0.root --POI r -o cms_scan_asimove --pub --main-label 'Observed' 

