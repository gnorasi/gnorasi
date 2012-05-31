# link against OTB library

unix {  
    LIBS += -lOTBCommon 
    LIBS += -lOTBIO 
    LIBS += -lOTBBasicFilters
    LIBS += -lOTBGui
    LIBS += -lOTBVisualization
    LIBS += -lotbMeanShiftModule
    LIBS += -lOTBOBIA
    LIBS += -lITKCommon
    LIBS += -lITKIO
    LIBS += -lITKStatistics
    LIBS += -lITKBasicFilters
    LIBS += -lITKAlgorithms
    LIBS += -lITKNumerics
}
