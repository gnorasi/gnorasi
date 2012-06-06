# link against OTB library

unix {  
    LIBS += -lOTBCommon 
    LIBS += -lOTBIO 
    LIBS += -lOTBBasicFilters
    LIBS += -lOTBGui
    LIBS += -lOTBVisualization
    LIBS += -lotbedison
    LIBS += -lOTBLearning
    LIBS += -lOTBOBIA
    LIBS += -lITKCommon
    LIBS += -lITKIO
    LIBS += -lITKStatistics
    LIBS += -lITKBasicFilters
    LIBS += -lITKAlgorithms
    LIBS += -lITKNumerics
}
