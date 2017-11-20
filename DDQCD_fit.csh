#Fitting DDQCD flat prior
analyzeSamples -D datac_QCD_cr -d metadatacards_st/QCD_2j1t_cr_mu.txt
combine -M MaxLikelihoodFit datac_QCD_cr/DDQCD/DDQCD_muon_2j1t_shape.txt --saveShapes --saveWithUncertainties --plot --robustFit 1 --customStartingPoint --out QCDmu_2j_cr >& QCDmu_2j_cr/QCD_2j1t_cr_mu.log 
tail QCDmu_2j_cr/QCD_2j1t_cr_mu.log

analyzeSamples -D datac_QCD_sr -d metadatacards_st/QCD_2j1t_sr_mu.txt
combine -M MaxLikelihoodFit datac_QCD_sr/DDQCD/DDQCD_muon_2j1t_shape.txt --saveShapes --saveWithUncertainties --plot --robustFit 1 --customStartingPoint --out QCDmu_2j_sr >& QCDmu_2j_sr/QCD_2j1t_sr_mu.log 
tail QCDmu_2j_sr/QCD_2j1t_sr_mu.log

analyzeSamples -D datac_QCD_cr -d metadatacards_st/QCD_3j1t_cr_mu.txt
combine -M MaxLikelihoodFit datac_QCD_cr/DDQCD/DDQCD_muon_3j1t_shape.txt --saveShapes --saveWithUncertainties --plot --robustFit 1 --customStartingPoint --out QCDmu_3j_cr >& QCDmu_3j_cr/QCD_3j1t_cr_mu.log 
tail QCDmu_3j_cr/QCD_3j1t_cr_mu.log

analyzeSamples -D datac_QCD_sr -d metadatacards_st/QCD_3j1t_sr_mu.txt
combine -M MaxLikelihoodFit datac_QCD_sr/DDQCD/DDQCD_muon_3j1t_shape.txt --saveShapes --saveWithUncertainties --plot --robustFit 1 --customStartingPoint --out QCDmu_3j_sr >& QCDmu_3j_sr/QCD_3j1t_sr_mu.log 
tail QCDmu_3j_sr/QCD_3j1t_sr_mu.log

analyzeSamples -D datac_QCD_cr -d metadatacards_st/QCD_2j1t_cr_ele.txt
combine -M MaxLikelihoodFit datac_QCD_cr/DDQCD/DDQCD_electron_2j1t_shape.txt --saveShapes --saveWithUncertainties --plot --robustFit 1 --customStartingPoint --out QCDele_2j_cr >& QCDele_2j_cr/QCD_2j1t_cr_ele.log 
tail QCDele_2j_cr/QCD_2j1t_cr_ele.log

analyzeSamples -D datac_QCD_sr -d metadatacards_st/QCD_2j1t_sr_ele.txt
combine -M MaxLikelihoodFit datac_QCD_sr/DDQCD/DDQCD_electron_2j1t_shape.txt --saveShapes --saveWithUncertainties --plot --robustFit 1 --customStartingPoint --out QCDele_2j_sr >& QCDele_2j_sr/QCD_2j1t_sr_ele.log 
tail QCDele_2j_sr/QCD_2j1t_sr_ele.log

analyzeSamples -D datac_QCD_cr -d metadatacards_st/QCD_3j1t_cr_ele.txt
combine -M MaxLikelihoodFit datac_QCD_cr/DDQCD/DDQCD_electron_3j1t_shape.txt --saveShapes --saveWithUncertainties --plot --robustFit 1 --customStartingPoint --out QCDele_3j_cr >& QCDele_3j_cr/QCD_3j1t_cr_ele.log 
tail QCDele_3j_cr/QCD_3j1t_cr_ele.log

analyzeSamples -D datac_QCD_sr -d metadatacards_st/QCD_3j1t_sr_ele.txt
combine -M MaxLikelihoodFit datac_QCD_sr/DDQCD/DDQCD_electron_3j1t_shape.txt --saveShapes --saveWithUncertainties --plot --robustFit 1 --customStartingPoint --out QCDele_3j_sr >& QCDele_3j_sr/QCD_3j1t_sr_ele.log 
tail QCDele_3j_sr/QCD_3j1t_sr_ele.log
