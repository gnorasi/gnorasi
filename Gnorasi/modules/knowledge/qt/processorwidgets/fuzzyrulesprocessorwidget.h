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

#include "modules/knowledge/processors/fuzzyrulesprocessor.h"
#include "voreen/core/voreencoreapi.h"
#include "voreen/qt/voreenmoduleqt.h"
#include "voreen/qt/widgets/processor/qprocessorwidget.h"


using namespace otb;

namespace voreen {

class OWLHelperItem;

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

    QString className() const { return m_className; }
    void setClassName(const QString &cl) { m_className = cl; }

    QList<FuzzyRestriction*> restrictions() const { return m_restrictionList; }
    void setRestrictions(const QList<FuzzyRestriction*> rl) { m_restrictionList = rl; }

    void addRestriction(FuzzyRestriction *r) { m_restrictionList.append(r); }
    void removeRestriction(FuzzyRestriction *r) { m_restrictionList.removeOne(r); }

    FuzzyRestriction* restrictionAt(int index) { if(index >= 0 && index < m_restrictionList.size())return m_restrictionList.at(index); return 0; }

private:
    int     m_id;
    QString m_opr;
    QString m_className;
    QList<FuzzyRestriction*> m_restrictionList;
};




class VRN_QT_API FuzzyRulesProcessorWidget : public QProcessorWidget
{
    Q_OBJECT
public:
    FuzzyRulesProcessorWidget(QWidget*, FuzzyRulesProcessor* );

    virtual ~FuzzyRulesProcessorWidget();

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
    void onAddButtonClicked();
    void onRemoveButtonClicked();

    void onComboboxCurrentIndexChanged(const QString& );

    void onRadioButtonMixMaxChanged();

    void updateOutPortTextData();

    void onModelChanged(QStandardItem* );

private:

    void processOntologyItem(OWLHelperItem *, QStringList &) ;

    void setupWidgetByCurrentRule();

    void setupOntologyClassItems(const QStringList &list);

//    void setupOntologyComboBoxItems();

    QString getTextOperatorFromSymbol(QString &text);

    QString getSymbolOperatorFromText(const QString &text);

    QString constructXmlFile() ;

    void setupOperatorField();

    void setupNameField();

    void setupFuzzyAtributes(FuzzyLabelMapUtility::LabelMapType* , const QString &prepkey);

    FuzzyRule* fuzzyRuleByOntologyClass(const QString &);

    FuzzyLabelMapUtility::LabelMapType* getMapFromPort();

    QStringList                         getOntologyClassesFromPort();

    FuzzyLabelMapUtility*               m_pFuzzyLabelMapUtility;

    QStringList                         m_fuzzyAttributesList;

    QTableView*                         m_pRulesTableView;

    QStandardItemModel*                 m_pRulesModel;

    QRadioButton*                       m_pMinRadioButton;
    QRadioButton*                       m_pMaxRadioButton;

    QPushButton*                        m_pAddPushButton;
    QPushButton*                        m_pRemovePushButton;
    QPushButton*                        m_pExportPushButton;

    QComboBox*                          m_pOntologyClassComboBox;

    QList<FuzzyRule*>                   m_fuzzyRuleList;

    FuzzyRule*                          m_pCurrentRule;
};

}

Q_DECLARE_METATYPE(FuzzyRestriction)

#endif // VRN_FUZZYDEFINITIONPROCESSORWIDGET_H
