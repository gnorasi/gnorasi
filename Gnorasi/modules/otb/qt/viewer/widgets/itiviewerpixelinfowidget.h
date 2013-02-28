#ifndef ITIVIEWERPIXELINFOWIDGET_H
#define ITIVIEWERPIXELINFOWIDGET_H

#include <QWidget>

//class QTextEdit;
class QLabel;
class QGroupBox;

namespace itiviewer{

/*!
 * \brief The ItiViewerPixelInfoWidget class shows pixel information related to the position of
 *  the cursor over the visualized image.
 */
class ItiViewerPixelInfoWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString text     READ text   WRITE setText   NOTIFY textChanged)
    Q_PROPERTY(QString title    READ title  WRITE setTitle  NOTIFY titleChanged)

public:
    /*!
     * \brief ItiViewerPixelInfoWidget , ctor
     * \param parent
     */
    explicit ItiViewerPixelInfoWidget(QWidget *parent = 0);

    /*!
     * \brief text , getter
     * \return the text variable
     */
    QString text() const { return m_text; }

    /*!
     * \brief setText , setter
     * \param s , the new text
     */
    void setText(const QString &s);

    /*!
     * \brief title
     * \return
     */
    QString title() const { return m_title; }

    /*!
     * \brief setTitle
     * \param t
     */
    void setTitle(const QString &t) { m_title = t; }
    
public slots:

    void updateText(const QString &s) { setText(s); }

signals:
    /*!
     * \brief textChanged
     */
    void textChanged();


    /*!
     * \brief titleChanged
     */
    void titleChanged();

private:

    QGroupBox *m_pGroupBox;

    /*!
     * \brief m_title
     */
    QString m_title;

    /*!
     * \brief m_pTextEdit
     */
//    QTextEdit *m_pTextEdit;
    QLabel *m_pLabel;

    /*!
     * \brief setupLayout
     */
    void setupLayout();

    /*!
     * \brief m_text
     */
    QString m_text;

};

}

#endif // ITIVIEWERPIXELINFOWIDGET_H
