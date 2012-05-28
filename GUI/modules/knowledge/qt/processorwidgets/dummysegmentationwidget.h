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

#ifndef VRN_DUMMYSEGMENTATIONWIDGET_H
#define VRN_DUMMYSEGMENTATIONWIDGET_H

#include "modules/knowledge/processors/dummysegmentationprocessor.h"
#include "voreen/qt/widgets/processor/qprocessorwidget.h"
//#include "voreen/qt/widgets/shaderplugin.h"

//#include "tgt/qt/qtcanvas.h"
#include <QLabel>
//#include <QCheckBox>

namespace voreen {

class DummySegmentationWidget : public QProcessorWidget {
    Q_OBJECT
public:
    DummySegmentationWidget(QWidget* parent, DummySegmentationProcessor* dummySegmentationProcessor);
    virtual ~DummySegmentationWidget();

    void initialize();

    virtual void updateFromProcessor();

private:

    static const std::string loggerCat_;

    //ShaderPlugin* plugin_;
};

} // namespace voreen

#endif // VRN_DUMMYSEGMENTATIONWIDGET_H

