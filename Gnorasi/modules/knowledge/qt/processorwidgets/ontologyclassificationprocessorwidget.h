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

#ifndef VRN_ONTOLOGYCLASSIFICATIONPROCESSORWIDGET_H
#define VRN_ONTOLOGYCLASSIFICATIONPROCESSORWIDGET_H

#include <QtCore>
#include <QtGui>

#include "../../../otb/ports/otblistlabelmapport.h"

#include "../utils/fuzzylabelmaputility.h"

#include "modules/knowledge/processors/ontologyclassificationprocessor.h"
#include "voreen/core/voreencoreapi.h"
#include "voreen/qt/voreenmoduleqt.h"
#include "voreen/qt/widgets/processor/qprocessorwidget.h"

class ClassHierarchyWidget;

class FuzzyFunctionFactory;

using namespace otb;

class FuzzyOntologyClass;

namespace voreen {

class OWLHelperItem;

class VRN_QT_API OntologyClassificationProcessorWidget : public QProcessorWidget
{
    Q_OBJECT
public:
    OntologyClassificationProcessorWidget(QWidget*, OntologyClassificationProcessor* );

    virtual ~OntologyClassificationProcessorWidget();

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
     * \brief save
     *  Saves the hierrarchy and fuzzy rules data in an external file.
     */
    void save();

    /*!
     * \brief save
     *  Load the hierrarchy and fuzzy rules data from an external file.
     */
    void load();

    /*!
     * \brief createANewAttribute
     *
     *  Create and set a new attribute for the current FuzzyOntologyClass item
     */
//    void createANewAttribute();

    /*!
     * \brief removeSelection
     */
//    void removeSelection();

    /*!
     * \brief onRadioButtonMinMaxChanged
     */
//    void onRadioButtonMinMaxChanged();

    /*!
     * \brief onComboboxCurrentIndexChanged
     */

    /*!
     * \brief onComboboxCurrentIndexChanged
     */
//    void onComboboxCurrentIndexChanged(const QString& );

    /*!
     * \brief onFuzzyAttributeModelChanged
     */
//    void onFuzzyAttributeModelChanged(QStandardItem* );

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
    QString constructXmlFile();


    QString constructCSVText();


    QString getFilePath();

    /*!
     * \brief processOntologyItem
     */
//    void processOntologyItem(OWLHelperItem *, QStringList &) ;

    /*!
     * \brief setupAvailableAttributesListViewByList
     * \param list
     */
//    void setupAvailableAttributesListViewByList(const QStringList &list);

    /*!
     * \brief setupOntologyClassItems
     * \param list
     */
//    void setupOntologyClassItems(const QStringList &list);

    /*!
     * \brief setupItemDelegates
     */
//    void setupItemDelegates();

    /*!
     * \brief setupWidgetByCurrentOntologyClass
     */
//    void setupWidgetByCurrentOntologyClass();


    /*!
     * \brief getMapFromPort
     * \return
     */
    void                                    processPortData();


    void                                    processAttributeList(int, const QStringList &);

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
    ClassHierarchyWidget                    *m_pClassHierarchyWidget;
    QPushButton                             *m_pPushButtonCalculate;
    QPushButton                             *m_pPushButtonSave;
    QPushButton                             *m_pPushButtonLoad;

};

}

#endif // VRN_ONTOLOGYCLASSIFICATIONPROCESSORWIDGET_H
