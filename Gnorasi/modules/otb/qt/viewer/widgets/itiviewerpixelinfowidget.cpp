#include "itiviewerpixelinfowidget.h"

#include <QTextEdit>
#include <QVBoxLayout>
#include <QLabel>

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
    QLabel *pLabel = new QLabel(m_title,this);
    QString css;
    css = QString("QLabel { color: blue; font: bold 13px; } ");
    pLabel->setStyleSheet(css);
    pLabel->setAlignment(Qt::AlignHCenter);
    pLabel->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    m_pTextEdit = new QTextEdit(this);
    QFont font;
    font.setPointSize(8);
    m_pTextEdit->setFont(font);
    m_pTextEdit->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->addWidget(pLabel);
    layout->addWidget(m_pTextEdit);

    setLayout(layout);
}

//!
void ItiViewerPixelInfoWidget::setText(const QString &s){
    m_text = s;

    m_pTextEdit->setText(s);
}
