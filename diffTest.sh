make clean all

./`ls -t NESEmu_BUILD_* | head -1` nestest.nes > output.log

#Credit for following command goes to https://github.com/docprofsky
diff --unchanged-line-format="" --old-line-format="" --new-line-format=":%dn: %L" <(awk -F':' '{print $2, $3, $4, $5}' nestest.log | awk -F' ' '{print "A:" $1, "X:"$3, "Y:"$5, "P:"$7}') <(awk -F':' '{print $4, $5, $6, $7}' output.log | awk -F' ' '{print "A:" $1, "X:"$3, "Y:"$5, "P:"$7}')
