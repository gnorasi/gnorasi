#ifndef INSERTEXPRESSIONDIALOG_H
#define INSERTEXPRESSIONDIALOG_H

#include <QDialog>
#include <QListView>
#include <QStandardItemModel>
#include <QPushButton>

class MembershipFunctionDialog;

class ObjectAttributesView;

class InsertExpressionDialog : public QDialog
{
    Q_OBJECT
public:
    explicit InsertExpressionDialog(QWidget *parent = 0);

    void setupByLevel(int );
    
signals:
    
public slots:

private slots:
    void onOkClicked();
    void onCancelClicked();

private:
    void initialize();

    QPushButton *m_pOkButton;
    QPushButton *m_pCancelButton;

    ObjectAttributesView *m_pObjectAttributesView;

    QStandardItemModel *m_pSIM;

    QStandardItem *m_pObjectMapAttributeRootItem;
    QStandardItem *m_pSpatialAttributeRootItem;

    
};

#endif // INSERTEXPRESSIONDIALOG_H
