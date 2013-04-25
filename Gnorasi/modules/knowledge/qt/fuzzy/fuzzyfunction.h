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


#ifndef FUZZYFUNCTION_H
#define FUZZYFUNCTION_H

#include <QObject>
#include <QStringList>

#define MAXNUMBEROFPARAMETERS 3

class FuzzyFunction;

/*!
 * \brief The FuzzyFunction class
 *  This is an interface class that provides the interface for all specialized , concrete sub classes
 *  It supports a set of pure virtual functions
 */
class FuzzyFunction : public QObject
{
    Q_OBJECT
public:

    /*!
     * \brief parametersCount
     *  This is used on enabling and disabling the table fields,
     * \return
     */
    virtual int parametersCount() const = 0;
    
    /*!
     * \brief parameterFunctionForIndex
     *  This function supports fast access to the parameter values , from the GUI side.
     * \param i
     * \return
     */
    virtual double parameterValueForIndex(int i) = 0;

    /*!
     * \brief setParameterValueForIndx
     *
     *  Update the parameter value for the given index, all concrete classes must implement this function
     * \param index
     * \param val
     */
    virtual void setParameterValueForIndex(int index , double val) = 0;

    /*!
     * \brief calculate
     *  This is the function where the calculation is performed.
     *  Each subclass must implement this function. Inside here all the functionality and the calculation logic
     *  is being encapsulated.
     *  ATM only one subclass function has been created and implemented the functionality see.  FuzzyFunctionLeftShoulder
     * \param val, the original value of the property
     * \return
     */
    virtual double calculate(double val) = 0;

    /*!
     * \brief name
     * \return
     */
    virtual QString name() const  = 0;

    /*!
     * \brief isReady
     *  This is a helper function, this function is used while creating the text output data , xml and label map properties.
     *
     * \return
     */
    virtual bool isReady() = 0;

    /*!
     * \brief MAXPARAMETERSCOUNT
     *  This is a general static value , for setting up the table view columns.
     *  This might not be here , this was done for making things quickly finished
     */
    static const int MAXPARAMETERSCOUNT = MAXNUMBEROFPARAMETERS;


    /*!
     * \brief availableFunctions
     *  Another list providing all the available names of the concrete function classes.
     *
     *        ** IMPORTANT **
     *
     *  When other subclasses are being created this list should be updated with the names of those function
     *
     *
     */
    static QStringList availableFunctions;


protected:
    /*!
     * \brief FuzzyFunction
     * \param parent
     */
    explicit FuzzyFunction(QObject *parent = 0) : QObject(parent) { }

signals:
    
public slots:
    
private:



};

#endif // FUZZYATTRIBUTE_H
