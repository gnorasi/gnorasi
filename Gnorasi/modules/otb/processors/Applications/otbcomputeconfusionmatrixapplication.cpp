/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) ALTEC SA - www.altec.gr - All rights reserved.                 *
 * Copyright (c) ALTEC SA - www.altec.gr - All rights reserved.                 *
 * Copyright (c) ALTEC SA - www.altec.gr - All rights reserved.                 *
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

#include "otbcomputeconfusionmatrixapplication.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string OTBComputeConfusionMatrixApplication::loggerCat_("voreen.OTBComputeConfusionMatrixApplication");

OTBComputeConfusionMatrixApplication::OTBComputeConfusionMatrixApplication()
    :OTBImageFilterProcessor(),
      groundTruthFormat_("groundTruthFormat_", "Choice of ground truth format"),
      vectorField_("vectorField_", "Field name"),
      noDataLabel_("noDataLabel_", "Value for nodata pixels", 0, 0, 255),
      ram_("ram_", "Available RAM (Mb)", 122, 1, 1024),
      csvPort_(Port::OUTPORT, "OUT CSV text", 0),
      inPort_(Port::INPORT, "IN OTB Image", 0),
      vectorDataPort_(Port::INPORT, "IN Vector Data", 0),
      referenceLabelPort_(Port::INPORT, "IN Label Image", 0)
{
    addPort(inPort_);
    addPort(referenceLabelPort_);
    addPort(vectorDataPort_);
    addPort(csvPort_);

    addProperty(groundTruthFormat_);

    groundTruthFormat_.addOption("blank", "Select format:");
    groundTruthFormat_.addOption("raster", "Ground truth as a raster image");
    groundTruthFormat_.addOption("vector", "Ground truth as vector data file");

    groundTruthFormat_.onChange(CallMemberAction<OTBComputeConfusionMatrixApplication>
                                (this,&OTBComputeConfusionMatrixApplication::updateFormat));

    addProperty(vectorField_);
    addProperty(noDataLabel_);
    addProperty(ram_);

    vectorField_.setVisible(false);

    rasterizeReference = RasterizeFilterType::New();
    splitter = SplitterType::New();
    confMatMeasurements = ConfusionMatrixMeasurementsType::New();
}

Processor* OTBComputeConfusionMatrixApplication::create() const {
    return new OTBComputeConfusionMatrixApplication();
}

OTBComputeConfusionMatrixApplication::~OTBComputeConfusionMatrixApplication() {
}

void OTBComputeConfusionMatrixApplication::initialize() throw (tgt::Exception) {
    Processor::initialize();
}

void OTBComputeConfusionMatrixApplication::deinitialize() throw (tgt::Exception) {
    Processor::deinitialize();
}

std::string OTBComputeConfusionMatrixApplication::getProcessorInfo() const {
    return "Compute Confusion Matrix Application";
}

bool OTBComputeConfusionMatrixApplication::isReady() const {

    if (!csvPort_.isConnected()) return false;

    if (groundTruthFormat_.get()=="vector") {
        if (!inPort_.isConnected() || !vectorDataPort_.isConnected())
        {
            LWARNING("Non defined input image or vector data!");
            return false;
        }
    } else {
        if (groundTruthFormat_.get()=="raster")
        {
            if (!inPort_.isConnected() || !referenceLabelPort_.isConnected())
            {
                LWARNING("Non defined input or reference image!");
                return false;
            }
        } else {
            LWARNING("Non defined format!");
            return false;   //"blank"
        }
    }
    return true;
}

void OTBComputeConfusionMatrixApplication::updateFormat() {

    if (groundTruthFormat_.get()=="vector") {
        vectorField_.setVisible(true);
    } else {
        vectorField_.setVisible(false);
    }
}

std::string OTBComputeConfusionMatrixApplication::LogConfusionMatrix(MapOfClassesType* mapOfClasses, ConfusionMatrixType* matrix) {

    // Compute minimal width
    size_t minwidth = 0;

    for (unsigned int i = 0; i < matrix->Rows(); i++)
    {
        for (unsigned int j = 0; j < matrix->Cols(); j++)
        {
            std::ostringstream os;
            os << (*matrix)(i,j);
            size_t size = os.str().size();

            if (size > minwidth)
            {
                minwidth = size;
            }
        }
    }
    MapOfClassesType::const_iterator it = mapOfClasses->begin();
    MapOfClassesType::const_iterator end = mapOfClasses->end();

    for(; it != end; ++it)
    {
        std::ostringstream os;
        os << "[" << it->first << "]";

        size_t size = os.str().size();
        if (size > minwidth)
        {
            minwidth = size;
        }
    }
    // Generate matrix string, with 'minwidth' as size specifier
    std::ostringstream os;

    // Header line
    for (size_t i = 0; i < minwidth; ++i)
        os << " ";
    os << " ";

    it = mapOfClasses->begin();
    end = mapOfClasses->end();
    for(; it != end; ++it)
    {
        //os << "[" << it->first << "]" << " ";
        os << "[" << std::setw(minwidth - 2) << it->first << "]" << " ";
    }

    os << std::endl;

    // Each line of confusion matrix
    it = mapOfClasses->begin();
    for (unsigned int i = 0; i < matrix->Rows(); i++)
    {
        ClassLabelType label = it->first;
        os << "[" << std::setw(minwidth - 2) << label << "]" << " ";
        for (unsigned int j = 0; j < matrix->Cols(); j++)
        {
            os << std::setw(minwidth) << (*matrix)(i,j) << " ";
        }
        os << std::endl;
        ++it;
    }

    LINFO("Confusion matrix (rows = reference labels, columns = produced labels):");
    LINFO(os.str());

    return os.str();
}

void OTBComputeConfusionMatrixApplication::process() {

    ImageType* input = inPort_.getData();
    std::string field;
    int nodata = noDataLabel_.get();
    ImageType* reference;
    otb::ogr::DataSource::Pointer ogrRef;

    if (groundTruthFormat_.get() == "raster")
    {
        reference = referenceLabelPort_.getData();
    }
    else
    {
        if (groundTruthFormat_.get() == "vector")
        {
            try
            {
                LINFO(vectorDataPort_.DataPath());
                ogrRef = otb::ogr::DataSource::New(vectorDataPort_.DataPath(), otb::ogr::DataSource::Modes::Read);
                field = vectorField_.get();
            } catch (int e) {
                LERROR("Error opening the vector DataSource!");
            }

            try
            {
                rasterizeReference->AddOGRDataSource(ogrRef);
                rasterizeReference->SetOutputParametersFromImage(input);
                rasterizeReference->SetBackgroundValue(nodata);
                rasterizeReference->SetBurnAttribute(field.c_str());

                reference = rasterizeReference->GetOutput();
                reference->UpdateOutputInformation();
            } catch (int e) {
                LERROR("Error in rasterize filter!");
            }
        }
        else
        {
            LERROR("No defined Ground truth format!");
        }
    }

    // Prepare local streaming
    unsigned int numberOfStreamDivisions;
    try
    {
        numberOfStreamDivisions = StreamingTraitsType::CalculateNumberOfStreamDivisions(
                    input,
                    input->GetLargestPossibleRegion(),
                    splitter,
                    otb::SET_BUFFER_MEMORY_SIZE,
                    0,
                    1048576*ram_.get(),
                    0);
        LINFO("Number of stream divisions : "<< numberOfStreamDivisions);
    } catch (int e) {
        LERROR("Error calculationg stream divisions!");
    }

    RegionType streamRegion;

    // Extraction of the Class Labels from the Reference image/rasterized vector data + filling of m_Matrix
    MapOfClassesType  mapOfClassesRef, mapOfClassesProd;
    MapOfClassesType::iterator  itMapOfClassesRef, itMapOfClassesProd;
    ClassLabelType labelRef = 0, labelProd = 0;
    int itLabelRef = 0, itLabelProd = 0;

    for (unsigned int index = 0; index < numberOfStreamDivisions; index++)
    {
        streamRegion = splitter->GetSplit(index, numberOfStreamDivisions, reference->GetLargestPossibleRegion());

        input->SetRequestedRegion(streamRegion);
        input->PropagateRequestedRegion();
        input->UpdateOutputData();

        reference->SetRequestedRegion(streamRegion);
        reference->PropagateRequestedRegion();
        reference->UpdateOutputData();

        ImageIteratorType itInput(input, streamRegion);
        itInput.GoToBegin();

        ImageIteratorType itRef(reference, streamRegion);
        itRef.GoToBegin();

        while (!itRef.IsAtEnd())
        {
            labelRef = static_cast<ClassLabelType> (itRef.Get());
            labelProd = static_cast<ClassLabelType> (itInput.Get());

            // Extraction of the reference/produced class labels
            if ((labelRef != nodata) && (labelProd != nodata))
            {
                // If the current labels have not been added to their respective mapOfClasses yet
                if (mapOfClassesRef.count(labelRef) == 0)
                {
                    mapOfClassesRef[labelRef] = itLabelRef;
                    ++itLabelRef;
                }
                if (mapOfClassesProd.count(labelProd) == 0)
                {
                    mapOfClassesProd[labelProd] = itLabelProd;
                    ++itLabelProd;
                }

                // Filling of m_Matrix
                if (m_Matrix[labelRef][labelProd] == 0)
                {
                    m_Matrix[labelRef][labelProd] = 1;
                }
                else
                {
                    m_Matrix[labelRef][labelProd]++;
                }

            } // END if ((labelRef != nodata) && (labelProd != nodata))
            ++itRef;
            ++itInput;
        }
    }

    /////////////////////////////////////////////

    // Filling the 2 headers for the output file
    const std::string commentRefStr = "#Reference labels (rows):";
    const std::string commentProdStr = "#Produced labels (columns):";
    const char separatorChar = ',';
    std::ostringstream ossHeaderRefLabels, ossHeaderProdLabels;

    // Filling ossHeaderRefLabels for the output file
    ossHeaderRefLabels << commentRefStr;
    itMapOfClassesRef = mapOfClassesRef.begin();
    int indexLabelRef = 0;
    while (itMapOfClassesRef != mapOfClassesRef.end())
    {
        // labels labelRef of mapOfClassesRef are already sorted
        labelRef = itMapOfClassesRef->first;

        // SORTING the itMapOfClassesRef->second items of mapOfClassesRef
        mapOfClassesRef[labelRef] = indexLabelRef;
        LINFO("mapOfClassesRef[" << labelRef << "] = " << mapOfClassesRef[labelRef]);

        ossHeaderRefLabels << labelRef;
        ++itMapOfClassesRef;
        if (itMapOfClassesRef != mapOfClassesRef.end())
        {
            ossHeaderRefLabels << separatorChar;
        }
        else
        {
            ossHeaderRefLabels << std::endl;
        }

        ++indexLabelRef;
    }

    // Filling ossHeaderProdLabels for the output file
    ossHeaderProdLabels << commentProdStr;
    itMapOfClassesProd = mapOfClassesProd.begin();
    int indexLabelProd = 0;
    while (itMapOfClassesProd != mapOfClassesProd.end())
    {
        // labels labelProd of mapOfClassesProd are already sorted
        labelProd = itMapOfClassesProd->first;

        // SORTING the itMapOfClassesProd->second items of mapOfClassesProd
        mapOfClassesProd[labelProd] = indexLabelProd;
        LINFO("mapOfClassesProd[" << labelProd << "] = " << mapOfClassesProd[labelProd]);

        ossHeaderProdLabels << labelProd;
        ++itMapOfClassesProd;
        if (itMapOfClassesProd != mapOfClassesProd.end())
        {
            ossHeaderProdLabels << separatorChar;
        }
        else
        {
            ossHeaderProdLabels << std::endl;
        }

        ++indexLabelProd;
    }

    try
    {
        outFile << std::fixed;
        outFile.precision(10);

        /////////////////////////////////////

        // Writing the 2 headers
        outFile << ossHeaderRefLabels.str();
        outFile << ossHeaderProdLabels.str();

        /////////////////////////////////////

        // Initialization of the Confusion Matrix for the application LOG and for measurements
        int nbClassesRef = mapOfClassesRef.size();
        int nbClassesProd = mapOfClassesProd.size();

        // Formatting m_MatrixLOG from m_Matrix in order to make m_MatrixLOG a square matrix
        // from the reference labels in mapOfClassesRef
        indexLabelRef = 0;
        int indexLabelProdInRef = 0;

        // Initialization of m_MatrixLOG
        m_MatrixLOG.SetSize(nbClassesRef, nbClassesRef);
        m_MatrixLOG.Fill(0);
        for (itMapOfClassesRef = mapOfClassesRef.begin(); itMapOfClassesRef != mapOfClassesRef.end(); ++itMapOfClassesRef)
        {
            // labels labelRef of mapOfClassesRef are already sorted
            labelRef = itMapOfClassesRef->first;

            indexLabelProd = 0;
            for (itMapOfClassesProd = mapOfClassesProd.begin(); itMapOfClassesProd != mapOfClassesProd.end(); ++itMapOfClassesProd)
            {
                // labels labelProd of mapOfClassesProd are already sorted
                labelProd = itMapOfClassesProd->first;

                // If labelProd is present in mapOfClassesRef
                if (mapOfClassesRef.count(labelProd) != 0)
                {
                    // Index of labelProd in mapOfClassesRef; itMapOfClassesRef->second elements are now SORTED
                    indexLabelProdInRef = mapOfClassesRef[labelProd];
                    m_MatrixLOG(indexLabelRef, indexLabelProdInRef) = m_Matrix[labelRef][labelProd];
                }

                ///////////////////////////////////////////////////////////

                // Writing the ordered confusion matrix in the output file
                outFile <<  m_Matrix[labelRef][labelProd];
                if (indexLabelProd < (nbClassesProd - 1))
                {
                    outFile << separatorChar;
                }
                else
                {
                    outFile << std::endl;
                }

                ///////////////////////////////////////////////////////////
                ++indexLabelProd;
            }

            m_Matrix[labelRef].clear();
            ++indexLabelRef;
        }

        // m_Matrix is cleared in order to remove old results in case of successive runs of the GUI application
        m_Matrix.clear();
        csvPort_.setData(outFile.str());

        LINFO("Reference class labels ordered according to the rows of the output confusion matrix: " << ossHeaderRefLabels.str());
        LINFO("Produced class labels ordered according to the columns of the output confusion matrix: " << ossHeaderProdLabels.str());
        //LINFO("Output confusion matrix (rows = reference labels, columns = produced labels):\n" << m_MatrixLOG);

    } catch (int e) {
        LERROR("Error with the output text file!");
    }

    LogConfusionMatrix(&mapOfClassesRef, &m_MatrixLOG);

    try
    {
        // Measurements of the Confusion Matrix parameters
        confMatMeasurements->SetMapOfClasses(mapOfClassesRef);
        confMatMeasurements->SetConfusionMatrix(m_MatrixLOG);
        confMatMeasurements->Compute();

        for (itMapOfClassesRef = mapOfClassesRef.begin(); itMapOfClassesRef != mapOfClassesRef.end(); ++itMapOfClassesRef)
        {
            labelRef = itMapOfClassesRef->first;
            indexLabelRef = itMapOfClassesRef->second;

            LINFO("Precision of class [" << labelRef << "] vs all: " << confMatMeasurements->GetPrecisions()[indexLabelRef]);
            LINFO("Recall of class [" << labelRef << "] vs all: " << confMatMeasurements->GetRecalls()[indexLabelRef]);
            LINFO("F-score of class [" << labelRef << "] vs all: " << confMatMeasurements->GetFScores()[indexLabelRef] << std::endl);
        }

        LINFO("Precision of the different classes: " << confMatMeasurements->GetPrecisions());
        LINFO("Recall of the different classes: " << confMatMeasurements->GetRecalls());
        LINFO("F-score of the different classes: " << confMatMeasurements->GetFScores() << std::endl);

        LINFO("Kappa index: " << confMatMeasurements->GetKappaIndex());
        LINFO("Overall accuracy index: " << confMatMeasurements->GetOverallAccuracy());

        LINFO("Compute Confusion Matrix Application Executed Successfully!");

    }
    catch (int e)
    {
        LERROR("Error in Confusion Matrix Measurements filter");
        return;
    }
}

}   // namespace
