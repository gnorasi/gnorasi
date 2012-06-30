./SimpleViewer FIRE_230_250.img
./SimpleViewer FIRE_242_250.img
./NDVI FIRE_230_250.img ndvi_230.img
./SimpleViewer ndvi_230.img
./NDVI FIRE_242_250.img ndvi_242.img
./SimpleViewer ndvi_242.img
./ImageDifference ndvi_230.img ndvi_242.img diff.img diff_8bit.img
./SimpleViewer diff.img
./BinaryThreshold diff.img thresh.img -1 -0.1 0 255
./SimpleViewer thresh.img 
./BinaryOpening thresh.img opened.img 1
./SimpleViewer opened.img 
./BinaryOpening thresh.img final.img 3
./SimpleViewer final.img 
