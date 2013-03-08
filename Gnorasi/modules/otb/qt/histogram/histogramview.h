/********************************************************************************
 *                                                                              *
 * GNORASI - The Knowlegde-Based Remote Sensing Engine                          *
 *                                                                              *
 * Language:  C++                                                               *
 *                                                                              *
 * Copyright (c) Ioannis Tsampoulatidis <itsam@iti.gr>. All rights reserved. 	*
 * Copyright (c) Informatics and Telematics Institute                           *
 *	  Centre for Research and Technology Hellas. All rights reserved.           *
 * Copyright (c) National Technical University of Athens. All rights reserved.	*
 *                                                                              *
 *                                                                              *
 * This file is part of the GNORASI software package. GNORASI is free           *
 * software: you can redistribute it and/or modify it under the terms           *
 * of the GNU General Public License version 2 as published by the              *
 * Free Software Foundation.                                                    *
 *                                                                              *
 * GNORASI is distributed in the hope that it will be useful,                   *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of               *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                 *
 * GNU General Public License for more details.                                 *
 *                                                                              *
 * You should have received a copy of the GNU General Public License            *
 * in the file "LICENSE.txt" along with this program.                           *
 * If not, see <http://www.gnu.org/licenses/>.                                  *
 *                                                                              *
 ********************************************************************************/

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

    QString m_histogramTitle;

    QColor m_color;

    QwtPlotPanner       *m_pPanner;
    QwtPlotMagnifier    *m_pMagnifier;

    const QRectF d_mapRect;

};

#endif // HISTOGRAMVIEW_H
