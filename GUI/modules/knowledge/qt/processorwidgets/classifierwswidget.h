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

#ifndef VRN_CLASSIFIERWSWIDGET_H
#define VRN_CLASSIFIERWSWIDGET_H

#include "modules/knowledge/processors/classifierwsprocessor.h"
#include "voreen/qt/widgets/processor/qprocessorwidget.h"

#include <QLabel>
#include <QString>
#include <QtGui>

namespace voreen {

class ClassifierWSWidget : public QProcessorWidget {
    Q_OBJECT
public:
    ClassifierWSWidget(QWidget* parent, ClassifierWSProcessor* classifierWSProcessor);
    virtual ~ClassifierWSWidget();

    void initialize();

    virtual void updateFromProcessor();
    virtual std::string invokeWebService(std::string input);

private:
    static const std::string loggerCat_;
    QPlainTextEdit* editor_;
};

} // namespace voreen

#endif // VRN_CLASSIFIERWSWIDGET_H

