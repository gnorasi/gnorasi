#ifndef VRN_GEOSPATIALCLASSIFICATIONWIDGET_H
#define VRN_GEOSPATIALCLASSIFICATIONWIDGET_H

#include "modules/knowledge/processors/geospatialclassificationprocessor.h"
#include "voreen/qt/widgets/processor/qprocessorwidget.h"
#include "voreen/core/voreencoreapi.h"
#include "voreen/qt/voreenmoduleqt.h"

#include "../models/ontologyclassitem.h"
#include "../models/ontologyclassmodel.h"

#include "../processorwidgets/rulewidget.h"
#include "../processorwidgets/ontologyclassview.h"

#include "../utils/owlparser.h"
#include "../utils/rulewriter.h"
#include "../utils/ruleparser.h"

#include <QLabel>
#include <QString>
#include <QtGui>

namespace voreen {

//!
//! This is the main widget of the GeospatialClassificationProcessor
//!
class VRN_QT_API GeoSpatialClassificationWidget : public QProcessorWidget
{
    Q_OBJECT
public:
    GeoSpatialClassificationWidget(QWidget*, GeospatialClassificationProcessor* );

    virtual ~GeoSpatialClassificationWidget() { }

    void initialize();

    virtual void updateFromProcessor();
    
signals:
    
public slots:
    //!
    void onTreeViewDoubleClicked(const QModelIndex &);

private slots:
    //!
    void onRuleWidgetJChanged();

private:
    //! Validates the items stored in the RuleUtility and the OntologyClassModel items
    void validateContent();

    //! Setup property features , a property is name for a spatial function ex. contains
    void setupPropertyFeatures();

    //! This is a helper class
    //! Segmentation levels should be loaded from a file better
    void setupLevels();

    static const std::string loggerCat_;

    //!
    OntologyClassModel *m_pOntologyClassModel;

    //!
    OntologyClassView *m_pOntologyClassView;

    //! This is the Rule widget
    RuleWidget *m_pRuleWidget;

    //! a parser for *owl documents
    OWLParser m_owlParser;

    //! a parse for xml documents holding rules data
    RuleParser m_ruleParser;

    //! a writer for storing Rules content to xml an document
    RuleWriter m_ruleWriter;
    
};

}

#endif // VRN_GEOSPATIALCLASSIFICATIONWIDGET_H
