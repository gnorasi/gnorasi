/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) Ioannis Tsampoulatidis <itsam@iti.gr>. All rights reserved. 	*
 * Copyright (c) Informatics and Telematics Institute                           *
 *	  Centre for Research and Technology Hellas. All rights reserved.           *
 * Copyright (c) National Technical University of Athens. All rights reserved.	*
 *                                                                              *
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

#include <QtCore>
#include <QtGui>

#include "commandcontrastenhancementgaussian.h"

#include "../widgets/vector/itiotbvectorimageviewer.h"
#include "../models/itiotbVectorImageModel.h"
#include "../vector_globaldefs.h"

using namespace itiviewer;
using namespace otb;

CommandContrastEnhancementGaussian::CommandContrastEnhancementGaussian(ItiOtbVectorImageViewer *viewer, QObject *parent) :
    m_deviation(1.0), m_pItiOtbVectorImageViewer(viewer), Command(parent)
{
}

void CommandContrastEnhancementGaussian::execute(){

    VectorImageModel *vModel = qobject_cast<VectorImageModel*>(m_pItiOtbVectorImageViewer->model());
    if(!vModel || !vModel->buffer())
        return;

    RenderingFunctionType::Pointer renderer;
    renderer = GaussianRenderingFunctionType::New();

    if(vModel){
        std::vector<unsigned int> l = vModel->GetChannelList();
        renderer->SetChannelList(l);
        renderer->SetAutoMinMax(false);

        ReaderType::Pointer reader = ReaderType::New();
        reader->SetFileName(vModel->lastPath().toLatin1().data());
        reader->Update();

        SampleListType::Pointer sampleList = SampleListType::New();
        sampleList->SetMeasurementVectorSize(reader->GetOutput()->GetVectorLength());

        itk::ImageRegionIterator<VectorImageType> imgIter (reader->GetOutput(),
                                                   reader->GetOutput()->
                                                   GetBufferedRegion());
        imgIter.GoToBegin();

        itk::ImageRegionIterator<VectorImageType> imgIterEnd (reader->GetOutput(),
                                                      reader->GetOutput()->
                                                      GetBufferedRegion());

        do
        {
            sampleList->PushBack(imgIter.Get());
            ++imgIter;
        }
        while (imgIter != imgIterEnd);

        renderer->SetListSample(sampleList);

        vModel->setRenderingFunction(renderer);

        vModel->resetData();

        const DefaultImageType *img = vModel->GetOutput(0);

        renderer->Initialize(img->GetMetaDataDictionary());
    }

//    m_pItiOtbVectorImageViewer->draw();
}
