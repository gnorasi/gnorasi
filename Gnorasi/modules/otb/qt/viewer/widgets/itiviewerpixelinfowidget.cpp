#include "itiviewerpixelinfowidget.h"

#include <QTextEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QGroupBox>

using namespace itiviewer;

ItiViewerPixelInfoWidget::ItiViewerPixelInfoWidget(QWidget *parent) :
    QWidget(parent)
{
    m_title = tr("Pixel Information");
    //!
    setupLayout();
}

//!
void ItiViewerPixelInfoWidget::setupLayout(){

    m_pGroupBox = new QGroupBox(this);
    m_pGroupBox->setTitle(tr("Pixel Information"));
    m_pLabel = new QLabel(this);
    QVBoxLayout *boxLayout1 = new QVBoxLayout();
    boxLayout1->addWidget(m_pLabel);
    m_pGroupBox->setLayout(boxLayout1);

    QFont font;
    font.setPointSize(8);
    m_pLabel->setFont(font);
    m_pLabel->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    m_pLabel->setMinimumHeight(140);
    m_pLabel->setMinimumWidth(220);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(m_pGroupBox);
    setLayout(layout);
}

//!
void ItiViewerPixelInfoWidget::setText(const QString &s){
    m_text = s;

//    m_pTextEdit->setText(s);
    m_pLabel->setText(s);
}
