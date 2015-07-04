make clean all

./`ls -t NESEmu_BUILD_* | head -1` nestest.nes | tee output.log | awk -F' ' 'BEGIN{OFS=" "}{print $2, $4, $5, $6, $7}' > output.out

# Credit for following command goes to https://github.com/docprofsky
echo -e "line\toutput\t\t\t\texpected";diff --unchanged-line-format="" --old-line-format="" --new-line-format=":%dn: %L" nestest.out  output.out | awk -F':' 'BEGIN{OFS=":";ORS="\t"}{$1=$1; print; system("head -n "$2" nestest.out | tail -1" )}'

