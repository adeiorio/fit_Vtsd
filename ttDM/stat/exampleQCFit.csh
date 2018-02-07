analyzeSamples -D myOutputDirectory -d metadatacards_st/data_st_mtwfit_muon.txt
combine -M MaxLikelihoodFit myOutputDirectory/QCDMuEPt20toInf/QCDMuEPt20toInf_shape.txt --saveShapes --saveWithUncertainties --plot --out mtw
combine -M MaxLikelihoodFit myOutputDirectory/QCDMuEPt20toInf/QCDMuEPt20toInf_shape.txt --saveShapes --saveWithUncertainties --plot --out mtw --skipBOnlyFit
#combine -M MaxLikelihoodFit testingst/QCDMuEPt20toInf/QCDMuEPt20toInf_shape.txt --saveShapes --saveWithUncertainties --plots
