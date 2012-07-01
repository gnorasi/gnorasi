####################################################################################################
#  Module activation options                                                                       #   
####################################################################################################

# standard modules without external dependencies (enabled by default)
OPTION(VRN_MODULE_BASE          "Provides basic rendering and data processing functionality"    OFF )
OPTION(VRN_MODULE_CONNEXE       "Connected component analysis using the Connexe library"        OFF )
OPTION(VRN_MODULE_DYNAMICGLSL         
    "Provides a processor that parses its shader and dynamically adds port and properties"      OFF )
OPTION(VRN_MODULE_FLOWREEN      "Provides flow visualization techniques"                        OFF )
OPTION(VRN_MODULE_PLOTTING      "Provides plotting components"                                  OFF )
OPTION(VRN_MODULE_PVM           "Provides a volume reader for Stefan Roettger's PVM format"     OFF )
OPTION(VRN_MODULE_SEGY          "Provides a volume reader for the SEGY format"                  OFF )
OPTION(VRN_MODULE_STAGING       "Experimental code almost ready for productive use"             OFF )
OPTION(VRN_MODULE_VOLUMELABELING "Provides interactive volume labeling"                         OFF )

# important modules with external dependencies that are usually available on a target system
OPTION(VRN_MODULE_DEVIL         "Provides image I/O and processing using the DevIL library"     OFF )
OPTION(VRN_MODULE_ZIP           "Provides ZIP support via the zlib library"                     OFF )

# modules with non-standard external dependencies
OPTION(VRN_MODULE_FONTRENDERING "Provides font rendering using the Freetype/FTGL libraries"     OFF)
OPTION(VRN_MODULE_FFMPEG        "Provides video encoding using the FFmpeg library"              OFF)
OPTION(VRN_MODULE_TIFF          "A volume reader for multi-image TIFF files, using the LibTIFF" OFF)
OPTION(VRN_MODULE_PYTHON        "Python bindings, allowing scripts to access the Voreen API"    OFF)
OPTION(VRN_MODULE_OPENCL        "Provides an OpenCL wrapper and OpenCL-based processors"        OFF)
OPTION(VRN_MODULE_OPENMP        "Activates OpenMP code for parallelization"                     OFF)

# dicom support
OPTION(VRN_MODULE_GDCM  
    "Provides a DICOM volume reader using the GDCMv2 library (recommended DICOM module)"        OFF)
OPTION(VRN_MODULE_DCMTK  
    "Provides a DICOM volume reader using the DCMTK library (deprecated, use GDCM module)"      OFF)

# additional modules
OPTION(VRN_MODULE_DEPRECATED    "Deprecated code to be removed in future releases"              OFF)
OPTION(VRN_MODULE_OTB    	"Orfeo Toolbox module"              ON)