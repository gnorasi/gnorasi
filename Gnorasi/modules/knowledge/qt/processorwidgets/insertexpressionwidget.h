#ifndef INSERTEXPRESSIONWIDGET_H
#define INSERTEXPRESSIONWIDGET_H

#include <QWidget>
#include <QListView>

class InsertExpressionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit InsertExpressionWidget(QWidget *parent = 0);
    
signals:
    
public slots:

private:
    QListView *m_pAvailableExpressionsListView;
    
};

#endif // INSERTEXPRESSIONWIDGET_H
