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

#ifndef VRN_ONTOLOGYCREATORWIDGET_H
#define VRN_ONTOLOGYCREATORWIDGET_H

#include "modules/knowledge/processors/ontologycreatorprocessor.h"
#include "voreen/qt/widgets/processor/qprocessorwidget.h"
#include "voreen/core/voreencoreapi.h"
#include "voreen/qt/voreenmoduleqt.h"

#include "../models/ontologyclassitem.h"
#include "../models/ontologyclassmodel.h"

#include "../processorwidgets/ontologyclassview.h"
#include "../utils/owlparser.h"
#include "../utils/owlwriter.h"

#include <QLabel>
#include <QString>
#include <QtGui>
#include <QDomDocument>

class QLineEdit;

namespace voreen {

class VRN_QT_API OntologyCreatorWidget : public QProcessorWidget {
    Q_OBJECT
public:
    OntologyCreatorWidget(QWidget*, OntologyCreatorProcessor* );
    virtual ~OntologyCreatorWidget();

    void initialize();

    //!
    //! This function is called whenever a new event is triggered by the OntologyCreatorProcessor
    //! IO functionality and new Ontology functionality is handled..
    //!
    virtual void updateFromProcessor();


private slots:
    void onModelDataChanged(QModelIndex,QModelIndex);

private:
    //!
    //! This is a core functionality
    //! every time the ontolgoy model changes the TextOutport is being updated
    //! in order connected Inports to be updated
    //!
    void setupOutportText();

    static const std::string loggerCat_;

    OntologyClassModel *m_pOntologyClassModel;

    OntologyClassView *m_pOntologyClassView;

    QLineEdit   *m_pLineEditXmlns;
    QLabel      *m_pLabelXmlBase;

    OWLParser m_owlParser;
    OwlWriter m_owlWriter;

};

} // namespace voreen

#endif // VRN_ONTOLOGYCREATORWIDGET_H

