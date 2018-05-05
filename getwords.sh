#!/bin/bash

wget https://raw.githubusercontent.com/satoshilabs/slips/master/slip-0039/wordlist.txt
awk 'BEGIN{printf "\t{\n\t\t"}; { s = "\"" $1 "\","; printf "%-11s", s}; NR % 5 == 0 {printf "\n\t\t"}; NR % 5 != 0 {printf " "}; END {printf "\n\t}\n====================\n"} ' wordlist.txt > wordlist_h_template.txt
rm wordlist.txt

wget https://raw.githubusercontent.com/bitcoin/bips/master/bip-0039/english.txt
awk 'BEGIN{printf "\t{\n\t\t"}; { s = "\"" $1 "\","; printf "%-11s", s}; NR % 5 == 0 {printf "\n\t\t"}; NR % 5 != 0 {printf " "}; END {printf "\n\t}\n====================\n"} ' english.txt >> wordlist_h_template.txt
rm english.txt
