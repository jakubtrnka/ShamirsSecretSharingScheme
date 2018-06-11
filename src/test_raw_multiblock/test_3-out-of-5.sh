#!/bin/bash

# generated shamirs share: 3-out-of-5
# last check should fail

#   1 9e3fe1d911052aa8fec2f1b23121d3dac37d7773934a19e7e950cf
#   2 99e6e1ac6039edce4b6bfc80698f3c904dc6caf22cfefd90665b50
#   3 53b6741a5156a246c3cc615b3bcbcf3eefd1d3e09fce9405ee7dfe
#   4 c6c2e31428ce365ed40ea5abf939b11dee1753b6e7d416cc5fb3c0
#   5 0c9276a219a179d65ca93870ab7d42b34c004aa454e47f59d7956e

echo "ok"
./multiblockshamir_test << EOF 
5 0c9276a219a179d65ca93870ab7d42b34c004aa454e47f59d7956e
1 9e3fe1d911052aa8fec2f1b23121d3dac37d7773934a19e7e950cf
4 c6c2e31428ce365ed40ea5abf939b11dee1753b6e7d416cc5fb3c0
EOF

echo ""
echo "ok"
./multiblockshamir_test << EOF
1 9e3fe1d911052aa8fec2f1b23121d3dac37d7773934a19e7e950cf
2 99e6e1ac6039edce4b6bfc80698f3c904dc6caf22cfefd90665b50
4 c6c2e31428ce365ed40ea5abf939b11dee1753b6e7d416cc5fb3c0
EOF

echo ""
echo "ok"
./multiblockshamir_test << EOF
1 9e3fe1d911052aa8fec2f1b23121d3dac37d7773934a19e7e950cf
2 99e6e1ac6039edce4b6bfc80698f3c904dc6caf22cfefd90665b50
3 53b6741a5156a246c3cc615b3bcbcf3eefd1d3e09fce9405ee7dfe
4 c6c2e31428ce365ed40ea5abf939b11dee1753b6e7d416cc5fb3c0
5 0c9276a219a179d65ca93870ab7d42b34c004aa454e47f59d7956e
EOF

echo ""
echo "Not ok: not enough shares"
./multiblockshamir_test << EOF
4 c6c2e31428ce365ed40ea5abf939b11dee1753b6e7d416cc5fb3c0
5 0c9276a219a179d65ca93870ab7d42b34c004aa454e47f59d7956e
EOF

echo "Not OK: duplicate shares"
./multiblockshamir_test << EOF
1 9e3fe1d911052aa8fec2f1b23121d3dac37d7773934a19e7e950cf
2 99e6e1ac6039edce4b6bfc80698f3c904dc6caf22cfefd90665b50
2 99e6e1ac6039edce4b6bfc80698f3c904dc6caf22cfefd90665b50
EOF
