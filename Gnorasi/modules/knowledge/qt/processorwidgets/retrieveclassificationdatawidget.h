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

#ifndef VRN_RETRIEVECLASSIFICATIONDATAWIDGET_H
#define VRN_RETRIEVECLASSIFICATIONDATAWIDGET_H

#include "modules/knowledge/processors/retrieveclassificationdataprocessor.h"
#include "voreen/qt/widgets/processor/qprocessorwidget.h"

#include <QLabel>
#include <QString>
#include <QtGui>
#include <QDomDocument>

namespace voreen {

class RetrieveClassificationDataWidget : public QProcessorWidget {
    Q_OBJECT
public:
    RetrieveClassificationDataWidget(QWidget* parent, RetrieveClassificationDataProcessor* retrieveClassificationDataProcessor);
    virtual ~RetrieveClassificationDataWidget();

    void initialize();

    virtual void updateFromProcessor();
    virtual std::string invokeWebService(std::string input);
    virtual QString parseResults(QString resultString);

private:
    static const std::string loggerCat_;
    QPlainTextEdit* editor_;
};

} // namespace voreen

#endif // VRN_RETRIEVECLASSIFICATIONDATAWIDGET_H

