#include "histogramgenerator.h"

#include <QDebug>

HistogramGenerator::HistogramGenerator(QObject *parent) :
    QObject(parent)
{
    //! initializations
    histogramGenerator  = HistogramGeneratorType::New();
    reader              = ReaderType::New();

    //! the default mode is RGB mode
    m_rmode = RMODE_RGB;

    //! OTB initialization
    writer = WriterType::New();

    //! scan the application directory and check whether a folder named "helperImageRepo" exists
    QStringList filters;
    filters << "helperImageRepo";
    m_dir.setPath(QCoreApplication::applicationDirPath());
    if(m_dir.entryInfoList(filters,QDir::Dirs).isEmpty())
        m_dir.mkdir("helperImageRepo");

    m_dir.cd("helperImageRepo");

    m_redChannel    = 0;
    m_greenChannel  = 1;
    m_blueChannel   = 2;

    clearImageRepo();
}

/*
 * Create a new name on the application dir folder
 */
QString HistogramGenerator::constructNewName(){
    QString path;

    int counter = 1;

    while(path.isEmpty()){
        QString hlper = QString("helperImage").append(QString::number(counter++)).append(".img");

        if(!m_dir.exists(hlper))
            path = m_dir.filePath(hlper);
    }

    return path;
}

/*!
 * \brief HistogramGenerator::generateHistogram
 *  the following code has been copy pasted from the ITK documentation
 * \param path
 */
void HistogramGenerator::generateHistogram(const QString &path){
    reader->SetFileName(path.toStdString());
    try
    {
        reader->Update();

        if(m_rmode == RMODE_GREYSCALE){
            m_greyscaleChannelData.clear();
            parseGreyscaleChannel();

        }else{
            m_redChannelData.clear();
            m_greenChannelData.clear();
            m_blueChannelData.clear();


            parseRedChannel();
            parseGreenChannel();
            parseBlueChannel();
        }
    }
    catch( itk::ExceptionObject & excp )
    {
        qDebug() << "Problem encoutered while reading image file : " << path << "\n";
        return;
    }
}


void HistogramGenerator::generateHistogram(VectorImageType *image){

    //! create a new path name
    QString path = constructNewName();

    QFile file(path);
    //! check whether the file exists
    if(file.exists()){
        qDebug() << "file exists .." << path;
        return;
    }

    //! now convert the filename to std string
    std::string filename = path.toStdString();

    //! check whether the std string is empty
    if(path.isEmpty())
        return;

    //! write the content to the file using the writer object
    writer->SetFileName(filename.c_str());
    writer->SetInput(image);
    try
    {
        writer->Update();
    }
    catch (itk::ExceptionObject& err)
    {
        qDebug() << "ExceptionObject caught !";
        return;
    }

    //! now get the path and generate the histogram from the paths
    generateHistogram(path);
}


void HistogramGenerator::parseGreyscaleChannel(){
    SizeType size;

    if(!m_currentGreyChannel){
        size[0] = 255;      // number of bins for the Red   channel
        size[1] = 1;        // number of bins for the Green channel
        size[2] = 1;        // number of bins for the Blue  channel
    }
    else if(m_currentGreyChannel == 1){
        size[0] = 1;        // number of bins for the Red   channel
        size[1] = 255;      // number of bins for the Green channel
        size[2] = 1;        // number of bins for the Blue  channel
    }else{
        size[0] = 1;        // number of bins for the Red   channel
        size[1] = 1;        // number of bins for the Green channel
        size[2] = 255;      // number of bins for the Blue  channel
    }

    //! set the input from the reader
    histogramGenerator->SetInput(  reader->GetOutput()  );

    //! setup properties
    histogramGenerator->SetNumberOfBins( size );
    histogramGenerator->SetMarginalScale( 10.0 );

    //! now compute the histogram
    histogramGenerator->Compute();

    //! get the histogram
    histogram = histogramGenerator->GetOutput();

    // get the histogram size
    const unsigned int histogramSize = histogram->Size();

    qDebug() << "Histogram size " << histogramSize << "\n";

    // create an iterator
    HistogramType::ConstIterator itr = histogram->Begin();
    HistogramType::ConstIterator end = histogram->End();

    typedef HistogramType::FrequencyType FrequencyType;

    unsigned int binNumber = 0;
    // iterate through the values and set data to the hash instance
    while( itr != end )
    {
        const FrequencyType frequency = itr.GetFrequency();

        m_greyscaleChannelData[binNumber] = (double)frequency;
//        histogramFile.write( (const char *)(&frequency), sizeof(frequency) );

        ++itr;
        ++binNumber;
    }
}

//
void HistogramGenerator::parseRedChannel(){
    SizeType size;

    //! TODO
    //!
    //! now this may change in the future
    //!
    //! the maximum number at the moment is 3, namely the hisogram is being calculaate only for the first three channels
    unsigned int maxNo = 3;
    unsigned int max = qMax(maxNo,m_redChannel);
    max = qMax(m_greenChannel,max);
    max = qMax(m_blueChannel,max);

    for(int i = 0; i < max; i++){
        if(i == m_redChannel)
            size[i] = 255;
        else
            size[i] = 1;
    }

    // set the num ber of bins
    histogramGenerator->SetNumberOfBins( size );

    // set margin scale
    histogramGenerator->SetMarginalScale( 10.0 );

    // set the input form the reader
    histogramGenerator->SetInput(  reader->GetOutput()  );

    // compute values
    histogramGenerator->Compute();

    // get the histogram output
    histogram = histogramGenerator->GetOutput();

    // get the histgram size
    const unsigned int histogramSize = histogram->Size();

    qDebug() << "Histogram size " << histogramSize << "\n";

    std::cout << "Histogram of the red component" << std::endl;

    double helperFreq[255];
    double helperAmpl[255];

    // iterate thgrough the histrgram and set  the values to the hash and list instaces
    for( unsigned int bin=0; bin < histogramSize; bin++ )
    {
        helperFreq[bin] = (double)bin;

        helperAmpl[bin] = (double)histogram->GetFrequency( bin, m_redChannel );

        m_redChannelData[bin] = (double)histogram->GetFrequency( bin, m_redChannel );
    }

    m_pRedChannelAmplitude = &helperAmpl[0];
    m_pRedChannelFrequency = &helperFreq[0];
}


void HistogramGenerator::parseGreenChannel(){
    SizeType size;

    //! TODO
    //!
    //! now this may change in the future
    //!
    //! the maximum number at the moment is 3, namely the hisogram is being calculaate only for the first three channels
    //!
    unsigned int maxNo = 3;
    unsigned int max = qMax(maxNo,m_redChannel);
    max = qMax(m_greenChannel,max);
    max = qMax(m_blueChannel,max);

    for(int i = 0; i < max; i++){
        if(i == m_greenChannel)
            size[i] = 255;
        else
            size[i] = 1;
    }

    // se the nuber of beans
    histogramGenerator->SetNumberOfBins( size );

    // compute the histogram
    histogramGenerator->Compute();

    // get the size
    const unsigned int histogramSize = histogram->Size();

    //
    std::cout << "Histogram of the green component" << std::endl;

    double helperFreq[255];
    double helperAmpl[255];

    // iterate thgrough histogram and set values to the hash instances
    for( unsigned int bin=0; bin < histogramSize; bin++ )
    {
        helperFreq[bin] = (double)bin;

        helperAmpl[bin] = (double)histogram->GetFrequency( bin, m_greenChannel );

        m_greenChannelData[bin] = (double)histogram->GetFrequency( bin, m_greenChannel );
    }

    m_pGreenChannelAmplitude = &helperAmpl[0];
    m_pGreenChannelFrequency = &helperFreq[0];
}


void HistogramGenerator::parseBlueChannel(){
    SizeType size;

    //! TODO
    //!
    //! now this may change in the future
    //!
    //! the maximum number at the moment is 3, namely the hisogram is being calculaate only for the first three channels
    unsigned int maxNo = 3;
    unsigned int max = qMax(maxNo,m_redChannel);
    max = qMax(m_greenChannel,max);
    max = qMax(m_blueChannel,max);

    for(int i = 0; i < max; i++){
        if(i == m_blueChannel)
            size[i] = 255;
        else
            size[i] = 1;
    }

    // set nubmer of beans
    histogramGenerator->SetNumberOfBins( size );

     // compute values
    histogramGenerator->Compute();

    // get the hiostgram size
    const unsigned int histogramSize = histogram->Size();

    std::cout << "Histogram of the blue component" << std::endl;

    double helperFreq[255];
    double helperAmpl[255];

    // iterate thgrough the histogram and set the values to the hash instances
    for( unsigned int bin=0; bin < histogramSize; bin++ )
    {
        helperFreq[bin] = (double)bin;
        helperAmpl[bin] = (double)histogram->GetFrequency( bin, m_blueChannel );

        m_blueChannelData[bin] = (double)histogram->GetFrequency( bin, m_blueChannel );
    }

    m_pBlueChannelAmplitude = &helperAmpl[0];
    m_pBlueChannelFrequency = &helperFreq[0];
}

// clear all files from the helper folder
void HistogramGenerator::clearImageRepo(){
    QStringList list = m_dir.entryList(QDir::Files);
    QStringList::const_iterator i;
    for(i = list.constBegin(); i != list.constEnd(); i++){
        if(m_dir.remove(*i))
            qDebug() << "removing file : " << *i;
        else
            qDebug() << "could not delete the file : " << *i;
    }
}

HistogramGenerator::~HistogramGenerator(){
    clearImageRepo();
}
