#!/bin/bash

#***********************************************************
#   change directory to home
#***********************************************************
cd ..
cmake src
make


#***********************************************************
#   parameters to install and run needed programs
#***********************************************************
###*** folders to be used
FLD_archive_dat="archive_dat"
FLD_archive_datasets="archive_datasets"
FLD_chromosomes="chromosomes"
FLD_dat="dat"
FLD_datasets="datasets"
FLD_goose="goose"
FLD_GULL="GULL"
FLD_script="script"
FLD_XS="XS"

GET_HUMAN=0             # download Human choromosomes and make SEQ out of FASTA
GET_CHIMPANZEE=0        # download Chimpanzee choromosomes and make SEQ out of FASTA
GET_GORILLA=0           # download Gorilla choromosomes and make SEQ out of FASTA
GET_CHICKEN=0           # download Chicken choromosomes and make SEQ out of FASTA
GET_TURKEY=0            # download Turkey choromosomes and make SEQ out of FASTA
INSTALL_XS=0            # install "XS" from Github
INSTALL_goose=0         # install "goose" from Github
INSTALL_GULL=0          # install "GULL" from Github
GEN_DATASETS=0          # generate datasets using "XS"
GEN_MUTATIONS=0         # generate mutations using "goose"
GEN_ARCHAEA=0           # generate archea dataset using "goose" -- output: out#.fa
RUN=0                   # run the program
PLOT_RESULTS=0          # plot results using "gnuplot"
BUILD_MATRIX=0          # build matrix from datasets
PLOT_MATRIX=1           # plot matrix from datasets
ARCHIVE_DATA=0          # archive data

# mutations list:   `seq -s' ' 1 10`
#MUT_LIST="1 2 3 4 5 6 7 8 9 10 12 14 16 18 20 25 30 35 40 45 50"

HUMAN_URL="ftp://ftp.ncbi.nlm.nih.gov/genomes/H_sapiens/Assembled_chromosomes/seq"
CHIMPANZEE_URL="ftp://ftp.ncbi.nlm.nih.gov/genomes/Pan_troglodytes/Assembled_chromosomes/seq"
GORILLA_URL="ftp://ftp.ncbi.nlm.nih.gov/genomes/Gorilla_gorilla/Assembled_chromosomes/seq"
CHICKEN_URL="ftp://ftp.ncbi.nlm.nih.gov/genomes/Gallus_gallus/Assembled_chromosomes/seq"
TURKEY_URL="ftp://ftp.ncbi.nlm.nih.gov/genomes/Meleagris_gallopavo/Assembled_chromosomes/seq"

HUMAN_CHR_PREFIX="hs_ref_GRCh38.p7_"
CHIMPANZEE_CHR_PREFIX="ptr_ref_Pan_tro_3.0_"
GORILLA_CHR_PREFIX="9595_ref_gorGor4_"
CHICKEN_CHR_PREFIX="gga_ref_Gallus_gallus-5.0_"
TURKEY_CHR_PREFIX="mga_ref_Turkey_5.0_"

CHR="chr"

HUMAN_CHR="HS"
CHIMPANZEE_CHR="PT"
GORILLA_CHR="GG"
ARCHAEA_CHR="A"
CHICKEN_CHR="GGA"
TURKEY_CHR="MGA"

CURR_CHR="21"
chromosomes="$HUMAN_CHR_PREFIX$CHR$CURR_CHR"

HUMAN_CHROMOSOME="$HUMAN_CHR_PREFIX$CHR"
CHIMPANZEE_CHROMOSOME="$CHIMPANZEE_CHR_PREFIX$CHR"
GORILLA_CHROMOSOME="$GORILLA_CHR_PREFIX$CHR"
CHICKEN_CHROMOSOME="$CHICKEN_CHR_PREFIX$CHR"
TURKEY_CHROMOSOME="$TURKEY_CHR_PREFIX$CHR"

HS_SEQ_RUN=`seq -s' ' 1 22`; HS_SEQ_RUN+=" X Y MT AL UL UP"
PT_SEQ_RUN="1 2A 2B "; PT_SEQ_RUN+=`seq -s' ' 3 22`; PT_SEQ_RUN+=" X Y MT UL UP"
GG_SEQ_RUN="1 2A 2B "; GG_SEQ_RUN+=`seq -s' ' 3 22`; GG_SEQ_RUN+=" X MT UL UP"
A_SEQ_RUN=`seq -s' ' 1 206`
GGA_SEQ_RUN=`seq -s' ' 1 28`; GGA_SEQ_RUN+=" "; GGA_SEQ_RUN+=`seq -s' ' 30 33`; GGA_SEQ_RUN+=" LG MT W Z UL UP"
MGA_SEQ_RUN=`seq -s' ' 1 30`; MGA_SEQ_RUN+=" MT W Z UL UP"

datasets="$HUMAN_CHR$CURR_CHR"
#datasets="";   for i in $HS_SEQ_RUN; do datasets+=$HUMAN_CHR${i}" "; done

### reference parameters
#REF_SPECIE=$HUMAN_CHR
#REF_SPECIE=$CHIMPANZEE_CHR
#REF_SPECIE=$GORILLA_CHR
#REF_SPECIE=$CHICKEN_CHR
#REF_SPECIE=$TURKEY_CHR
REF_SPECIE=$ARCHAEA_CHR
#
tempRefSeqRun=${REF_SPECIE}_SEQ_RUN
REF_SEQ_RUN=${!tempRefSeqRun}     # all chromosomes for that specie, e.g. HS_SEQ_RUN
REF_DATASET="";  for i in 24; do REF_DATASET+=$REF_SPECIE${i}" "; done
#REF_DATASET="";  for i in $REF_SEQ_RUN; do REF_DATASET+=$REF_SPECIE${i}" "; done

###*** target parameters
#TAR_SPECIE=$HUMAN_CHR
#TAR_SPECIE=$CHIMPANZEE_CHR
#TAR_SPECIE=$GORILLA_CHR
#TAR_SPECIE=$CHICKEN_CHR
#TAR_SPECIE=$TURKEY_CHR
TAR_SPECIE=$ARCHAEA_CHR
#
tempTarSeqRun=${TAR_SPECIE}_SEQ_RUN
TAR_SEQ_RUN=${!tempTarSeqRun}     # all chromosomes for that specie, e.g. HS_SEQ_RUN
TAR_DATASET="";  for i in $TAR_SEQ_RUN; do TAR_DATASET+=$TAR_SPECIE${i}" "; done


FILE_TYPE="fa"          # file type
COMP_FILE_TYPE="gz"     # compressed file type
INF_FILE_TYPE="dat"     # information (data) file type
#INF_FILE_TYPE="csv"     # information (data) file type

PIX_FORMAT=pdf          # output format: pdf, png, svg, eps, epslatex (set output x.y)
IR_LBL=i                # label for inverted repeat
a_LBL=a                 # label for alpha denominator

INV_REPEATS="0 1"         # list of inverted repeats
ALPHA_DENS="100"        # list of alpha denominators
MIN_CTX=20              # min context-order size
MAX_CTX=20              # max context-order size


#***********************************************************
#   download Human choromosomes and make SEQ out of FASTA
#***********************************************************
if [[ $GET_HUMAN == 1 ]]; then

###*** download FASTA
for i in {1..22} X Y MT; do
 wget $HUMAN_URL/$HUMAN_CHROMOSOME$i.$FILE_TYPE.$COMP_FILE_TYPE;
 gunzip < $HUMAN_CHROMOSOME$i.$FILE_TYPE.$COMP_FILE_TYPE > $FLD_chromosomes/$HUMAN_CHR$i.$FILE_TYPE;
 rm $HUMAN_CHROMOSOME$i.$FILE_TYPE.$COMP_FILE_TYPE
done

for i in alts unlocalized unplaced; do
 wget $HUMAN_URL/$HUMAN_CHR_PREFIX$i.$FILE_TYPE.$COMP_FILE_TYPE;
 gunzip < $HUMAN_CHR_PREFIX$i.$FILE_TYPE.$COMP_FILE_TYPE > $FLD_chromosomes/$HUMAN_CHR$i.$FILE_TYPE;
 rm $HUMAN_CHR_PREFIX$i.$FILE_TYPE.$COMP_FILE_TYPE
done

###*** rename: HSalts -> HSAL, HSunlocalized -> HSUL, HSunplaced -> HSUP
mv $FLD_chromosomes/$HUMAN_CHR"alts".$FILE_TYPE $FLD_chromosomes/$HUMAN_CHR"AL".$FILE_TYPE
mv $FLD_chromosomes/$HUMAN_CHR"unlocalized".$FILE_TYPE $FLD_chromosomes/$HUMAN_CHR"UL".$FILE_TYPE
mv $FLD_chromosomes/$HUMAN_CHR"unplaced".$FILE_TYPE $FLD_chromosomes/$HUMAN_CHR"UP".$FILE_TYPE

###*** FASTA -> SEQ
for i in $HS_SEQ_RUN; do grep -v ">" $FLD_chromosomes/$HUMAN_CHR$i.$FILE_TYPE > $FLD_datasets/$HUMAN_CHR$i; done

fi  # end of $GET_HUMAN


#***********************************************************
#   download Chimpanzee choromosomes and make SEQ out of FASTA
#***********************************************************
if [[ $GET_CHIMPANZEE == 1 ]]; then

###*** download FASTA
for i in 1 2A 2B {3..22} X Y MT; do
 wget $CHIMPANZEE_URL/$CHIMPANZEE_CHROMOSOME$i.$FILE_TYPE.$COMP_FILE_TYPE;
 gunzip < $CHIMPANZEE_CHROMOSOME$i.$FILE_TYPE.$COMP_FILE_TYPE > $FLD_chromosomes/$CHIMPANZEE_CHR$i.$FILE_TYPE;
 rm $CHIMPANZEE_CHROMOSOME$i.$FILE_TYPE.$COMP_FILE_TYPE
done

for i in unlocalized unplaced; do
 wget $CHIMPANZEE_URL/$CHIMPANZEE_CHR_PREFIX$i.$FILE_TYPE.$COMP_FILE_TYPE;
 gunzip < $CHIMPANZEE_CHR_PREFIX$i.$FILE_TYPE.$COMP_FILE_TYPE > $FLD_chromosomes/$CHIMPANZEE_CHR$i.$FILE_TYPE;
 rm $CHIMPANZEE_CHR_PREFIX$i.$FILE_TYPE.$COMP_FILE_TYPE
done

###*** rename: PTunlocalized -> PTUL, PTunplaced -> PTUP
mv $FLD_chromosomes/$CHIMPANZEE_CHR"unlocalized".$FILE_TYPE $FLD_chromosomes/$CHIMPANZEE_CHR"UL".$FILE_TYPE
mv $FLD_chromosomes/$CHIMPANZEE_CHR"unplaced".$FILE_TYPE $FLD_chromosomes/$CHIMPANZEE_CHR"UP".$FILE_TYPE

###*** FASTA -> SEQ
for i in $PT_SEQ_RUN; do grep -v ">" $FLD_chromosomes/$CHIMPANZEE_CHR$i.$FILE_TYPE > $FLD_datasets/$CHIMPANZEE_CHR$i; done

fi  # end of $GET_CHIMPANZEE


#***********************************************************
#   download Gorilla choromosomes and make SEQ out of FASTA
#***********************************************************
if [[ $GET_GORILLA == 1 ]]; then

###*** download FASTA
for i in 1 2A 2B {3..22} X MT; do
 wget $GORILLA_URL/$GORILLA_CHROMOSOME$i.$FILE_TYPE.$COMP_FILE_TYPE;
 gunzip < $GORILLA_CHROMOSOME$i.$FILE_TYPE.$COMP_FILE_TYPE > $FLD_chromosomes/$GORILLA_CHR$i.$FILE_TYPE;
 rm $GORILLA_CHROMOSOME$i.$FILE_TYPE.$COMP_FILE_TYPE
done

for i in unlocalized unplaced; do
 wget $GORILLA_URL/$GORILLA_CHR_PREFIX$i.$FILE_TYPE.$COMP_FILE_TYPE;
 gunzip < $GORILLA_CHR_PREFIX$i.$FILE_TYPE.$COMP_FILE_TYPE > $FLD_chromosomes/$GORILLA_CHR$i.$FILE_TYPE;
 rm $GORILLA_CHR_PREFIX$i.$FILE_TYPE.$COMP_FILE_TYPE
done

###*** rename: GGunlocalized -> GGUL, GGunplaced -> GGUP
mv $FLD_chromosomes/$GORILLA_CHR"unlocalized".$FILE_TYPE $FLD_chromosomes/$GORILLA_CHR"UL".$FILE_TYPE
mv $FLD_chromosomes/$GORILLA_CHR"unplaced".$FILE_TYPE $FLD_chromosomes/$GORILLA_CHR"UP".$FILE_TYPE

###*** FASTA -> SEQ
for i in $GG_SEQ_RUN; do grep -v ">" $FLD_chromosomes/$GORILLA_CHR$i.$FILE_TYPE > $FLD_datasets/$GORILLA_CHR$i; done

fi  # end of $GET_GORILLA


#***********************************************************
#   download Chicken choromosomes and make SEQ out of FASTA
#***********************************************************
if [[ $GET_CHICKEN == 1 ]]; then

###*** download FASTA
for i in {1..28} {30..33} LGE64 MT W Z; do
 wget $CHICKEN_URL/$CHICKEN_CHROMOSOME$i.$FILE_TYPE.$COMP_FILE_TYPE;
 gunzip < $CHICKEN_CHROMOSOME$i.$FILE_TYPE.$COMP_FILE_TYPE > $FLD_chromosomes/$CHICKEN_CHR$i.$FILE_TYPE;
 rm $CHICKEN_CHROMOSOME$i.$FILE_TYPE.$COMP_FILE_TYPE
done

for i in unlocalized unplaced; do
 wget $CHICKEN_URL/$CHICKEN_CHR_PREFIX$i.$FILE_TYPE.$COMP_FILE_TYPE;
 gunzip < $CHICKEN_CHR_PREFIX$i.$FILE_TYPE.$COMP_FILE_TYPE > $FLD_chromosomes/$CHICKEN_CHR$i.$FILE_TYPE;
 rm $CHICKEN_CHR_PREFIX$i.$FILE_TYPE.$COMP_FILE_TYPE
done

###*** rename: GGALGE64 -> GGALG, GGAunlocalized -> GGAUL, GGAunplaced -> GGAUP
mv $FLD_chromosomes/$CHICKEN_CHR"LGE64".$FILE_TYPE $FLD_chromosomes/$CHICKEN_CHR"LG".$FILE_TYPE
mv $FLD_chromosomes/$CHICKEN_CHR"unlocalized".$FILE_TYPE $FLD_chromosomes/$CHICKEN_CHR"UL".$FILE_TYPE
mv $FLD_chromosomes/$CHICKEN_CHR"unplaced".$FILE_TYPE $FLD_chromosomes/$CHICKEN_CHR"UP".$FILE_TYPE

###*** FASTA -> SEQ
for i in $GGA_SEQ_RUN; do grep -v ">" $FLD_chromosomes/$CHICKEN_CHR$i.$FILE_TYPE > $FLD_datasets/$CHICKEN_CHR$i; done

fi  # end of $GET_CHICKEN


#***********************************************************
#   download Turkey choromosomes and make SEQ out of FASTA
#***********************************************************
if [[ $GET_TURKEY == 1 ]]; then

###*** download FASTA
for i in {1..30} MT W Z; do
 wget $TURKEY_URL/$TURKEY_CHROMOSOME$i.$FILE_TYPE.$COMP_FILE_TYPE;
 gunzip < $TURKEY_CHROMOSOME$i.$FILE_TYPE.$COMP_FILE_TYPE > $FLD_chromosomes/$TURKEY_CHR$i.$FILE_TYPE;
 rm $TURKEY_CHROMOSOME$i.$FILE_TYPE.$COMP_FILE_TYPE
done

for i in unlocalized unplaced; do
 wget $TURKEY_URL/$TURKEY_CHR_PREFIX$i.$FILE_TYPE.$COMP_FILE_TYPE;
 gunzip < $TURKEY_CHR_PREFIX$i.$FILE_TYPE.$COMP_FILE_TYPE > $FLD_chromosomes/$TURKEY_CHR$i.$FILE_TYPE;
 rm $TURKEY_CHR_PREFIX$i.$FILE_TYPE.$COMP_FILE_TYPE
done

###*** rename: MGAunlocalized -> MGAUL, MGAunplaced -> MGAUP
mv $FLD_chromosomes/$TURKEY_CHR"unlocalized".$FILE_TYPE $FLD_chromosomes/$TURKEY_CHR"UL".$FILE_TYPE
mv $FLD_chromosomes/$TURKEY_CHR"unplaced".$FILE_TYPE $FLD_chromosomes/$TURKEY_CHR"UP".$FILE_TYPE

###*** FASTA -> SEQ
for i in $MGA_SEQ_RUN; do grep -v ">" $FLD_chromosomes/$TURKEY_CHR$i.$FILE_TYPE > $FLD_datasets/$TURKEY_CHR$i; done

fi  # end of $GET_TURKEY


#***********************************************************
#   install "XS" from Github
#***********************************************************
if [[ $INSTALL_XS == 1 ]]; then

rm -fr XS
git clone https://github.com/pratas/XS.git
cd $FLD_XS
make
cd ..

fi  # end of $INSTALL_XS


#***********************************************************
#   install "goose" from Github
#***********************************************************
if [[ $INSTALL_goose == 1 ]]; then

rm -fr goose
git clone https://github.com/pratas/goose.git
cd ${FLD_goose}/src
make
cd ../../

fi  # end of $INSTALL_goose


#***********************************************************
#   install "GULL" from Github
#***********************************************************
if [[ $INSTALL_GULL == 1 ]]; then

rm -fr GULL
git clone https://github.com/pratas/GULL.git
cd ${FLD_GULL}/src
cmake .
make
cd ../../

fi  # end of $INSTALL_GULL


#***********************************************************
#   generate datasets using "XS"
#***********************************************************
if [[ $GEN_DATASETS == 1 ]]; then

XS/XS -ls 100 -n 100000 -rn 0 -f 0.20,0.20,0.20,0.20,0.20 -eh -eo -es datasetXS
echo ">X" > HEADER      # add ">X" as the header of the sequence (build "nonRepX")
cat HEADER datasetXS > dataset
rm -f HEADER

fi  # end of $GEN_DATASETS


#***********************************************************
#   generate mutations using "goose"
#***********************************************************
if [[ $GEN_MUTATIONS == 1 ]]; then

for c in $chromosomes; do
 for x in $MUT_LIST; do      #((x=1; x<$NUM_MUTATIONS; x+=1));
 MRATE=`echo "scale=3;$x/100" | bc -l`;      # handle transition 0.09 -> 0.10
 goose/src/goose-mutatefasta -s $x -a5 -mr $MRATE " " < $FLD_chromosomes/$c.$FILE_TYPE > temp;
 cat temp | grep -v ">" > $HUMAN_CHR${CURR_CHR}_$x      # remove the header line
 done
done
rm -f temp*    # remove temporary files

#-----------------------------------
#   move all generated mutations files to "datasets" folder
#-----------------------------------
##rm -fr $FLD_datasets
#mkdir -p $FLD_datasets
mv ${HUMAN_CHR}* $FLD_datasets

fi  # end of $GEN_MUTATIONS


#***********************************************************
#   generate archaea dataset using "goose" -- output: out#.fa
#***********************************************************
if [[ $GEN_ARCHAEA == 1 ]]; then

$FLD_goose/src/goose-splitreads < "$FLD_archive_datasets/DB.mfa"

for i in {1..206}; do
 grep -v ">" out$i.$FILE_TYPE > $FLD_datasets/A$i;
done

rm -f out*.fa    # remove fa files generated by goose

fi  # end of $GEN_ARCHAEA


#***********************************************************
#   running the program
#***********************************************************
if [[ $RUN == 1 ]]; then

for ir in $INV_REPEATS; do
 for alphaDen in $ALPHA_DENS; do
  for refDataset in $REF_DATASET; do
   echo -e "ref\ttar\tir\talpha\tctx\tbpb\tNRC\ttime(s)" \
        >> $IR_LBL$ir-$refDataset-$TAR_SPECIE.$INF_FILE_TYPE
   for tarDataset in $TAR_DATASET; do
##   rm -f $IR_LBL$ir-$a_LBL$alphaDen-${dataset}.$INF_FILE_TYPE
#   touch $IR_LBL$ir-$a_LBL$alphaDen-$dataset.$INF_FILE_TYPE
#   echo -e "mut\tmin_bpb\tmin_ctx" >> $IR_LBL$ir-$a_LBL$alphaDen-$dataset.$INF_FILE_TYPE
#    for mut in $MUT_LIST; do
#    rm -f $IR_LBL$ir-$a_LBL$alphaDen-${dataset}_$mut.$INF_FILE_TYPE
#    touch $IR_LBL$ir-$a_LBL$alphaDen-${dataset}_$mut.$INF_FILE_TYPE
#    echo -e "# ir\talpha\tctx\tbpb\ttime(s)" >> $IR_LBL$ir-$a_LBL$alphaDen-${dataset}_$mut.$INF_FILE_TYPE
     for((ctx=$MIN_CTX; ctx<=$MAX_CTX; ctx+=1)); do
#     for ctx in {2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20}; do
     ./phoenix -m r,$ctx,$alphaDen,$ir -t $FLD_datasets/$tarDataset -r $FLD_datasets/$refDataset \
               >> $IR_LBL$ir-$refDataset-$TAR_SPECIE.$INF_FILE_TYPE
     done
#    done
   done
##    # save "min bpb" and "min ctx" for each dataset
#    minBpbCtx=$(awk 'NR==1 || $4 < minBpb {minBpb=$4; minCtx=$3}; \
#                END {print minBpb"\t"minCtx}' $IR_LBL$ir-$a_LBL$alphaDen-${dataset}_$mut.$INF_FILE_TYPE)
#    echo -e "$mut\t$minBpbCtx" >> $IR_LBL$ir-$a_LBL$alphaDen-$dataset.$INF_FILE_TYPE
#   done
  done
 done
done

#-----------------------------------
#   create "dat" folder and save the results
#-----------------------------------
###rm -fr $FLD_dat              # remove "dat" folder, if it already exists
##mkdir -p $FLD_dat             # make "dat" folder
#mv ${IR_LBL}*.$INF_FILE_TYPE $FLD_dat    # move all created dat files to the "dat" folder

fi  # end of $RUN


#***********************************************************
#   plot results using "gnuplot"
#***********************************************************
if [[ $PLOT_RESULTS == 1 ]]; then

for ir in $INV_REPEATS; do
 for alphaDen in $ALPHA_DENS; do
#  for dataset in $datasets; do
#   for mut in $MUT_LIST; do

gnuplot <<- EOF
set term $PIX_FORMAT        # set terminal for output picture format


##########################    bpb    ##########################
#set xlabel "% mutation"                 # set label of x axis
#set ylabel "bpb"                        # set label of y axis
##set xtics 0,5,100                      # set steps for x axis
#set xtics add ("1" 1)
#set key bottom right                    # legend position
##set output "$IR_LBL$ir-$a_LBL$alphaDen-$dataset-bpb.$PIX_FORMAT"       # set output name
##plot "$FLD_dat/$IR_LBL$ir-$a_LBL$alphaDen-${dataset}.$INF_FILE_TYPE" using 1:2  with linespoints ls 7 title "$IR_LBL=$ir, $a_LBL=1/$alphaDen, $CHR$CURR_CHR"
##set output "$a_LBL$alphaDen-$dataset-bpb.$PIX_FORMAT"       # set output name
##plot "$FLD_dat/${IR_LBL}0-$a_LBL$alphaDen-${dataset}.$INF_FILE_TYPE" using 1:2  with linespoints ls 6 title "$IR_LBL=0, $a_LBL=1/$alphaDen, $CHR$CURR_CHR", \
##     "$FLD_dat/${IR_LBL}1-$a_LBL$alphaDen-${dataset}.$INF_FILE_TYPE" using 1:2  with linespoints ls 7 title "$IR_LBL=1, $a_LBL=1/$alphaDen, $CHR$CURR_CHR"
#set output "$IR_LBL$ir-$a_LBL$alphaDen-bpb.$PIX_FORMAT"       # set output name
#set title "IR=$ir,   Alpha=$alphaDen"
##plot for [i=1:22] "$FLD_archive_dat/$IR_LBL$ir-$a_LBL$alphaDen-HS".i.".$INF_FILE_TYPE" using 1:2  with linespoints ls "".i."" title "${CHR} ".i."", \
##     "$FLD_archive_dat/$IR_LBL$ir-$a_LBL$alphaDen-HSX.$INF_FILE_TYPE" using 1:2  with linespoints ls 23 title "${CHR} X", \
##     "$FLD_archive_dat/$IR_LBL$ir-$a_LBL$alphaDen-HSY.$INF_FILE_TYPE" using 1:2  with linespoints ls 24 title "${CHR} Y", \
#
#plot \
# for [i=1:8]    "$FLD_archive_dat/$IR_LBL$ir-$a_LBL$alphaDen-HS".i.".$INF_FILE_TYPE" using 1:2  with linespoints ls "".i."" title "${CHR} ".i."", \
# for [i=10:12]  "$FLD_archive_dat/$IR_LBL$ir-$a_LBL$alphaDen-HS".i.".$INF_FILE_TYPE" using 1:2  with linespoints ls "".i."" title "${CHR} ".i."", \
# for [i=18:18]  "$FLD_archive_dat/$IR_LBL$ir-$a_LBL$alphaDen-HS".i.".$INF_FILE_TYPE" using 1:2  with linespoints ls "".i."" title "${CHR} ".i."", \
#                "$FLD_archive_dat/$IR_LBL$ir-$a_LBL$alphaDen-HSX.$INF_FILE_TYPE" using 1:2  with linespoints ls 23 title "${CHR} X", \
# for [i=9:9]    "$FLD_archive_dat/$IR_LBL$ir-$a_LBL$alphaDen-HS".i.".$INF_FILE_TYPE" using 1:2  with linespoints ls "".i."" title "${CHR} ".i."", \
# for [i=16:16]  "$FLD_archive_dat/$IR_LBL$ir-$a_LBL$alphaDen-HS".i.".$INF_FILE_TYPE" using 1:2  with linespoints ls "".i."" title "${CHR} ".i."", \
# for [i=19:19]  "$FLD_archive_dat/$IR_LBL$ir-$a_LBL$alphaDen-HS".i.".$INF_FILE_TYPE" using 1:2  with linespoints ls "".i."" title "${CHR} ".i."", \
## for [i=17:17]  "$FLD_archive_dat/$IR_LBL$ir-$a_LBL$alphaDen-HS".i.".$INF_FILE_TYPE" using 1:2  with linespoints ls "".i."" title "${CHR} ".i."", \
## for [i=21:21]  "$FLD_archive_dat/$IR_LBL$ir-$a_LBL$alphaDen-HS".i.".$INF_FILE_TYPE" using 1:2  with linespoints ls "".i."" title "${CHR} ".i."", \
## for [i=22:22]  "$FLD_archive_dat/$IR_LBL$ir-$a_LBL$alphaDen-HS".i.".$INF_FILE_TYPE" using 1:2  with linespoints ls "".i."" title "${CHR} ".i."", \
## "$FLD_archive_dat/$IR_LBL$ir-$a_LBL$alphaDen-HSY.$INF_FILE_TYPE" using 1:2  with linespoints ls 24 title "${CHR} Y", \
## for [i=13:15]  "$FLD_archive_dat/$IR_LBL$ir-$a_LBL$alphaDen-HS".i.".$INF_FILE_TYPE" using 1:2  with linespoints ls "".i."" title "${CHR} ".i."", \


###########################    ctx    ##########################
##set ylabel "context-order size"
##set terminal pngcairo size 600, 850
#set terminal $PIX_FORMAT size 600, 850
#set output "$IR_LBL$ir-$a_LBL$alphaDen-ctx.$PIX_FORMAT"
#set multiplot layout 12,2 columnsfirst margins 0.08,0.98,0.06,0.98 spacing 0.013,0.0
#set offset 0,0,graph 0.1, graph 0.1
#set key top right samplen 2 spacing 1.5 font ",11"
#
#LT=7                # linetype
#LW=2.0              # linewidth
#AxisNumScale=0.35   # axis numbers scale
#
#set grid
#set label 1 '%mutation' at screen 0.47,0.015
#set label 2 'context-order size' at screen 0.02,0.47 rotate by 90
#set xtics 5,5,50 scale 0.35                             # set steps for x axis
#set ytics 2,2,10 scale 0.5 offset 0.4,0 font ",10"      # set steps for y axis
#set yrange [2:10]
#
######   first column   #####
#do for [i=1:11] {
#set xtics format ''
#plot "$FLD_archive_dat/$IR_LBL$ir-$a_LBL$alphaDen-HS".i.".$INF_FILE_TYPE" using 1:3 \
#     with lines linetype LT linewidth LW title "".i.""
#}
####   chromosome 12   ###
#set xtics add ("1" 1, "5" 5, "10" 10, "15" 15, "20" 20, "25" 25, "30" 30, "35" 35, "40" 40, "45" 45, "50  " 50) \
#    scale AxisNumScale offset 0.25,0.4 font ",10"
#plot "$FLD_archive_dat/$IR_LBL$ir-$a_LBL$alphaDen-HS12.$INF_FILE_TYPE" using 1:3 \
#     with lines linetype LT linewidth LW title "12"
#
######   second column   #####
#do for [i=13:22] {
#set xtics 5,5,50 scale AxisNumScale
#set xtics format ''
#set ytics format ''
#plot "$FLD_archive_dat/$IR_LBL$ir-$a_LBL$alphaDen-HS".i.".$INF_FILE_TYPE" using 1:3 \
#     with lines linetype LT linewidth LW title "".i.""
#}
####   chromosome X   ###
#plot "$FLD_archive_dat/$IR_LBL$ir-$a_LBL$alphaDen-HS23.$INF_FILE_TYPE" using 1:3 \
#     with lines linetype LT linewidth LW title "X"
####   chromosome Y   ###
#set xtics add ("  1" 1, "5" 5, "10" 10, "15" 15, "20" 20, "25" 25, "30" 30, "35" 35, "40" 40, "45" 45, "50" 50) \
#    scale AxisNumScale offset 0.25,0.4 font ",10"
#plot "$FLD_archive_dat/$IR_LBL$ir-$a_LBL$alphaDen-HS24.$INF_FILE_TYPE" using 1:3 \
#     with lines linetype LT linewidth LW title "Y"
#
#unset multiplot; set output

# the following line (EOF) MUST be left as it is; i.e. no space, etc
EOF

#   done
#  done
 done
done

fi  #end of $PLOT_RESULTS


#***********************************************************
#   build matrix from Reference Target correspondence
#***********************************************************
if [[ $BUILD_MATRIX == 1 ]]; then

cd $FLD_dat

for i in $TAR_SEQ_RUN; do printf "\t%s" "$TAR_SPECIE$i" >> "${TAR_SPECIE}_HORIZ_PAD"; done;    echo >> "${TAR_SPECIE}_HORIZ_PAD"

for alphaDen in $ALPHA_DENS; do
 for i in $INV_REPEATS; do
  cat "${TAR_SPECIE}_HORIZ_PAD" >> "tot-$IR_LBL$i-$REF_SPECIE-$TAR_SPECIE.$INF_FILE_TYPE"

  for c in $REF_SEQ_RUN; do
   awk -F "\t" '{print $7}' "$IR_LBL$i-$REF_SPECIE$c-$TAR_SPECIE.$INF_FILE_TYPE" \
        | awk -v ref_ch=$REF_SPECIE$c 'NR == 1 {print ref_ch; next} {print}' | tr '\n' '\t' | tr ',' '.' \
        >> "tot-$IR_LBL$i-$REF_SPECIE-$TAR_SPECIE.$INF_FILE_TYPE"
   echo >> "tot-$IR_LBL$i-$REF_SPECIE-$TAR_SPECIE.$INF_FILE_TYPE"
  done

 ###*** filter NRC values greater than 1
  awk 'NR>1 ' tot-$IR_LBL$i-$REF_SPECIE-$TAR_SPECIE.dat \
   | awk '{for (i=1;i<=NF;i++) if($i!=$i+0) printf "%s\t", $i; \
			              else if($i==$i+0 && $i>1) printf "%.5f\t", 1; \
 		   	              else if($i==$i+0 && $i<=1) printf "%.5f\t", $i; \
 		   	              print ""}' \
   > temp;
  cat "${TAR_SPECIE}_HORIZ_PAD" > "temp-tot"
  cat "temp" >> "temp-tot"
  rm -f "tot-$IR_LBL$i-$REF_SPECIE-$TAR_SPECIE.dat";
  mv "temp-tot" "tot-$IR_LBL$i-$REF_SPECIE-$TAR_SPECIE.dat";
  rm -f "temp" "temp-tot"
 done

#       | tr ',' '.' | awk 'NR == 1 {print; next} {print}' \
#       | awk '{for (i=1;i<=NF/2;i++) printf "%s\t", ($i==$i+0)?$i-$(i+NF/2):$i; print ""}' \
 paste "tot-${IR_LBL}0-$REF_SPECIE-$TAR_SPECIE.$INF_FILE_TYPE" "tot-${IR_LBL}1-$REF_SPECIE-$TAR_SPECIE.$INF_FILE_TYPE" \
       | tr ',' '.' \
       | awk '{for (i=1;i<=NF/2;i++) if($i==$i+0) {printf "%0.5f\t", $i-$(i+NF/2);} else if($i!=$i+0) {printf "%s\t", $i;} print ""}' \
       > "diff-$REF_SPECIE-$TAR_SPECIE.$INF_FILE_TYPE"

 echo -e "\t$(cat "diff-$REF_SPECIE-$TAR_SPECIE.$INF_FILE_TYPE")" > "diff-$REF_SPECIE-$TAR_SPECIE.$INF_FILE_TYPE"
done

rm -f *HORIZ_PAD

cd ..

fi  # end of $BUILD_MATRIX


#***********************************************************
#   plot matrix from Reference Target correspondence
#***********************************************************
if [[ $PLOT_MATRIX==1 ]]; then . $FLD_script/plot_matrix.sh; fi


#***********************************************************
#   archive data
#***********************************************************
if [[ $ARCHIVE_DATA == 1 ]]; then

#mkdir -p archive
mv $FLD_dat/* $FLD_archive_dat/

fi  # end of $ARCHIVE_DATA


########################
cd $FLD_script