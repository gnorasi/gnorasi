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

#include "../../../otb/ports/otblabelmapport.h"

#include "../utils/fuzzylabelmaputility.h"

#include "modules/knowledge/processors/fuzzyprocessor.h"
#include "voreen/core/voreencoreapi.h"
#include "voreen/qt/voreenmoduleqt.h"
#include "voreen/qt/widgets/processor/qprocessorwidget.h"

class FuzzyFunctionFactory;

using namespace otb;

class FuzzyOntologyClass;

namespace voreen {

class OWLHelperItem;

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


    static QStringList parameterCharacters;


signals:
    
public slots:
    //!


private slots:
    /*!
     * \brief calculate
     */
    void calculate();

    /*!
     * \brief addSelection
     */
    void addSelection();

    /*!
     * \brief removeSelection
     */
    void removeSelection();

    /*!
     * \brief onRadioButtonMinMaxChanged
     */
    void onRadioButtonMinMaxChanged();

    /*!
     * \brief onComboboxCurrentIndexChanged
     */

    /*!
     * \brief onComboboxCurrentIndexChanged
     */
    void onComboboxCurrentIndexChanged(const QString& );

    /*!
     * \brief onFuzzyAttributeModelChanged
     */
    void onFuzzyAttributeModelChanged(QStandardItem* );

    /*!
     * \brief updateOutPortTextData
     */
    void updateOutPortTextData();


private:

    /*!
     * \brief constructXmlFile
     * \return
     */
    QString constructXmlFile() ;

    /*!
     * \brief processOntologyItem
     */
    void processOntologyItem(OWLHelperItem *, QStringList &) ;

    /*!
     * \brief setupAvailableAttributesListViewByList
     * \param list
     */
    void setupAvailableAttributesListViewByList(const QStringList &list);

    /*!
     * \brief setupOntologyClassItems
     * \param list
     */
    void setupOntologyClassItems(const QStringList &list);

    /*!
     * \brief setupItemDelegates
     */
    void setupItemDelegates();

    /*!
     * \brief setupWidgetByCurrentOntologyClass
     */
    void setupWidgetByCurrentOntologyClass();


    QStringList                             getOntologyClassesFromPort();


    FuzzyLabelMapUtility::LabelMapType*     getMapFromPort();

    FuzzyLabelMapUtility                    *m_pFuzzyLabelMapUtility;

    QListView                               *m_pAvailableAttributesListView;
    QTableView                              *m_pFuzzyAttributesTableView;

    QStandardItemModel                      *m_pAvailableAttributesModel;
    QStandardItemModel                      *m_pFuzzyAttributesModel;

    QPushButton                             *m_pPushButtonAdd;
    QPushButton                             *m_pPushButtonRemove;
    QPushButton                             *m_pPushButtonCalculate;

    QRadioButton*                           m_pMinRadioButton;
    QRadioButton*                           m_pMaxRadioButton;

    QComboBox*                              m_pOntologyClassComboBox;

    FuzzyFunctionFactory                    *m_pFuzzuFunctionFactory;

    QStringList                             m_fuzzyAttributesList;


    FuzzyOntologyClass                      *m_pCurrentFuzzyOntologyClass;
};

}

#endif // VRN_QGLOTBIMAGEVIEWERWIDGET_H
