# -P process -i input metadacards -D output datacard -L leptoch --maxL --impacts
#python fit.py -P ST_tch -i metadatacards_st -D datac_muon -L muon --imp #--maxL
#python fit.py -P ST_tch -i metadatacards_st -D datac_ele -L electron --maxL
python fit.py -P ST_tch -i metadatacards_st -D datac_muele -L muele --maxL --imp
#python fit.py -P ST_tch_sd -i metadatacards_st -D datac_muon -L muon --asymp
#python fit.py -P ST_tch_sd -i metadatacards_st -D datac_ele -L electron --maxL
#python fit.py -P ST_tch_sd -i metadatacards_st -D datac_muele -L muele --maxL
