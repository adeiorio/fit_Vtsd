# -P process -i input metadacards -D output datacard -L leptoch --maxL --impacts
#echo "================================================================ Muon fit ==========================================================================================="
#python fit.py -P ST_tch -i metadatacards_st -D datac_muon -L muon --multi --maxL #--imp
#echo "================================================================ Electron fit ======================================================================================="
#python fit.py -P ST_tch -i metadatacards_st -D datac_electron -L electron  --maxL #--imp --multi
#echo "================================================================ Combined fit ======================================================================================="
#python fit.py -P ST_tch -i metadatacards_st -D datac_muele -L muele --maxL #--imp  --multi
echo "================================================================ Muon fit ==========================================================================================="
python fit.py -P ST_tch_sd -i metadatacards_st -D datac_muon -L muon --asymp  #--imp
echo "================================================================ Electron fit ======================================================================================="
python fit.py -P ST_tch_sd -i metadatacards_st -D datac_electron -L electron  --asymp  #--imp --maxL
echo "================================================================ Combined fit ======================================================================================="
python fit.py -P ST_tch_sd -i metadatacards_st -D datac_muele -L muele --asymp  # --imp --maxL
