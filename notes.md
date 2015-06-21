Find diff in output:

diff --unchanged-line-format="" --old-line-format="" --new-line-format=":%dn: %L" <(cat nestest.log | awk -F'P:' '{print $2}' | awk -F' ' '{print $1}') <(cat output.txt | awk -F'P:' '{print $3}' | awk -F'{' '{print $1}') | head

