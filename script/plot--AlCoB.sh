#!/bin/bash


#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#   total ir=0 & ir=1
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
for ir in $INV_REPS; do
    for alphaDen in $ALPHA_DENS; do

gnuplot <<- EOF

#-------------------------------------------------------------------------------
#	terminal, output 
#-------------------------------------------------------------------------------
#set terminal $PIX_FORMAT enhanced color size 7.3,6.6	# F-F
set terminal $PIX_FORMAT enhanced color size 6.5,4.45	# V-V

set output "tot-i$ir-$REF-$TAR.$PIX_FORMAT"

#-------------------------------------------------------------------------------
#	multiplot layout, offset, size ratio, key
#-------------------------------------------------------------------------------
#set offset 0,0,graph 0.1, graph 0.1

xticsOffset=0.2
yticsOffset=0
#xlabelOffset=18.5	# F-F
xlabelOffset=6		# V-V
#ylabelOffset=31	# F-F
ylabelOffset=8		# V-V

#set size ratio .9  #0.85

set key off

#-------------------------------------------------------------------------------
#	margin	
#-------------------------------------------------------------------------------
##set tmargin 2.1    ### with title
set tmargin 0.5    ### without title
#set bmargin 23.25	# F-F
set bmargin 14		# V-V
#set lmargin 42		# F-F
set lmargin 27		# V-V
set rmargin 2

#-------------------------------------------------------------------------------
#	font, colorbox  	
#-------------------------------------------------------------------------------
#set pm3d map
set macros
fontLabelSpecies='font "Latin Modern Math Italic, 14"'
fontLabel='font "Latin Modern Math, 14"'
fontTics='font "Latin Modern Sans, 10"'

#set nocbtics
set cblabel "NRC_{IR=$ir}" @fontLabel offset 0,0     #-0.25 or -1.5
set cbtics scale 0.5 @fontTics offset -0.65,0
#set cbtics
#unset colorbox
#set cbrange [ -0.05 : 0.3 ] noreverse nowriteback  # diff-HS-PT

#-------------------------------------------------------------------------------
#	color palette, title, tics, label  	
#-------------------------------------------------------------------------------
set palette defined (0 "red", 0.5 "green", 1 "white")     # tot F-F & V-V

#set title "The difference"
#set title "Relative compression: HS-PT\nDifference between considering and\
#   not considering inverted repeats\nReference: HS, Target: PT"

###########    F-F    ##########	
#set xtics( \
#"Aspergillus oryzae 3.042 mitochondrion"                                   0,  \
#"Aspergillus oryzae mitochondrion"                                         1,  \
#"Colletotrichum acutatum strain KC05 mitochondrion"                        2,  \
#"Colletotrichum fioriniae mitochondrion"                                   3,  \
#"Colletotrichum lupini strain CBS 119142 culture-collection mitochondrion" 4,  \
#"Colletotrichum tamarilloi mitochondrion"                                  5,  \
#"Gigaspora margarita mitochondrion"                                        6,  \
#"Gigaspora rosea mitochondrion"                                            7,  \
#"Madurella mycetomatis mitochondrion"                                      8,  \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c01"                9,  \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c10"                10, \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c11"                11, \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c25"                12, \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c27"                13, \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c28"                14, \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c30"                15, \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c31"                16, \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c32"                17, \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c34"                18, \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c35"                19, \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c36"                20, \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c39"                21, \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c42"                22, \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c44"                23, \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c46"                24, \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c47"                25, \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c48"                26, \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c49"                27, \
#"Verticillium dahliae mitochondrion"                                       28, \
#"Verticillium nonalfalfae mitochondrion"                                   29, \
#         ) right @fontTics rotate by 90 offset 0,xticsOffset
#
#
#set ytics( \
#"Verticillium dahliae mitochondrion"                                       0,  \
#"Talaromyces marneffei mitochondrion"                                      1,  \
#"Rhizophagus fasciculatus isolate DAOM240159 mitochondrion"                2,  \
#"Pyronema omphalodes st. CBS 100304 culture-collection mitochondrion"      3,  \
#"Pseudogymnoascus destructans st. 20631-21 mitochondrion"                  4,  \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c48"                5,  \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c47"                6,  \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c46"                7,  \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c45"                8,  \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c43"                9,  \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c41"                10, \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c38"                11, \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c35"                12, \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c34"                13, \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c33"                14, \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c31"                15, \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c30"                16, \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c29"                17, \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c27"                18, \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c26"                19, \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c24"                20, \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c10"                21, \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c09"                22, \
#"Metarhizium anisopliae mitochondrion"                                     23, \
#"Marssonina brunnea f. sp. 'multigermtubi' mitochondrion"                  24, \
#"Lecanora strobilina mitochondrion"                                        25, \
#"Gigaspora rosea mitochondrion"                                            26, \
#"Colletotrichum lupini st. CBS 119142 culture-collection mitochondrion"    27, \
#"Colletotrichum lindemuthianum mitochondrion"                              28, \
#"Candida phangngaensis voucher CBS:10407 mitochondrion"                    29, \
#	) right @fontTics rotate by 0 offset yticsOffset,0
#

##########    V-V    ##########	
set xtics( \
"Enterococcus phage SANTOR1"            0,  \
"Escherichia phage Envy"                1,  \
"Escherichia phage Gluttony"            2,  \
"Escherichia phage HY03"                3,  \
"Escherichia phage UFV-AREG1"           4,  \
"Escherichia phage vB\\\_EcoM-UFV13"    5,  \
"Klebsiella phage PKO111"               6,  \
"Klebsiella phage vB\\\_KpnM\\\_KpV477" 7,  \
"Pseudomonas phage K5"                  8,  \
"Pseudomonas phage PAK\\\_P4"           9,  \
"Pseudomonas phage phiMK"               10, \
"Pseudomonas phage vB\\\_PaeM\\\_MAG1"  11, \
"Salmonella phage 118970\\\_sal1"       12, \
"Salmonella phage BP12C"                13, \
"Shigella phage SHBML-50-1"             14, \
"Shigella phage SHFML-11"               15, \
"Shigella phage SHFML-26"               16, \
"Streptococcus phage phiARI0004"        17, \
"Streptococcus phage phiARI0031"        18, \
"Streptococcus phage phiARI0131-1"      19, \
"Streptococcus phage phiARI0131-2"      20, \
"Streptococcus phage phiARI0460-1"      21, \
"Streptococcus phage phiARI0462"        22, \
"Streptococcus phage phiARI0468-1"      23, \
"Streptococcus phage phiARI0468-2"      24, \
"Streptococcus phage phiARI0923"        25, \
         ) right @fontTics rotate by 90 offset 0,xticsOffset


set ytics( \
"Streptococcus pneumoniae bacteriophage MM1"        0,  \
"Shigella phage SHFML-26"               1,  \
"Shigella phage SHFML-11"               2,  \
"Shigella phage SHBML-50-1"             3,  \
"Salmonella phage BP12C"                4,  \
"Salmonella phage 118970\\\_sal1"       5,  \
"Pseudomonas phage vB\\\_PaeM\\\_MAG1"  6,  \
"Pseudomonas phage phiMK"               7,  \
"Pseudomonas phage PAK_P4"              8,  \
"Pseudomonas phage K5"                  9,  \
"Klebsiella phage vB\\\_KpnM\\\_KpV477" 10, \
"Klebsiella phage vB\\\_KpnM\\\_KpV477" 11, \
"Escherichia phage vB\\\_EcoM-UFV13"    12, \
"Escherichia phage UFV-AREG1"           13, \
"Escherichia phage HY03"                14, \
"Escherichia phage Gluttony"            15, \
"Escherichia phage Envy"                16, \
"Enterococcus phage Ec-ZZ2"             17, \
	) right @fontTics rotate by 0 offset yticsOffset,0


set xlabel "$TAR_SNAME" offset 0,xlabelOffset @fontLabelSpecies
set ylabel "$REF_SNAME" offset ylabelOffset,0 @fontLabelSpecies

plot "<awk 'NR>1' '$FLD_dat/tot_filtered-i$ir-$REF-$TAR.$INF_FTYPE' \
    | cut -f2-" matrix with image

unset multiplot; set output

EOF

    done
done


#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#   difference
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#for ir in $INV_REPS; do
    for alphaDen in $ALPHA_DENS; do

gnuplot <<- EOF

#-------------------------------------------------------------------------------
#	terminal, output 
#-------------------------------------------------------------------------------
#set terminal $PIX_FORMAT enhanced color size 7.3,6.6	# F-F
set terminal $PIX_FORMAT enhanced color size 6.5,4.45	# V-V

set output "diff-$REF-$TAR.$PIX_FORMAT"

#-------------------------------------------------------------------------------
#	multiplot layout, offset, size ratio, key
#-------------------------------------------------------------------------------
#set offset 0,0,graph 0.1, graph 0.1

xticsOffset=0.2
yticsOffset=0
#xlabelOffset=18.5	# F-F
xlabelOffset=6		# V-V
#ylabelOffset=31	# F-F
ylabelOffset=18.5		# V-V

#set size ratio .9  #0.85

set key off

#-------------------------------------------------------------------------------
#	margin	
#-------------------------------------------------------------------------------
##set tmargin 2.1    ### with title
set tmargin 0.5    ### without title
#set bmargin 23.25	# F-F
set bmargin 14		# V-V
#set lmargin 42		# F-F
#set lmargin 27		# V-V
set lmargin 27.5		# V-V
set rmargin 2

#-------------------------------------------------------------------------------
#	font, colorbox  	
#-------------------------------------------------------------------------------
#set pm3d map
set macros
fontLabelSpecies='font "Latin Modern Math Italic, 14"'
fontLabel='font "Latin Modern Math, 14"'
fontTics='font "Latin Modern Sans, 10"'

#set nocbtics
set cblabel "NRC_{IR=0} - NRC_{IR=1}" @fontLabel offset 0,0     #-0.25 or -1.5
set cbtics scale 0.5 @fontTics offset -0.65,0
#set cbtics
#unset colorbox
#set cbrange [ -0.05 : 0.3 ] noreverse nowriteback  # diff-HS-PT

#-------------------------------------------------------------------------------
#	color palette, title, tics, label  	
#-------------------------------------------------------------------------------
#set palette defined (-0.1 "white", 0.45 "green", 1 "red")     # diff-F-F
set palette defined (-0.1 "white", 0.3 "green", 0.7 "red")     # diff-V-V

#set title "The difference"
#set title "Relative compression: HS-PT\nDifference between considering and\
#   not considering inverted repeats\nReference: HS, Target: PT"

###########    F-F    ##########	
#set xtics( \
#"Aspergillus oryzae 3.042 mitochondrion"                                   0,  \
#"Aspergillus oryzae mitochondrion"                                         1,  \
#"Colletotrichum acutatum strain KC05 mitochondrion"                        2,  \
#"Colletotrichum fioriniae mitochondrion"                                   3,  \
#"Colletotrichum lupini strain CBS 119142 culture-collection mitochondrion" 4,  \
#"Colletotrichum tamarilloi mitochondrion"                                  5,  \
#"Gigaspora margarita mitochondrion"                                        6,  \
#"Gigaspora rosea mitochondrion"                                            7,  \
#"Madurella mycetomatis mitochondrion"                                      8,  \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c01"                9,  \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c10"                10, \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c11"                11, \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c25"                12, \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c27"                13, \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c28"                14, \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c30"                15, \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c31"                16, \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c32"                17, \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c34"                18, \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c35"                19, \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c36"                20, \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c39"                21, \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c42"                22, \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c44"                23, \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c46"                24, \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c47"                25, \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c48"                26, \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c49"                27, \
#"Verticillium dahliae mitochondrion"                                       28, \
#"Verticillium nonalfalfae mitochondrion"                                   29, \
#         ) right @fontTics rotate by 90 offset 0,xticsOffset
#
#
#set ytics( \
#"Verticillium dahliae mitochondrion"                                       0,  \
#"Talaromyces marneffei mitochondrion"                                      1,  \
#"Rhizophagus fasciculatus isolate DAOM240159 mitochondrion"                2,  \
#"Pyronema omphalodes st. CBS 100304 culture-collection mitochondrion"      3,  \
#"Pseudogymnoascus destructans st. 20631-21 mitochondrion"                  4,  \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c48"                5,  \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c47"                6,  \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c46"                7,  \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c45"                8,  \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c43"                9,  \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c41"                10, \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c38"                11, \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c35"                12, \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c34"                13, \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c33"                14, \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c31"                15, \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c30"                16, \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c29"                17, \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c27"                18, \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c26"                19, \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c24"                20, \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c10"                21, \
#"Penicillium chrysogenum Wisconsin 54-1255, contig Pc00c09"                22, \
#"Metarhizium anisopliae mitochondrion"                                     23, \
#"Marssonina brunnea f. sp. 'multigermtubi' mitochondrion"                  24, \
#"Lecanora strobilina mitochondrion"                                        25, \
#"Gigaspora rosea mitochondrion"                                            26, \
#"Colletotrichum lupini st. CBS 119142 culture-collection mitochondrion"    27, \
#"Colletotrichum lindemuthianum mitochondrion"                              28, \
#"Candida phangngaensis voucher CBS:10407 mitochondrion"                    29, \
#	) right @fontTics rotate by 0 offset yticsOffset,0
#

##########    V-V    ##########	
set xtics( \
"Enterococcus phage SANTOR1"            0,  \
"Escherichia phage Envy"                1,  \
"Escherichia phage Gluttony"            2,  \
"Escherichia phage HY03"                3,  \
"Escherichia phage UFV-AREG1"           4,  \
"Escherichia phage vB\\\_EcoM-UFV13"    5,  \
"Klebsiella phage PKO111"               6,  \
"Klebsiella phage vB\\\_KpnM\\\_KpV477" 7,  \
"Pseudomonas phage K5"                  8,  \
"Pseudomonas phage PAK\\\_P4"           9,  \
"Pseudomonas phage phiMK"               10, \
"Pseudomonas phage vB\\\_PaeM\\\_MAG1"  11, \
"Salmonella phage 118970\\\_sal1"       12, \
"Salmonella phage BP12C"                13, \
"Shigella phage SHBML-50-1"             14, \
"Shigella phage SHFML-11"               15, \
"Shigella phage SHFML-26"               16, \
"Streptococcus phage phiARI0004"        17, \
"Streptococcus phage phiARI0031"        18, \
"Streptococcus phage phiARI0131-1"      19, \
"Streptococcus phage phiARI0131-2"      20, \
"Streptococcus phage phiARI0460-1"      21, \
"Streptococcus phage phiARI0462"        22, \
"Streptococcus phage phiARI0468-1"      23, \
"Streptococcus phage phiARI0468-2"      24, \
"Streptococcus phage phiARI0923"        25, \
         ) right @fontTics rotate by 90 offset 0,xticsOffset


set ytics( \
"Streptococcus pneumoniae bacteriophage MM1"        0,  \
"Shigella phage SHFML-26"               1,  \
"Shigella phage SHFML-11"               2,  \
"Shigella phage SHBML-50-1"             3,  \
"Salmonella phage BP12C"                4,  \
"Salmonella phage 118970\\\_sal1"       5,  \
"Pseudomonas phage vB\\\_PaeM\\\_MAG1"  6,  \
"Pseudomonas phage phiMK"               7,  \
"Pseudomonas phage PAK\\\_P4"           8,  \
"Pseudomonas phage K5"                  9,  \
"Klebsiella phage vB\\\_KpnM\\\_KpV477" 10, \
"Klebsiella phage vB\\\_KpnM\\\_KpV477" 11, \
"Escherichia phage vB\\\_EcoM-UFV13"    12, \
"Escherichia phage UFV-AREG1"           13, \
"Escherichia phage HY03"                14, \
"Escherichia phage Gluttony"            15, \
"Escherichia phage Envy"                16, \
"Enterococcus phage Ec-ZZ2"             17, \
	) right @fontTics rotate by 0 offset yticsOffset,0


set xlabel "$TAR_SNAME" offset 0,xlabelOffset @fontLabelSpecies
set ylabel "$REF_SNAME" offset ylabelOffset,0 @fontLabelSpecies

plot "<awk 'NR>1' '$FLD_dat/diff_filtered-$REF-$TAR.$INF_FTYPE' \
    | cut -f2-" matrix with image

unset multiplot; set output

EOF

    done
#done
