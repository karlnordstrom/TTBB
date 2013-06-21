#!/bin/bash

pngs=png
html=html
statistics=stats

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
  Green circle -- reconstructed lepton (to compare to the black crosses mentioned above) <br>
  <br>
  Small diamond -- indicates that this truth jet was matched to a closeby reconstructed jet <br>
  Red line -- indicates eta cut on the reconstructed jets
</p>
EOF

cd $pngs
for png in $(ls *.png)
do

if [ "$png" = "${png%constant.png}" ]; then
    directory=${png%pt.png}
fi
if [ "$png" = "${png%pt.png}" ]; then
    directory=${png%constant.png}
fi

cp $png ../$html/plots/

if [ "$png" = "${png%pt.png}" ]; then
cat <<EOF >> ../$html/index.html
<div style="float:left; font-size:smaller; font-weight:bold;">
    <a name="${png%.png}"><a href="./$directory/index.html">
    <img src="./plots/$png" height="337" width="518">
</div>
EOF
fi

done

cd ..
cd $statistics

for stat in $(ls)
do

if [ ! -d "../$html/$stat" ]; then
    mkdir ../$html/$stat
fi

cat <<EOF > ../$html/$stat/index.html
<html>
<head>
<title>Event display</title>
<style>
  html { font-family: sans-serif; }
  img { border: 0; }
  a { text-decoration: none; font-weight: bold; }
</style></head>
<body>
<h3>Information about $stat.</h3>
<p>
EOF
cat $stat >> ../$html/$stat/index.html
cat <<EOF >> ../$html/$stat/index.html
</p>
EOF

for pic in $(ls ../$html/plots/${stat}constant.png)
do
cat <<EOF >> ../$html/$stat/index.html
<div style="float:left; font-size:smaller; font-weight:bold;">
    <img src="plots/../../$pic" height="520" width="800">
</div>
EOF
done

for pic in $(ls ../$html/plots/${stat}pt.png)
do
cat <<EOF >> ../$html/$stat/index.html
<div style="float:left; font-size:smaller; font-weight:bold;">
    <img src="plots/../../$pic" height="520" width="800">
</div>
EOF
done


done

exit
