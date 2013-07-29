#ifndef itiotbAbstractImageModel_h
#define itiotbAbstractImageModel_h

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "itiotbAbstractModel.h"


namespace itiviewer
{
//
// Internal classes pre-declaration.

class ItiOtbImageManager;

/** \class AbstractImageModel
 *
 */
class AbstractImageModel :
    public AbstractModel
{
  Q_OBJECT

//
// Public methods.
public:

  /** Destructor */
  virtual ~AbstractImageModel();

    /*!
    * \brief itiOtbImageManager
    * \return
    */
    ItiOtbImageManager* itiOtbImageManager() { return m_pManager; }

//
// SIGNALS.
signals:

//
// Protected methods.
protected:

  /** Constructor */
  AbstractImageModel(ItiOtbImageManager *manager, QObject* parent = NULL );

//
// Protected attributes.
protected:
    ItiOtbImageManager *m_pManager;
//
// Private methods.
private:


//
// Private attributes.
private:



//
// SLOTS.
private slots:
};

} // end namespace itiviewer

#endif // itiotbAbstractImageModel_h
