./LineSegmentDetector Calw1_10cm.ers out.shp
./OneImageBandMath Calw1_10cm.ers test1.tif test1.png "if(b1 > 0.0, 0, 0)"
gdal_translate -ot Byte test1.tif test_8.tif
gdal_rasterize -a FID -l out /home/kalxas/tmp/gnorasi_build/out.shp /home/kalxas/tmp/gnorasi_build/test_8.tif
./OneImageBandMath test_8.tif test_bin.tif test2.png "if(b1 > 0.0, 255, 0)"
gdal_translate -ot Byte test_bin.tif Calw1_10cm_LSD.tif
rm test*
gdal_translate -of "ERS" Calw1_10cm_LSD.tif Calw1_10cm_LSD.ers