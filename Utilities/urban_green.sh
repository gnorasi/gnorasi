./SimpleViewer Heraklion_urban_green_sub.tif
./MeanShiftSegmentation Heraklion_urban_green_sub.tif MSFilteredOut.tif MSClusteredOut.tif MSLabeledOut.tif MSBoundOut.tif MSFilteredPretty.jpg MSClusteredPretty.jpg 7 30 10 1.0
gdal_translate -b 3 -b 2 -b 1 MSClusteredPretty.jpg Heraklion_ObjectMeans.png
gdal_translate -ot Byte -of JPEG -scale MSBoundOut.tif Heraklion_Objects.jpg
./OneImageBandMath Heraklion_urban_green_sub.tif urban_green.img urban_green.png "if((b4-b3)/(b4+b3) > 0.2, 255, 0)"
