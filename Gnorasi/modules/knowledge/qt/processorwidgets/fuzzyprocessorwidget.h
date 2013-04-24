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


signals:
    
public slots:
    //!


private slots:
    /*!
     * \brief calculate
     *  This is were the calculation is done. What it is actually done is to iterate over the
     *  label map and the attributes set and calculate a value for each attribute based on
     *  the function and the parameters set.
     */
    void calculate();

    /*!
     * \brief createANewAttribute
     *
     *  Create and set a new attribute for the current FuzzyOntologyClass item
     */
    void createANewAttribute();

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
     *  What is being done here is to create a new QDomDocument and take the text from it.
     *  The structure of this document has been prototyped by Babis . ask him for more info.
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


    /*!
     * \brief getOntologyClassesFromPort
     *  This is a helper function
     * \return
     */
    QStringList                             getOntologyClassesFromPort();

    /*!
     * \brief getMapFromPort
     * \return
     */
    FuzzyLabelMapUtility::LabelMapType*     getMapFromPort();

    /*!
     * \brief m_pFuzzyLabelMapUtility
     */
    FuzzyLabelMapUtility                    *m_pFuzzyLabelMapUtility;

    /*!
     * \brief m_pFuzzyFunctionFactory
     */
    FuzzyFunctionFactory                    *m_pFuzzyFunctionFactory;

    /*!
     * \brief m_pCurrentFuzzyOntologyClass
     */
    FuzzyOntologyClass                      *m_pCurrentFuzzyOntologyClass;

    // helper list
    QStringList                             m_fuzzyAttributesList;


    // Qt Widgets
    QListView                               *m_pAvailableAttributesListView;
    QTableView                              *m_pFuzzyAttributesTableView;

    QPushButton                             *m_pPushButtonAdd;
    QPushButton                             *m_pPushButtonRemove;
    QPushButton                             *m_pPushButtonCalculate;

    QRadioButton*                           m_pMinRadioButton;
    QRadioButton*                           m_pMaxRadioButton;

    QComboBox*                              m_pOntologyClassComboBox;


    // the two models
    QStandardItemModel                      *m_pAvailableAttributesModel;
    QStandardItemModel                      *m_pFuzzyAttributesModel;
};

}

#endif // VRN_QGLOTBIMAGEVIEWERWIDGET_H
