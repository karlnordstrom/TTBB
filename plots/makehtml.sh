#!/bin/bash

pngs=png
html=html

## Really stupid script for creating simple html webpage out of event displays

cat <<EOF > ./$html/index.html
<html>
<head>
<title>Event displays</title>
<style>
  html { font-family: sans-serif; }
  img { border: 0; }
  a { text-decoration: none; font-weight: bold; }
</style></head>
<body>
<h3>Event displays comparing reconstructed jets to truth jets.</h3>
<p>
  Red cross -- truth jet which falls outside of the eta cut on the reconstructed jets <br>
  Blue cross -- truth jet which has less pT than the pT cut on the reconstructed jets <br>
  Black cross -- truth jet close to the lepton selected during reconstruction, removed from the reconstructed data <br>
  Green cross -- truth jet which falls in none of the above mentioned categories, "should" be reconstructed <br>
 <br>
  Black circle -- reconstructed jet <br>
  Green circle -- reconstructed lepton (to compare to the black crosses mentioned above)
</p>
EOF

cd $pngs
for png in $(ls *.png)
do

cp $png ../$html/plots/
cat <<EOF >> ../$html/index.html
<div style="float:left; font-size:smaller; font-weight:bold;">
    <img src="./plots/$png" height="337" width="518">
</div>
EOF

done

exit
