/********************************************************************************
 *                                                                    		*
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                		*
 * 								      		*
 * Language:  C++						      		*
 * 										*
 * Copyright (c) Angelos Tzotsos <tzotsos@gmail.com>. All rights reserved. 	*
 * Copyright (c) National Technical University of Athens. All rights reserved.	*
 * Copyright (c) Informatics and Telematics Institute				*
 *	  Centre for Research and Technology Hellas. All rights reserved.	*
 * 										*
 *                                                                    		*
 * This file is part of the GNORASI software package. GNORASI is free  		*
 * software: you can redistribute it and/or modify it under the terms 		*
 * of the GNU General Public License version 2 as published by the    		*
 * Free Software Foundation.                                          		*
 *                                                                    		*
 * GNORASI is distributed in the hope that it will be useful,          		*
 * but WITHOUT ANY WARRANTY; without even the implied warranty of     		*
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the       		*
 * GNU General Public License for more details.                       		*
 *                                                                    		*
 * You should have received a copy of the GNU General Public License  		*
 * in the file "LICENSE.txt" along with this program.                 		*
 * If not, see <http://www.gnu.org/licenses/>.                        		*
 *                                                                    		*
 ********************************************************************************/

#include "otbmodule.h"

#include "processors/ImageIO/otbimagereaderprocessor.h"
#include "processors/ImageIO/otblabelimagereaderprocessor.h"
#include "processors/ImageIO/otbvectorimagereaderprocessor.h"
#include "processors/ImageIO/otbimagewriterprocessor.h"
#include "processors/ImageIO/otbvectorimagewriterprocessor.h"
#include "processors/ImageIO/otblabelimagewriterprocessor.h"
#include "processors/ImageIO/otbimagetovectorimagecastprocessor.h"
#include "processors/ImageIO/otbcsvwriterprocessor.h"
#include "processors/ImageIO/otbcsvreaderprocessor.h"
#include "processors/BasicFilters/otbconvolutionimagefilterprocessor.h"
#include "processors/BasicFilters/otbfftconvolutionimagefilterprocessor.h"
#include "processors/BasicFilters/otbcannyedgedetectionimagefilterprocessor.h"
#include "processors/BasicFilters/otbgradientanisotropicdiffusionfilterprocessor.h"
#include "processors/BasicFilters/otbmeanimagefilterprocessor.h"
#include "processors/BasicFilters/otbmedianimagefilterprocessor.h"
#include "processors/BasicFilters/otbsobelimagefilterprocessor.h"
#include "processors/BasicFilters/otbmeanshiftimagesegmentationprocessor.h"
#include "processors/BasicFilters/otbbinarythresholdfilterprocessor.h"
#include "processors/BasicFilters/otbbinarydilatefilterprocessor.h"
#include "processors/BasicFilters/otbbinaryerodefilterprocessor.h"
#include "processors/BasicFilters/otbbinaryopeningfilterprocessor.h"
#include "processors/BasicFilters/otbbinaryclosingfilterprocessor.h"
#include "processors/OBIA/otblabelimagetolabelmapprocessor.h"
#include "processors/OBIA/otbshapeattributeslabelmapprocessor.h"
#include "processors/OBIA/otbbandstatisticsattributeslabelmapprocessor.h"
#include "processors/OBIA/otbstatisticsattributeslabelmapprocessor.h"
#include "processors/OBIA/otblabelmapstatisticwriterprocessor.h"
#include "processors/Radiometry/otbndviprocessor.h"
#include "processors/Radiometry/otbmsaviprocessor.h"
#include "processors/Radiometry/otbndwiprocessor.h"
#include "processors/Radiometry/otbvectorimagebandmathprocessor.h"
#include "processors/Radiometry/otbtwoimagebandmathprocessor.h"
#include "processors/Visualization/otbsimpleviewerprocessor.h"


namespace voreen {

OTBModule::OTBModule() 
    : VoreenModule()
{
    // module name to be used in the GUI
    setName("Orfeo Toolbox");
    
    // module description file
    setXMLFileName("otb/otbmodule.xml");

    // each module processor needs to be registered
    //addProcessor(new DummySegmentationProcessor());
    addProcessor(new OTBImageReaderProcessor());
    addProcessor(new OTBLabelImageReaderProcessor());
    addProcessor(new OTBVectorImageReaderProcessor());
    addProcessor(new OTBImageWriterProcessor());
    addProcessor(new OTBVectorImageWriterProcessor());
    addProcessor(new OTBImageToVectorImageCastProcessor());
    addProcessor(new OTBLabelImageWriterProcessor());
    addProcessor(new OTBCSVWriterProcessor());
    addProcessor(new OTBCSVReaderProcessor());
    addProcessor(new OTBConvolutionImageFilterProcessor());
    addProcessor(new OTBCannyEdgeDetectionImageFilterProcessor());
    addProcessor(new OTBSimpleViewerProcessor());
    addProcessor(new OTBMeanImageFilterProcessor());
    addProcessor(new OTBBinaryThresholdFilterProcessor());
    addProcessor(new OTBBinaryDilateFilterProcessor());
    addProcessor(new OTBBinaryErodeFilterProcessor());
    addProcessor(new OTBBinaryOpeningFilterProcessor());
    addProcessor(new OTBBinaryClosingFilterProcessor());
    addProcessor(new OTBMedianImageFilterProcessor());
    addProcessor(new OTBSobelImageFilterProcessor());
    addProcessor(new OTBGradientAnisotropicDiffusionFilterProcessor());
    addProcessor(new OTBMeanShiftSegmentationProcessor());
    addProcessor(new OTBLabelImageToLabelMapProcessor());
    addProcessor(new OTBShapeAttributesLabelMapProcessor());
    addProcessor(new OTBBandStatisticsAttributesLabelMapProcessor());
    addProcessor(new OTBStatisticsAttributesLabelMapProcessor());
    addProcessor(new OTBLabelMapStatisticsWriterProcessor());
    addProcessor(new OTBNDVIProcessor());
    addProcessor(new OTBMSAVIProcessor());
    addProcessor(new OTBNDWIProcessor());
    addProcessor(new OTBVectorImageBandMathProcessor());
    addProcessor(new OTBTwoVectorImagesBandMathProcessor());
    addProcessor(new OTBFFTConvolutionImageFilterProcessor());
}

} // namespace
