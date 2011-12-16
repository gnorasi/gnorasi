./SimpleViewer ORTHO_EBROS_NEW.tif
./NDWI ORTHO_EBROS_NEW.tif ndwi.img
./SimpleViewer ndwi.img
gdal_translate -ot Byte -of JPEG -scale ndwi.img ndwi.jpg
./BinaryThreshold ndwi.img thresh.img 0.14 1 0 255
gdal_translate -ot Byte -of JPEG thresh.img thresh.jpg
./OneImageBandMath ORTHO_EBROS_NEW.tif thresh2.img pretty2.img "if(((if((b5 > 0),1,151) + b5) < 150),255,0)"
gdal_translate -ot Byte -of JPEG thresh2.img thresh2.jpg
gdal_translate -of JPEG -b 5 -b 4 -b 3 ORTHO_EBROS_NEW.tif 543.jpg
gdal_translate -of JPEG -b 5 ORTHO_EBROS_NEW.tif 5.jpg





