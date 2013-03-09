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

#ifndef VRN_FUZZYPROCESSORWIDGET_H
#define VRN_FUZZYPROCESSORWIDGET_H

#include <QtCore>
#include <QtGui>

#include "modules/otb/ports/otblabelmapport.h"

#include "../utils/fuzzylabelmapparser.h"

#include "modules/knowledge/processors/fuzzyprocessor.h"
#include "voreen/core/voreencoreapi.h"
#include "voreen/qt/voreenmoduleqt.h"
#include "voreen/qt/widgets/processor/qprocessorwidget.h"


using namespace otb;

namespace voreen {


class VRN_QT_API FuzzyProcessorWidget : public QProcessorWidget
{
    Q_OBJECT
public:
    FuzzyProcessorWidget(QWidget*, FuzzyProcessor* );

    virtual ~FuzzyProcessorWidget();

    /*!
     * \brief initialize
     */
    void initialize();

    /*!
     * \brief updateFromProcessor , implementation
     */
    virtual void updateFromProcessor();

    /*!
     * \brief loggerCat_
     */
    static const std::string loggerCat_;


signals:
    
public slots:
    //!


private slots:
    void calculate();
    void addSelection();
    void removeSelection();


private:

    void setupAvailableTableByList(const QStringList &list);

    FuzzyLabelMapParser::LabelMapType*       getMapFromPort();

    QSpinBox            *m_pSpinBoxMin;
    QSpinBox            *m_pSpinBoxMax;

    QTableView          *m_pTableViewAvailable;
    QTableView          *m_pTableViewSelection;

    QStandardItemModel  *m_pModel;
    QStandardItemModel  *m_pModelSelection;

    QPushButton         *m_pPushButtonAdd;
    QPushButton         *m_pPushButtonRemove;
    QPushButton         *m_pPushButtonCalculate;

};


inline double calculateValue(double val, double a , double b){
    if(val <= a)
        return 0.0;
    else if(val >= b)
        return 1.0;
    else
        return (val -a)/(a-b);
}

}

#endif // VRN_QGLOTBIMAGEVIEWERWIDGET_H
