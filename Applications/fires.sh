./SimpleViewer 230_250.img
./SimpleViewer 242_250.img
./NDVI 230_250.img ndvi_230.img
./SimpleViewer ndvi_230.img
./NDVI 242_250.img ndvi_242.img
./SimpleViewer ndvi_242.img
./ImageDifference ndvi_230.img ndvi_242.img diff.img diff_8bit.img
./SimpleViewer diff.img
./BinaryThreshold diff.img thresh.img -1 -0.1 0 255
./SimpleViewer thresh.img 
./BinaryOpening thresh.img opened_1.img 1
./SimpleViewer opened_1.img 
./BinaryOpening thresh.img opened_3.img 3
./SimpleViewer opened_3.img 
