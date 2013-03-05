#include "histogramview.h"

#include "qwt_plot_curve.h"
#include "qwt_plot_grid.h"
#include <qwt_plot_layout.h>
#include "qwt_column_symbol.h"
#include "qwt_plot_histogram.h"
#include "qwt_series_data.h"
#include <qwt_legend.h>
#include <qwt_legend_item.h>

#include <QPen>
#include <QColor>


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
    QwtPlot(parent)
{
    initialize();
}


void HistogramView::initialize(){

    setTitle("Histogram");

    plotLayout()->setAlignCanvasToScales(true);

    m_pRedCurve = new QwtPlotCurve(QString("Red Channel"));
    m_pRedCurve->setPen(QPen(Qt::red));
    m_pRedCurve->attach(this);

    m_pGreenCurve = new QwtPlotCurve(QString("Green Channel"));
    m_pGreenCurve->setPen(QPen(Qt::green));
    m_pGreenCurve->attach(this);

    m_pBlueCurve = new QwtPlotCurve(QString("Blue Channel"));
    m_pBlueCurve->setPen(QPen(Qt::blue));
    m_pBlueCurve->attach(this);

    // grid
    QwtPlotGrid *grid = new QwtPlotGrid;
    grid->enableX(false);
    grid->enableY(true);
    grid->enableXMin(false);
    grid->enableYMin(false);
    grid->setMajPen(QPen(Qt::black, 0, Qt::DotLine));
    grid->attach(this);

    // axes
    setAxisTitle(QwtPlot::xBottom, "Pixel Value");
    setAxisTitle(QwtPlot::yLeft, "Frequency");

    QwtLegend *legend = new QwtLegend;
    legend->setItemMode(QwtLegend::CheckableItem);
    insertLegend(legend, QwtPlot::RightLegend);

    connect(this, SIGNAL(legendChecked(QwtPlotItem *, bool)),
        SLOT(showItem(QwtPlotItem *, bool)));

    replot(); // creating the legend items

    QwtPlotItemList items = itemList(QwtPlotItem::Rtti_PlotHistogram);
    for ( int i = 0; i < items.size(); i++ )
    {
        if ( i == 0 )
        {
            QwtLegendItem *legendItem = (QwtLegendItem *)legend->find(items[i]);
            if ( legendItem )
                legendItem->setChecked(true);
            items[i]->setVisible(true);
        }
        else
            items[i]->setVisible(false);
    }

    setAutoReplot(true);
}

void HistogramView::showItem(QwtPlotItem *item, bool on)
{
    item->setVisible(on);
}


void HistogramView::setupData(){
}


void HistogramView::setupRedChannel(int sd, const QHash<int,double> &data){

    double helperVal[255];
    double helperVal2[255];
    QHash<int,double>::const_iterator i;
    int counter = 0;
    for(i = data.constBegin(); i != data.constEnd(); i++){
        helperVal2[counter] = i.key();
        helperVal[counter++] = i.value();
    }

    m_pRedCurve->setSamples(helperVal2,helperVal,255);

//    Histogram *histogramRedChannel = new Histogram("Red Channel", Qt::red);
//    histogramRedChannel->setValues(255, helperVal);
//    histogramRedChannel->attach(this);
}


void HistogramView::setupGreenChannel(int sd, const QHash<int,double> &data){
    double helperVal[255];
    double helperVal2[255];
    QHash<int,double>::const_iterator i;
    int counter = 0;
    for(i = data.constBegin(); i != data.constEnd(); i++){
        helperVal2[counter] = i.key();
        helperVal[counter++] = i.value();
    }

    m_pGreenCurve->setSamples(helperVal2,helperVal,255);

//    Histogram *histogramRedChannel = new Histogram("Green Channel", Qt::green);
//    histogramRedChannel->setValues(data.size(), helperVal);
//    histogramRedChannel->attach(this);
}


void HistogramView::setupBlueChannel(int sd, const QHash<int,double> &data){
    double helperVal[255];
    double helperVal2[255];
    QHash<int,double>::const_iterator i;
    int counter = 0;
    for(i = data.constBegin(); i != data.constEnd(); i++){
        helperVal2[counter] = i.key();
        helperVal[counter++] = i.value();
    }

    m_pBlueCurve->setSamples(helperVal2,helperVal,255);

//    Histogram *histogramRedChannel = new Histogram("Green Channel", Qt::blue);
//    histogramRedChannel->setValues(data.size(), helperVal);
//    histogramRedChannel->attach(this);
}
