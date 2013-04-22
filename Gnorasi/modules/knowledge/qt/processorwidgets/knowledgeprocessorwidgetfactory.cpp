/********************************************************************************
 *                                                                    		*
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                		*
 * 								      		*
 * Language:  C++						      		*
 * 										*
 * Copyright (c) Ioannis Tsampoulatidis <itsam@iti.gr>. All rights reserved. 	*
 * Copyright (c) Informatics and Telematics Institute				*
 *	  Centre for Research and Technology Hellas. All rights reserved.	*
 * Copyright (c) National Technical University of Athens. All rights reserved.	*
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

#include "knowledgeprocessorwidgetfactory.h"

#include "../../processors/classifierwsprocessor.h"
#include "classifierwswidget.h"

#include "../../processors/retrieveclassificationdataprocessor.h"
#include "retrieveclassificationdatawidget.h"

#include "../../processors/ontologycreatorprocessor.h"
#include "ontologycreatorwidget.h"

#include "../../processors/geospatialclassificationprocessor.h"
#include "geospatialclassificationwidget.h"

//#include "../../processors/fuzzypreprocessor.h"
//#include "fuzzypreprocessorwidget.h"

//#include "../../processors/fuzzyrulesprocessor.h"
//#include "fuzzyrulesprocessorwidget.h"

#include "voreen/qt/voreenapplicationqt.h"
#include <QWidget>
#include <QMainWindow>

namespace voreen {

ProcessorWidget* KnowledgeProcessorWidgetFactory::createWidget(Processor* processor) const {

    if (!VoreenApplicationQt::qtApp()) {
        LERRORC("voreen.KnowledgeProcessorWidgetFactory", "VoreenApplicationQt not instantiated");
        return 0;
    }
    QWidget* parent = VoreenApplicationQt::qtApp()->getMainWindow();

    if (dynamic_cast<RetrieveClassificationDataProcessor*>(processor))
        return new RetrieveClassificationDataWidget(parent, static_cast<RetrieveClassificationDataProcessor*>(processor));

    if (dynamic_cast<ClassifierWSProcessor*>(processor))
        return new ClassifierWSWidget(parent, static_cast<ClassifierWSProcessor*>(processor));

    if (dynamic_cast<OntologyCreatorProcessor*>(processor))
        return new OntologyCreatorWidget(parent, static_cast<OntologyCreatorProcessor*>(processor));

    if(dynamic_cast<GeospatialClassificationProcessor*>(processor))
        return new GeoSpatialClassificationWidget(parent, static_cast<GeospatialClassificationProcessor*>(processor));

//    if(dynamic_cast<FuzzyPreProcessor*>(processor))
//        return new FuzzyPreProcessorWidget(parent, static_cast<FuzzyPreProcessor*>(processor));

//    if(dynamic_cast<FuzzyRulesProcessor*>(processor))
//        return new FuzzyRulesProcessorWidget(parent,static_cast<FuzzyRulesProcessor*>(processor));

    return 0;
}
} // namespace voreen
