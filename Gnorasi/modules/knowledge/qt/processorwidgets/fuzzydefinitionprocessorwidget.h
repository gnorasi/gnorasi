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

#ifndef VRN_FUZZYDEFINITIONPROCESSORWIDGET_H
#define VRN_FUZZYDEFINITIONPROCESSORWIDGET_H

#include <QtCore>
#include <QtGui>

#include "modules/otb/ports/otblabelmapport.h"

#include "../utils/fuzzylabelmaputility.h"

#include "modules/knowledge/processors/fuzzydefinitionprocessor.h"
#include "voreen/core/voreencoreapi.h"
#include "voreen/qt/voreenmoduleqt.h"
#include "voreen/qt/widgets/processor/qprocessorwidget.h"


using namespace otb;

namespace voreen {

struct FuzzyRestriction{
    QString fuzzyProperty;
    double val;
    QString opr;
};

class FuzzyRule : public QObject{
    Q_OBJECT
public:
    FuzzyRule(QObject *parent = 0) : QObject(parent) { }

    int id() const { return m_id; }
    void setId(int i) { m_id = i; }

    QString opr() const { return m_opr; }
    void setOpr(const QString &opr) { m_opr = opr; }

    QList<FuzzyRestriction*> restrictions() const { return m_restrictionList; }
    void setRestrictions(const QList<FuzzyRestriction*> rl) { m_restrictionList = rl; }

    void addRestriction(FuzzyRestriction *r) { m_restrictionList.append(r); }
    void removeRestriction(FuzzyRestriction *r) { m_restrictionList.removeOne(r); }

private:
    int     m_id;
    QString m_opr;
    QList<FuzzyRestriction*> m_restrictionList;
};




class VRN_QT_API FuzzyDefinitionProcessorWidget : public QProcessorWidget
{
    Q_OBJECT
public:
    FuzzyDefinitionProcessorWidget(QWidget*, FuzzyDefinitionProcessor* );

    virtual ~FuzzyDefinitionProcessorWidget();

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



private:

//    void setupOntologyClassItems();

//    void setupOntologyComboBoxItems();

    QString getTextOperatorFromSymbol(const QString &text);

    QString constructXmlFile() const;

    void setupOperatorField();

    void setupFuzzyAtributes(FuzzyLabelMapUtility::LabelMapType* , const QString &prepkey);

    FuzzyLabelMapUtility::LabelMapType* getMapFromPort();

    FuzzyLabelMapUtility*               m_pFuzzyLabelMapUtility;

    QComboBox*                          m_pOntologyClassesComboBox;

    QStringList                         m_fuzzyAttributesList;

    QTableView*                         m_pRulesTableView;

    QStandardItemModel*                 m_pRulesModel;

    QRadioButton*                       m_pMinRadioButton;
    QRadioButton*                       m_pMaxRadioButton;

    QComboBox*                          m_pOntologyClassComboBox;

    QList<FuzzyRule*>                   m_fuzzyRuleList;
};

}

Q_DECLARE_METATYPE(FuzzyRestriction)

#endif // VRN_FUZZYDEFINITIONPROCESSORWIDGET_H
