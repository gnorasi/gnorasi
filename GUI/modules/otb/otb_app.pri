# link against OTB library

unix {  
    LIBS += -lOTBCommon 
    LIBS += -lOTBIO 
    LIBS += -lOTBBasicFilters
    LIBS += -lOTBGui
    LIBS += -lOTBVisualization
    LIBS += -lITKCommon
    LIBS += -lITKIO
    LIBS += -lITKStatistics
    LIBS += -lITKBasicFilters
    LIBS += -lITKAlgorithms
    LIBS += -lITKNumerics
}
