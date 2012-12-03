#ifndef ITIOTBIMAGEVIEWERPANELHISTOGRAMTAB_H
#define ITIOTBIMAGEVIEWERPANELHISTOGRAMTAB_H

#include <QWidget>

namespace itiviewer{

/*!
 * \brief The ItiOtbImageViewerPanelHistogramTab class
 */
class ItiOtbImageViewerPanelHistogramTab : public QWidget
{
    Q_OBJECT
public:
    //! ctor
    explicit ItiOtbImageViewerPanelHistogramTab(QWidget *parent = 0);


    
signals:
    
public slots:

private:
    void initialize();


    
};

}

#endif // ITIOTBIMAGEVIEWERPANELHISTOGRAMTAB_H
