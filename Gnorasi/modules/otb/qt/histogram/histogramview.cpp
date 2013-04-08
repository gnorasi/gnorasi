#include "histogramview.h"

#include "qwt_plot_curve.h"
#include "qwt_plot_grid.h"
#include <qwt_plot_layout.h>
#include "qwt_column_symbol.h"
#include "qwt_plot_histogram.h"
#include "qwt_series_data.h"
#include <qwt_legend.h>
#include <qwt_legend_item.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_canvas.h>

#include <QPen>
#include <QColor>

#define BINS_OVERSAMPLING_RATE_DOUBLE 5.0
#define BINS_OVERSAMPLING_RATE_INT 5


class Histogram: public QwtPlotHistogram
{
public:
    Histogram(const QString &, const QColor &);

    void setColor(const QColor &);
    void setValues(uint numValues, const double *);
};

Histogram::Histogram(const QString &title, const QColor &symbolColor):
    QwtPlotHistogram(title)
{
    setStyle(QwtPlotHistogram::Columns);

    setColor(symbolColor);
}

void Histogram::setColor(const QColor &symbolColor)
{
    QColor color = symbolColor;
    color.setAlpha(180);

    setPen(QPen(Qt::black));
    setBrush(QBrush(color));

    QwtColumnSymbol *symbol = new QwtColumnSymbol(QwtColumnSymbol::Box);
    symbol->setFrameStyle(QwtColumnSymbol::Raised);
    symbol->setLineWidth(2);
    symbol->setPalette(QPalette(color));
    setSymbol(symbol);
}

void Histogram::setValues(uint numValues, const double *values)
{
    QVector<QwtIntervalSample> samples(numValues);
    for ( uint i = 0; i < numValues; i++ )
    {
        QwtInterval interval(double(i), i + 1.0);
        interval.setBorderFlags(QwtInterval::ExcludeMaximum);

        samples[i] = QwtIntervalSample(values[i], interval);
    }

    setData(new QwtIntervalSeriesData(samples));
}


HistogramView::HistogramView(QWidget *parent) :
    d_mapRect(0.0, 0.0, BINS_OVERSAMPLING_RATE_DOUBLE*256.0, BINS_OVERSAMPLING_RATE_DOUBLE*256),
    QwtPlot(parent)
{
    initialize();
}


void HistogramView::initialize(){

    setTitle(tr("Histogram"));

    setCanvasBackground(QColor("#EBEBEB"));

    m_color = QColor(Qt::red);
    m_histogramTitle = tr("Red Channel");

    /*
      Navigation:

      Left Mouse Button: Panning
      Mouse Wheel:       Zooming In/Out
      Right Mouse Button: Reset to initial
    */

    m_pPanner = new QwtPlotPanner(canvas());
    m_pMagnifier = new QwtPlotMagnifier(canvas());

    canvas()->setFocusPolicy(Qt::WheelFocus);
    rescale();

    plotLayout()->setAlignCanvasToScales(true);

    setAxisMaxMinor(QwtPlot::xBottom, 0);
    setAxisMaxMinor(QwtPlot::yLeft, 0);

    canvas()->setBorderRadius( 10 );

    // grid
    QwtPlotGrid *grid = new QwtPlotGrid;
    grid->enableX(false);
    grid->enableY(true);
    grid->enableXMin(false);
    grid->enableYMin(false);
    grid->setMajPen(QPen(Qt::black, 0, Qt::DotLine));
    grid->attach(this);

    // axes
    setAxisTitle(QwtPlot::xBottom, tr("Intensity"));

    setAutoReplot(true);
}


void HistogramView::showItem(QwtPlotItem *item, bool on)
{
    item->setVisible(on);
}


void HistogramView::setupData(const QHash<int,double> &data){
    setAutoReplot(false);
    m_pPanner->setEnabled(false);
    m_pMagnifier->setEnabled(false);

    detachItems();

    double helperVal[BINS_OVERSAMPLING_RATE_INT*256];
    double helperVal2[BINS_OVERSAMPLING_RATE_INT*256];
    QHash<int,double>::const_iterator i;
    int counter = 0;
    for(i = data.constBegin(); i != data.constEnd(); i++){
        helperVal2[counter] = i.key();
        helperVal[counter++] = i.value();
    }

    Histogram *histogram = new Histogram(m_histogramTitle,m_color);
    histogram->attach(this);
    histogram->setValues(BINS_OVERSAMPLING_RATE_INT*256, helperVal);

    rescale();

    setAutoReplot(true);
    m_pPanner->setEnabled(true);
    m_pMagnifier->setEnabled(true);
}


void HistogramView::setHistogramTitle(const QString &t){
    m_histogramTitle = t;

    this->setTitle(t);
}

void HistogramView::setColor(const QColor &c){
    m_color = c;

}


void HistogramView::rescale(){
    setAxisScale(QwtPlot::xBottom,
        d_mapRect.left(), d_mapRect.right());
//    setAxisScale(QwtPlot::yLeft,
//        d_mapRect.top(), d_mapRect.bottom());
}
