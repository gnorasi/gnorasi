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

#include "commandcontrastenhancementsquareroot.h"

#include "../widgets/vector/itiotbvectorimageviewer.h"
#include "../models/itiotbVectorImageModel.h"
#include "../vector_globaldefs.h"

using namespace itiviewer;
using namespace otb;

CommandContrastEnhancementSquareRoot::CommandContrastEnhancementSquareRoot(ItiOtbVectorImageViewer *viewer, QObject *parent) :
    m_upperQuantile(2.0), m_lowerQuantile(2.0), m_pItiOtbVectorImageViewer(viewer), Command(parent)
{
}

void CommandContrastEnhancementSquareRoot::execute(){
    VectorImageModel *vModel = qobject_cast<VectorImageModel*>(m_pItiOtbVectorImageViewer->model());
    if(!vModel)
        return;

    RenderingFunctionType::Pointer renderer = SquareRootRenderingFunctionType::New();

    if(vModel){
        std::vector<unsigned int> l = vModel->GetChannelList();
        renderer->SetAutoMinMax(false);
        renderer->SetChannelList(l);
        vModel->setRenderingFunction(renderer);
    }

    m_pItiOtbVectorImageViewer->draw();

}
