#!/bin/bash


#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#   download FASTA
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
for i in {1..28} {30..33} LGE64 MT W Z; do
    wget $CHICKEN_URL/$CHICKEN_CHROMOSOME$i.$FTYPE.$COMP_FTYPE;
    gunzip < $CHICKEN_CHROMOSOME$i.$FTYPE.$COMP_FTYPE \
     > $FLD_chromosomes/$CHICKEN$i.$FTYPE;
    rm $CHICKEN_CHROMOSOME$i.$FTYPE.$COMP_FTYPE
done

for i in unlocalized unplaced; do
    wget $CHICKEN_URL/$CHICKEN_CHR_PREFIX$i.$FTYPE.$COMP_FTYPE;
    gunzip < $CHICKEN_CHR_PREFIX$i.$FTYPE.$COMP_FTYPE \
     > $FLD_chromosomes/$CHICKEN$i.$FTYPE;
    rm $CHICKEN_CHR_PREFIX$i.$FTYPE.$COMP_FTYPE
done


#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#   rename: GGALGE64 -> GGALG, GGAunlocalized -> GGAUL, GGAunplaced -> GGAUP
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
mv $FLD_chromosomes/$CHICKEN"LGE64".$FTYPE \
    $FLD_chromosomes/$CHICKEN"LG".$FTYPE
mv $FLD_chromosomes/$CHICKEN"unlocalized".$FTYPE \
    $FLD_chromosomes/$CHICKEN"UL".$FTYPE
mv $FLD_chromosomes/$CHICKEN"unplaced".$FTYPE \
    $FLD_chromosomes/$CHICKEN"UP".$FTYPE


#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#   FASTA -> SEQ
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
for i in $GGA_SEQ_RUN; do
    cat $FLD_chromosomes/$CHICKEN$i.$FTYPE \
        | grep -v -e "ERROR" -e "eFetchResult" -e "DOCTYPE" -e "xml version" \
                  -e "Unable to obtain" | grep -v ">" \
     > $FLD_dataset/$CHICKEN$i
done
