#ifndef HISTOGRAMVIEW_H
#define HISTOGRAMVIEW_H

#include <QWidget>

#include "qwt_plot.h"

class QwtPlotCurve;
class QwtPlotItem;
class QwtPlotHistogram;

class HistogramView : public QwtPlot
{
    Q_OBJECT
public:
    explicit HistogramView(QWidget *parent = 0);

    void setupData();

    /*!
     * \brief setupRGBMode
     */
    void setupRGBMode();

    /*!
     * \brief setupGreyScaleMode
     */
    void setupGreyScaleMode();

    /*!
     * \brief setupGreyChannel
     * \param data
     */
    void setupGreyChannel(const QHash<int,double> &data);

    /*!
     * \brief setupRedChannel
     * \param data
     */
    void setupRedChannel(const QHash<int,double> &data);

    /*!
     * \brief setupGreenChannel
     * \param data
     */
    void setupGreenChannel(const QHash<int,double> &data);

    /*!
     * \brief setupBlueChannel
     * \param data
     */
    void setupBlueChannel(const QHash<int,double> &data);
    
signals:
    
public slots:

public slots:
    void showItem(QwtPlotItem *item, bool on);

private:

    void initialize();

    QwtPlotCurve *m_pGreyscaleCurve;
    QwtPlotCurve *m_pRedCurve;
    QwtPlotCurve *m_pGreenCurve;
    QwtPlotCurve *m_pBlueCurve;

//    QwtPlotHistogram *m_pistogram;

};

#endif // HISTOGRAMVIEW_H
