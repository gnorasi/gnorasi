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

./MeanDifferenceChangeDetection rethymno_before_b4.img rethymno_after_b4.img mean_diff.img 1
dal_translate -of JPEG mean_diff.img mean_diff.jpg
./RatioChangeDetection rethymno_before_b4.img rethymno_after_b4.img rethymno_ratio_3.img 3
./RatioChangeDetection rethymno_before_b4.img rethymno_after_b4.img rethymno_ratio_1.img 1
./RatioChangeDetection rethymno_before_b4.img rethymno_after_b4.img rethymno_ratio5.img 5
./LocalCorrelationChangeDetection rethymno_before_b4.img rethymno_after_b4.img rethymno_corel_5.img 5
./LocalCorrelationChangeDetection rethymno_before_b4.img rethymno_after_b4.img rethymno_corel_1.img 1
./KullbackLeiblerDistanceChangeDetection rethymno_before_b4.img rethymno_after_b4.img rethymno_kullback_9x9.img 9
./KullbackLeiblerDistanceChangeDetection rethymno_before_b4.img rethymno_after_b4.img rethymno_kullback_7x7.img 7


