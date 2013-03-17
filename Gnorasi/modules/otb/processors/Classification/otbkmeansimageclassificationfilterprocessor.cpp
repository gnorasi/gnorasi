/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) Draxis SA - www.draxis.gr - All rights reserved.		*
 *                                                                              *
 * This file is part of the GNORASI software package. GNORASI is free           *
 * software: you can redistribute it and/or modify it under the terms           *
 * of the GNU General Public License version 2 as published by the              *
 * Free Software Foundation.                                                    *
 *                                                                              *
 * GNORASI is distributed in the hope that it will be useful,                   *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of               *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                 *
 * GNU General Public License for more details.                                 *
 *                                                                              *
 * You should have received a copy of the GNU General Public License            *
 * in the file "LICENSE.txt" along with this program.                           *
 * If not, see <http://www.gnu.org/licenses/>.                                  *
 *                                                                              *
 ********************************************************************************/
 
#include "otbkmeansimageclassificationfilterprocessor.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {
const std::string OTBKMeansImageClassificationFilterProcessor::loggerCat_("voreen.OTBKMeansImageClassificationFilterProcessor");

OTBKMeansImageClassificationFilterProcessor::OTBKMeansImageClassificationFilterProcessor()
    : OTBImageFilterProcessor(),
    inPort_(Port::INPORT, "Vector Image Input", 0),
    outPort_(Port::OUTPORT, "Image Output", 0),
    m_nbClassesProperty("nbClasses", "Number of Classes", 3),
    m_centroidsProperty("centroids", "Centrtoids")
{
    addPort(inPort_);
    addPort(outPort_);

    addProperty(m_nbClassesProperty);
    addProperty(m_centroidsProperty);

    classifier = ClassificationFilterType::New();

}

void OTBKMeansImageClassificationFilterProcessor::update(){
    process();
}

Processor* OTBKMeansImageClassificationFilterProcessor::create() const {
  return new OTBKMeansImageClassificationFilterProcessor();
}

OTBKMeansImageClassificationFilterProcessor::~OTBKMeansImageClassificationFilterProcessor() {

}

void OTBKMeansImageClassificationFilterProcessor::initialize() throw (tgt::Exception) {

    Processor::initialize();
}

void OTBKMeansImageClassificationFilterProcessor::deinitialize() throw (tgt::Exception) {

    Processor::deinitialize();
}

std::string OTBKMeansImageClassificationFilterProcessor::getProcessorInfo() const {
    
    return "Classification of a VectorImage using a KMeans estimation result";
}

void OTBKMeansImageClassificationFilterProcessor::process() {
  
    try
    {

        VImageType *img = inPort_.getData();
        if(!img)
            return;

        std::string text = m_centroidsProperty.get();
        std::vector<std::string> strings;
        std::istringstream f(text);
        std::string s;
        while (std::getline(f, s, ';')) {
            std::cout << s << std::endl;
            strings.push_back(s);
        }

        const unsigned int sampleSize = ClassificationFilterType::MaxSampleDimension;
        const unsigned int parameterSize = m_nbClassesProperty.get() * sampleSize;
        KMeansParametersType parameters;

        int nbp = img->GetNumberOfComponentsPerPixel();

        if(strings.size() != m_nbClassesProperty.get() * nbp){
            std::cout << "the number of centroids must be equyal to : " << sampleSize * img->GetNumberOfComponentsPerPixel();
            return;
        }

        parameters.SetSize(parameterSize);
        parameters.Fill(0);

        for (unsigned int i = 0; i<m_nbClassesProperty.get();++i)
        {
          for (unsigned int j = 0; j < img->GetNumberOfComponentsPerPixel();++j)
          {
                std::string cs = strings[i*img->GetNumberOfComponentsPerPixel()+j];
                const char * c = cs.c_str();
                parameters[i*sampleSize+j]= atof(c);
          }
        }

        classifier->SetCentroids(parameters);
        classifier->SetInput(img);

        classifier->Update();

        outPort_.setData(classifier->GetOutput());

    }
    catch (int e)
    {
    LERROR("Problem with KMeans Classification!");
	return;
    }
    
}

} // namespace
