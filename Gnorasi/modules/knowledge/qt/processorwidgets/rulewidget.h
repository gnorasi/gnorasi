#ifndef RULEWIDGET_H
#define RULEWIDGET_H

#include <QWidget>

#include <QItemSelectionModel>

class QLabel;
class QLineEdit;
class QComboBox;
class QDoubleSpinBox;
class QPushButton;
class QStandardItemModel;
class QTableWidget;
class QStandardItem;
class QListView;
class QModelIndex;

class RuleItemView;
class Rule;
class OntologyClassItem;
class ComboBoxDelegate;


//!
//! This is where all the handling of Rule items is done
//! An interface for adding edititng and deleting new Rules is provided
//!
class RuleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RuleWidget(const QString &ocid = QString(), QWidget *parent = 0);

    virtual ~RuleWidget();

    //! setups this widget by the OntologyClassItem parameter and a list
    //! of the available ontology class item headers
    void setupByOntologyClassItem(OntologyClassItem*, const QStringList &);

    //! sets the number of feature properties
    void setFeaturePropertyList(const QStringList &list);

    //! sets the number of levels
    void setLevelList(const QStringList &list);

    //! sets the list of ontology classes
    void setOntologyClassList(const QStringList &list);
    
signals:
    void dataChanged();
    
public slots:

private slots:

    //! adds a new Rule item
    void onAddNewRuleClicked();

    //!
    void onDeleteRuleClicked();

    //! when an item in the list is clicked
    void onRulesListViewSelectionChanged(const QItemSelection& ,const QItemSelection& );

    //! update the Rule item when the rule name - id changes
    void onRulesListViewChanged(QStandardItem*);

    //! update the the Rule items handled by the RuleUtility
    void onRuleItemJChanged(QStandardItem*);

    //! update the confidence value when the spinbox changes value
    void onSpinBoxJChanged(double );

private:
    //! initializing stuff
    void initialize();

    //! setup the widget by the Rule parameter
    void setupRuleItemViewByRule(Rule *);
    
    //! the name of the class that this rule is applied to , namely the head class
    QLabel *m_pLabelClassName;

    //! a spinbox
    QDoubleSpinBox *m_pSpinBoxConfidence;

    //! a drop box containing all the rules
    QListView *m_pListViewRules;

    //! create a rule
    QPushButton *m_pPushButtonNewRule;

    //! delete a rule
    QPushButton *m_pPushButtonDeleteRule;

    //! This is the rule item model
    QStandardItemModel *m_pRuleItemModel;

    //! this is the available rules model
    QStandardItemModel *m_pRulesModel;

    //! This is the rule view which contains rule items
    RuleItemView *m_pRuleItemView;

    //! This is the ontology class that the rules contained in this widget refer to
    QString m_ontologyClassId;

    //! holds a list of the available OntologyClassItem headers
    QStringList m_ontologyClassIdsList;

    //! holds the list of the availabel property values
    QStringList m_propertyFeatureList;
};

#endif // RULEWIDGET_H
