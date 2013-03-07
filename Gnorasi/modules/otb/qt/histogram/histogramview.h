#ifndef HISTOGRAMVIEW_H
#define HISTOGRAMVIEW_H

#include <QWidget>

#include "qwt_plot.h"

class QwtPlotCurve;
class QwtPlotItem;
class QwtPlotHistogram;
class Histogram;
class QwtPlotPanner;
class QwtPlotMagnifier;

class HistogramView : public QwtPlot
{
    Q_OBJECT
    Q_PROPERTY(QColor color             READ color          WRITE setColor          NOTIFY colorChanged)
    Q_PROPERTY(QString histogramTitle   READ histogramTitle WRITE setHistogramTitle NOTIFY histogramTitleChanged)

public:
    explicit HistogramView(QWidget *parent = 0);

    /*!
     * \brief setupData
     * \param data
     */
    void setupData(const QHash<int,double> &data);

    /*!
     * \brief color
     * \return
     */
    QColor color() const { return m_color; }
    void setColor(const QColor &c);


    QString histogramTitle() const { return m_histogramTitle; }
    void setHistogramTitle(const QString &t);
    

signals:
    void colorChanged();
    void histogramTitleChanged();
    
public slots:

public slots:
    void showItem(QwtPlotItem *item, bool on);

private:

    void initialize();

    void rescale();

//    Histogram *m_pHistogram;

    QString m_histogramTitle;

    QColor m_color;

    QwtPlotPanner       *m_pPanner;
    QwtPlotMagnifier    *m_pMagnifier;

    const QRectF d_mapRect;

};

#endif // HISTOGRAMVIEW_H
