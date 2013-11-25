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

#ifndef ITIOTBIMAGEVIEWERPANELSETUPDATATAB_H
#define ITIOTBIMAGEVIEWERPANELSETUPDATATAB_H

#include <QWidget>
#include <QGroupBox>
#include <QRadioButton>
#include <QSpinBox>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QTreeView>
#include <QStandardItemModel>
#include <QTextEdit>

namespace itiviewer{

class ItiOtbImageViewerPanel;
class ItiOtbImageManager;
/*!
 * \brief The ItiOtbImageViewerPanelSetupTab class
 */
class ItiOtbImageViewerPanelSetupTab : public QWidget
{
    Q_OBJECT
public:
    //! ctor
    explicit ItiOtbImageViewerPanelSetupTab(ItiOtbImageViewerPanel *panel);

    //!
    void setupChannels();

    bool isGreyScale() const { return m_isGreyscale; }

    // getter
    int currentGreyscaleChannel() const { return m_pSpinBoxGreyscaleChannel->value(); }

    // getter
    int currentRedChannel() const { return m_pSpinBoxRedChannel->value(); }

    // getter
    int currentGreenChannel() const { return m_pSpinBoxGreenChannel->value(); }

    // getter
    int currentBlueChannel() const { return m_pSpinBoxBlueChannel->value(); }
    
signals:

    /*!
     * \brief greyScaleColorCompositionChannelChanged
     *  emitted when the grey scale channel selection through the spinbox value has been changed
     */
    void greyScaleColorCompositionChannelChanged(int);

    /*!
     * \brief rgbColorCompositionChannelsChanged emitted when the rgb spinbox values has been changed
     * \param red, the red channel number
     * \param green, the green channel number
     * \param blue, the blue channel number
     */
    void rgbColorCompositionChannelsChanged(int red, int green, int blue);

    /*!
     * \brief contrastEnhancementChanged emitted when the contrast enhancement method has been changed
     * \param method , the method to be used for contrast enhancement
     * \param aval, the a val could be a standard deviation value of a lower quantile value
     * \param bval, the bval could be -1.0 if the method is standard deviation, or the upper quantile value
     */
    void contrastEnhancementChanged(int method, double aval, double bval);

    //!
    void classLabelChanged();

    //!
    void classLabelToggled(bool, int);


    void colorChanged(const QColor& , int);

    /*!
     * \brief deviationChanged
     */
    void deviationChanged(double );

    /*!
     * \brief upperQuantileChanged
     */
    void upperQuantileChanged(double );

    /*!
     * \brief lowerQuantileChanged
     */
    void lowerQuantileChanged(double );

    /*!
     * \brief greyscaleChannelChanged
     */
    void greyscaleChannelChanged(int );

    /*!
     * \brief applyContrastEnhancementGaussian
     */
    void applyContrastEnhancementGaussian();

    /*!
     * \brief applyContrastEnhancementLinear0255
     */
    void applyContrastEnhancementLinear0255();

    /*!
     * \brief applyContrastEnhancementLinearXPerc
     */
    void applyContrastEnhancementLinearXPerc();

    /*!
     * \brief applyContrastEnhancementSquareRoot
     */
    void applyContrastEnhancementSquareRoot();

    /*!
     * \brief applyColorCompositionGreyscale
     */
    void applyColorCompositionGreyscale();

    /*!
     * \brief applyColorCompostionRGB
     */
    void applyColorCompositionRGB();
    
public slots:
    //!
    void onRadioButtonColorCompositionChanged();

    //!
    void onComboBoxContrastEnhancementMethodCurrentIndexChanged(int);

    //!
    void onChannelValueChanged(int val);


private slots:
    //
    void onUpperQuantileLineEditChanged(const QString &);

    //
    void onLowerQuantileLineEditChanged(const QString &);

    //
    void onStandardDeviationLineEdiitChanged(const QString &);

    /*!
     * \brief onContrastEnhancementApplyButtonClicked
     */
    void onContrastEnhancementApplyButtonClicked();

    /*!
     * \brief onColorCompositionApplyButtonClicked
     */
    void onColorCompositionApplyButtonClicked();

    /*!
     * \brief onClassLabelCheckstateToggled
     */
    void onClassLabelCheckstateToggled(QStandardItem*);


    void onClassificationTreeViewDoubleClick(const QModelIndex& );

private:
    //
    void setupClassificationData();

    //
    void initialize();

    //!
    void setupColorCompositionGroupBox();

    //!
    void setupContrastEnhancememtGroupBox();

    //!
    void setupClassficationGroupBox();

    //! Classes declarations for color composition group box
    QGroupBox       *m_pGroupBoxColorComposition;
    QRadioButton    *m_pRadioButtonGrayscaleMode;
    QRadioButton    *m_pRadioButtonRGBCompositionMode;
    QSpinBox        *m_pSpinBoxGreyscaleChannel;
    QSpinBox        *m_pSpinBoxRedChannel;
    QSpinBox        *m_pSpinBoxGreenChannel;
    QSpinBox        *m_pSpinBoxBlueChannel;
    QPushButton     *m_pButtonApplyColorComposition;
    QLabel          *m_pLabelGreyChannel;
    QLabel          *m_pLabelRedChannel;
    QLabel          *m_pLabelGreenChannel;
    QLabel          *m_pLabelBlueChannel;

    //! Classes declarations for contrast enhancement group box
    QGroupBox       *m_pGroupBoxContrastEnhancement;
    QGroupBox       *m_pGroupBoxMethod;
    QGroupBox       *m_pGroupBoxClassification;
    QComboBox       *m_pComboBoxMethod;
    QPushButton     *m_pButtonApplyContrastEnhancement;
    QLabel          *m_pLabelUpperQuantile;
    QLabel          *m_pLabelLowerQuantile;
    QLabel          *m_pLabelStandardDeviation;
    QLineEdit       *m_pLineEditUpperQuantileValue;
    QLineEdit       *m_pLineEditLowerQuantileValue;
    QLineEdit       *m_pLineEditStandardDeviationValue;
    QTreeView       *m_pTreeViewClassfication;
    QLabel          *m_pLabelClassificationListHeader;
    QLabel          *m_pLabelClassificationOutput;

    /*!
     * \brief m_pClassificationModel
     */
    QStandardItemModel *m_pClassificationModel;

    /*!
     * \brief m_pItiOtbImageViewerPanel
     */
    ItiOtbImageViewerPanel* m_pItiOtbImageViewerPanel;


    QHash<int ,QColor> m_colordata;


    bool            m_isGreyscale;
};

}

#endif // ITIOTBIMAGEVIEWERPANELSETUPDATATAB_H
