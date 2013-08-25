#ifndef SELECTOPERATORDIALOG_H
#define SELECTOPERATORDIALOG_H

#include <QDialog>

#include <QStandardItemModel>
#include <QTreeView>

#include <QPushButton>

class SelectOperatorDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SelectOperatorDialog(QWidget *parent = 0);

    QString selectedOperator() const;
    
signals:
    
public slots:

private slots:
    void onOkClicked();
    void onCancelCLicked();
    

private:
    void setupData();

    QStandardItemModel *m_pModel;
    QTreeView *m_pTreeView;

    QPushButton *m_pOkbutton;
    QPushButton *m_pCancelbutton;
};

#endif // SELECTOPERATORDIALOG_H
