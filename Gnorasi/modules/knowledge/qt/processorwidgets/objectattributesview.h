#ifndef OBJECTATTRIBUTESVIEW_H
#define OBJECTATTRIBUTESVIEW_H

#include <QTreeView>
#include <QMouseEvent>

class ObjectAttributesView : public QTreeView
{
    Q_OBJECT
public:
    explicit ObjectAttributesView(QWidget *parent = 0);
    
    int levelId() const {return m_levelId; }
    void setLeveLId(int l) { m_levelId = l; }

protected:
    void contextMenuEvent(QContextMenuEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

signals:
    
public slots:

private:
    int m_levelId;
    

};

#endif // OBJECTATTRIBUTESVIEW_H
