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

    bool checkForRules(const QString &);

    virtual void updateFromProcessor();

    OwlWriter owlWriter() { return m_owlWriter; }
    OWLParser owlParser() { return m_owlParser; }

private slots:
    void onModelDataChanged(QModelIndex,QModelIndex);

private:
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

