####################################################
# Project file for the Voreen-Core library
####################################################
TEMPLATE = lib
TARGET = voreen_core
unix: VERSION = 1.0

CONFIG += thread
CONFIG -= qt

# include config
!exists(../../config.txt) {
  error("config.txt not found! copy config-default.txt to config.txt and edit!")
}
include(../../config.txt)

# Include common configuration
include(../../commonconf.pri)

# Destination directory for the library
macx {
DESTDIR = "$${VRN_HOME}/bin"
QMAKE_LFLAGS_SONAME = -install_name$${LITERAL_WHITESPACE}$${VRN_HOME}/bin/
}
unix: DESTDIR = "$${VRN_HOME}/bin"
win32 {
    CONFIG(debug, debug|release) {
        DESTDIR = "$${VRN_HOME}/bin/Debug"
    }
    else {
        DESTDIR = "$${VRN_HOME}/bin/Release"
    }
}

win32|macx {
    contains(DEFINES, VRN_DYNAMIC_LIBS) {
        CONFIG += dll
        CONFIG -= static embed_manifest_dll
        DEFINES += VRN_CORE_BUILD_DLL
    }
    else {
        CONFIG += static
        CONFIG -= dll
    }
}

win32 { # common configuration for win 32 and 64 bit builds  
    QMAKE_LIBDIR += "$${VRN_HOME}/ext/glew/lib"

    LIBS += -lnetapi32 -lopengl32 -lglu32
    LIBS += -lVersion

    nmake: LIBS *= advapi32.lib shell32.lib

    # no libc for vc++ since we build a multithreaded executable
    LIBS += /NODEFAULTLIB:libc.lib
    
    contains(DEFINES, TGT_WITH_WMI) : LIBS += -lWbemUuid

    for(i, VRN_MODULES) : exists($${VRN_MODULE_DIR}/$${i}/$${i}_app.pri) {
        include($${VRN_MODULE_DIR}/$${i}/$${i}_app.pri)
    }
}

win32-msvc { # win 32 bit configuration
    LIBS += "$${VRN_HOME}/ext/glew/lib/win32/vs$${VS_VERSION_NUMBER}/$${WIN32_CONFIG_NAME}/glew32s.lib"
    LIBS += "$${VRN_HOME}/bin/$${WIN32_CONFIG_NAME}/tgt.lib"
}
  
win64-msvc { # win 64 bit configuration
    LIBS += "$${VRN_HOME}/ext/glew/lib/win64/vs$${VS_VERSION_NUMBER}/$${WIN64_CONFIG_NAME}/glew64s.lib"
    LIBS += "$${VRN_HOME}/bin/$${WIN64_CONFIG_NAME}/tgt.lib"
}

unix {
    LIBS += -ltgt
}

macx {
    LIBS += -framework CoreFoundation
}

contains(DEFINES, VRN_PRECOMPILE_HEADER) {
  PRECOMPILED_HEADER = ../../pch.h
  CONFIG += precompile_header
}
else {
  CONFIG -= precompile_header
}

# Include modules which are selected in local configuration. The entry
# 'foo' in VRN_MODULES must correspond to a subdir 'modules/foo' and a
# file 'foo_core.pri' there.
for(i, VRN_MODULES) : include($${VRN_MODULE_DIR}/$${i}/$${i}_core.pri)


#
# Generate module registration file
#
!contains(DEFINES, VRN_NO_REGISTRATION_HEADER_GENERATION) {
    MODULE_REGISTRATION_FILE = "$${VRN_MODULE_DIR}/gen_moduleregistration.h"
    message ("Generating 'modules/gen_moduleregistration.h'...")

    REGIST_LINES += "$${LITERAL_HASH}include \"voreen/core/voreenapplication.h\""
    REGIST_LINES += "// module class headers"
    for(i, VRN_MODULE_CLASS_HEADERS) {
        REGIST_LINES += "$${LITERAL_HASH}include \"modules/$${i}\""
    }

    REGIST_LINES += "// instantiate module classes"
    REGIST_LINES += "namespace voreen \{"
    REGIST_LINES += "void addAllModules(VoreenApplication* vapp) \{"
    for(i, VRN_MODULE_CLASSES) {
        REGIST_LINES += "(*vapp).addModule(new $${i}());"
    }
    REGIST_LINES += "}}"
#    REGIST_LINES += "\} // namespace"

    # write lines to module registration file
    win32 {
        system(echo "// WARNING: This file is auto-generated!" > \"$${MODULE_REGISTRATION_FILE}\")
        for(i, REGIST_LINES) : system(echo $${i}  >> \"$${MODULE_REGISTRATION_FILE}\")
    }
    else:unix {
        system(echo "\"// WARNING: This file is auto-generated!\"" > \"$${MODULE_REGISTRATION_FILE}\")
        for(i, REGIST_LINES) : system(echo \'$${i}\'  >> \"$${MODULE_REGISTRATION_FILE}\")
    }
    else {
        warning("Unknown platform: registration file not generated")
        DEFINES += VRN_NO_REGISTRATION_HEADER_GENERATION
    }

    # add generated file to headers if exists, otherwise use static registration file as fallback
    exists($${MODULE_REGISTRATION_FILE}) {
        HEADERS += "$${VRN_MODULE_DIR}/gen_moduleregistration.h"
    }
    else {
        warning("$${MODULE_REGISTRATION_FILE} not found. Using static fallback!")
        DEFINES += VRN_NO_REGISTRATION_HEADER_GENERATION
    }
}

contains(DEFINES, VRN_NO_REGISTRATION_HEADER_GENERATION) {
    message("Using static module registration file 'modules/moduleregistration.h'")
    HEADERS += "$${VRN_MODULE_DIR}/moduleregistration.h"
}

# add module class source/headers
for(i, VRN_MODULE_CLASS_HEADERS) : HEADERS += $${VRN_MODULE_DIR}/$${i}
for(i, VRN_MODULE_CLASS_SOURCES) : SOURCES += $${VRN_MODULE_DIR}/$${i}


# please insert new files in alphabetically order!
SOURCES += \
    coremodule.cpp \
    version.cpp \
    voreenapplication.cpp \
    voreenmodule.cpp
SOURCES += \
    animation/animatedprocessor.cpp \
    animation/animation.cpp \
    animation/interpolationfunction.cpp \
    animation/interpolationfunctionbase.cpp \
    animation/interpolationfunctionfactory.cpp \
    animation/propertykeyvalue.cpp \
    animation/serializationfactories.cpp \
    animation/templatepropertytimeline.cpp \
    animation/templatepropertytimelinestate.cpp \
    animation/interpolation/basicfloatinterpolation.cpp \
    animation/interpolation/basicintinterpolation.cpp \
    animation/interpolation/boolinterpolationfunctions.cpp \
    animation/interpolation/camerainterpolationfunctions.cpp \
    animation/interpolation/floatinterpolationfunctions.cpp \
    animation/interpolation/intinterpolationfunctions.cpp \
    animation/interpolation/ivec2interpolationfunctions.cpp \
    animation/interpolation/ivec3interpolationfunctions.cpp \
    animation/interpolation/ivec4interpolationfunctions.cpp \
    animation/interpolation/mat2interpolationfunctions.cpp \
    animation/interpolation/mat3interpolationfunctions.cpp \
    animation/interpolation/mat4interpolationfunctions.cpp \
    animation/interpolation/quatinterpolationfunctions.cpp \
    animation/interpolation/shadersourceinterpolationfunctions.cpp \
    animation/interpolation/stringinterpolationfunctions.cpp \
    animation/interpolation/transfuncinterpolationfunctions.cpp \
    animation/interpolation/vec2interpolationfunctions.cpp \
    animation/interpolation/vec3interpolationfunctions.cpp \
    animation/interpolation/vec4interpolationfunctions.cpp \
    animation/interpolation/volumecollectioninterpolationfunctions.cpp \
    animation/interpolation/volumehandleinterpolationfunctions.cpp
SOURCES += \
    datastructures/datetime.cpp \
    datastructures/imagesequence.cpp \
    datastructures/rendertarget.cpp \
    datastructures/geometry/facegeometry.cpp \
    datastructures/geometry/geometry.cpp \
    datastructures/geometry/geometryfactory.cpp \
    datastructures/geometry/meshgeometry.cpp \
    datastructures/geometry/meshlistgeometry.cpp \
    datastructures/geometry/vertexgeometry.cpp \
    datastructures/transfunc/transfunc.cpp \
    datastructures/transfunc/transfuncfactory.cpp \
    datastructures/transfunc/transfuncintensity.cpp \
    datastructures/transfunc/transfuncintensitygradient.cpp \
    datastructures/transfunc/transfuncmappingkey.cpp \
    datastructures/transfunc/transfuncprimitive.cpp \
    datastructures/volume/gradient.cpp \
    datastructures/volume/histogram.cpp \
    datastructures/volume/modality.cpp \
    datastructures/volume/volume.cpp \
    datastructures/volume/volumecollection.cpp \
    datastructures/volume/volumecontainer.cpp \
    datastructures/volume/volumederiveddata.cpp \
    datastructures/volume/volumederiveddatafactory.cpp \
    datastructures/volume/volumeelement.cpp \
    datastructures/volume/volumefactory.cpp \
    datastructures/volume/volumegl.cpp \
    datastructures/volume/diskrepresentation.cpp \
    datastructures/volume/volumehandle.cpp \
    datastructures/volume/volumehandledecorator.cpp \
    datastructures/volume/volumehash.cpp \
    datastructures/volume/volumerepresentation.cpp \
    datastructures/volume/volumetexture.cpp 

SOURCES += \
    interaction/booltoggleinteractionhandler.cpp \
    interaction/buttonpressinteractionhandler.cpp \
    interaction/camerainteractionhandler.cpp \
    interaction/firstpersonnavigation.cpp \
    interaction/flythroughnavigation.cpp \
    interaction/idmanager.cpp \
    interaction/interactionhandler.cpp \
    interaction/interactionhandlerfactory.cpp \
    interaction/pickingmanager.cpp \
    interaction/slicecamerainteractionhandler.cpp \
    interaction/trackballnavigation.cpp \
    interaction/voreentrackball.cpp
SOURCES += \
    io/datvolumereader.cpp \
    io/datvolumewriter.cpp \
    io/progressbar.cpp \
    io/rawvolumereader.cpp \
    io/textfilereader.cpp \
    io/timetofinishreporter.cpp \
    io/vevovolumereader.cpp \
    io/visiblehumanreader.cpp \
    io/volumereader.cpp \
    io/volumeserializer.cpp \
    io/volumeserializerpopulator.cpp \
    io/volumewriter.cpp \
    io/vvdformat.cpp \
    io/vvdvolumereader.cpp \
    io/vvdvolumewriter.cpp \
    io/serialization/xmldeserializer.cpp \
    io/serialization/xmlserializationconstants.cpp \
    io/serialization/xmlserializer.cpp \
    io/serialization/xmlserializerbase.cpp \
    io/serialization/meta/aggregationmetadata.cpp \
    io/serialization/meta/coremetadatafactory.cpp \
    io/serialization/meta/metadatacontainer.cpp \
    io/serialization/meta/networkannotation.cpp \
    io/serialization/meta/positionmetadata.cpp \
    io/serialization/meta/realworldmappingmetadata.cpp \
    io/serialization/meta/windowstatemetadata.cpp \
    io/serialization/meta/zoommetadata.cpp
SOURCES += \
    network/networkconverter.cpp \
    network/networkevaluator.cpp \
    network/networkgraph.cpp \
    network/networkserializer.cpp \
    network/portconnection.cpp \
    network/processornetwork.cpp \
    network/processornetworkobserver.cpp \
    network/propertystate.cpp \
    network/propertystatecollection.cpp \
    network/workspace.cpp
SOURCES += \
    ports/coprocessorport.cpp \
    ports/geometryport.cpp \
    ports/port.cpp \
    ports/renderport.cpp \
    ports/textport.cpp \
    ports/volumeport.cpp
SOURCES += \
    ports/conditions/portcondition.cpp \
    ports/conditions/portconditionvolumetype.cpp 
SOURCES += \
    processors/cache.cpp \
    processors/canvasrenderer.cpp \
    processors/geometryrendererbase.cpp \
    processors/imageprocessor.cpp \
    processors/imageprocessorbypassable.cpp \
    processors/processor.cpp \
    processors/processorfactory.cpp \
    processors/processorwidget.cpp \
    processors/profiling.cpp \
    processors/renderprocessor.cpp \
    processors/volumecreatebase.cpp \
    processors/volumeprocessor.cpp \
    processors/volumeraycaster.cpp \
    processors/volumerenderer.cpp
SOURCES += \
    properties/boolproperty.cpp \
    properties/buttonproperty.cpp \
    properties/cameraproperty.cpp \
    properties/condition.cpp \
    properties/eventproperty.cpp \
    properties/filedialogproperty.cpp \
    properties/floatproperty.cpp \
    properties/fontproperty.cpp \
    properties/intproperty.cpp \
    properties/matrixproperty.cpp \
    properties/numericproperty.cpp \
    properties/optionproperty.cpp \
    properties/property.cpp \
    properties/propertyfactory.cpp \
    properties/propertyowner.cpp \
    properties/propertyvector.cpp \
    properties/shaderproperty.cpp \
    properties/stringproperty.cpp \
    properties/stringexpressionproperty.cpp \
    properties/transfuncproperty.cpp \
    properties/vectorproperty.cpp \
    properties/volumecollectionproperty.cpp \
    properties/volumehandleproperty.cpp \
    properties/voxeltypeproperty.cpp \
    properties/link/buttonlinkevaluator.cpp \
    properties/link/corelinkevaluatorfactory.cpp \
    properties/link/dependencylinkevaluator.cpp \
    properties/link/dependencylinkevaluators.cpp \
    properties/link/linkevaluatorbase.cpp \
    properties/link/linkevaluatorboolinvert.cpp \
    properties/link/linkevaluatorfactory.cpp \
    properties/link/linkevaluatorid.cpp \
    properties/link/linkevaluatoridnormalized.cpp \
    properties/link/propertylink.cpp
SOURCES += \
    utils/glsl.cpp \
    utils/hashing.cpp \
    utils/observer.cpp \
    utils/stringconversion.cpp \
    utils/variant.cpp \
    utils/voreenpainter.cpp \
    utils/cmdparser/command.cpp \
    utils/cmdparser/command_loglevel.cpp \
    utils/cmdparser/commandlineparser.cpp \
    utils/GLSLparser/glslprogram.cpp \
    utils/GLSLparser/grammarsymbol.cpp \
    utils/GLSLparser/lexer.cpp \
    utils/GLSLparser/parser.cpp \
    utils/GLSLparser/parseraction.cpp \
    utils/GLSLparser/parsetreenode.cpp \
    utils/GLSLparser/production.cpp \
    utils/GLSLparser/terminalsmap.cpp \
    utils/GLSLparser/token.cpp \
    utils/GLSLparser/annotations/annotationlexer.cpp \
    utils/GLSLparser/annotations/annotationparser.cpp \
    utils/GLSLparser/annotations/annotationterminals.cpp \
    utils/GLSLparser/annotations/annotationvisitor.cpp \
    utils/GLSLparser/generator/annotationgrammar.cpp \
    utils/GLSLparser/generator/glslgrammar.cpp \
    utils/GLSLparser/generator/grammar.cpp \
    utils/GLSLparser/generator/lritem.cpp \
    utils/GLSLparser/generator/parsertable.cpp \
    utils/GLSLparser/generator/ppgrammar.cpp \
    utils/GLSLparser/glsl/glsllexer.cpp \
    utils/GLSLparser/glsl/glslparser.cpp \
    utils/GLSLparser/glsl/glslsymbol.cpp \
    utils/GLSLparser/glsl/glslterminals.cpp \
    utils/GLSLparser/glsl/glslvisitor.cpp \
    utils/GLSLparser/preprocessor/pplexer.cpp \
    utils/GLSLparser/preprocessor/ppmacro.cpp \
    utils/GLSLparser/preprocessor/ppparser.cpp \
    utils/GLSLparser/preprocessor/ppstatement.cpp \
    utils/GLSLparser/preprocessor/ppterminals.cpp \
    utils/GLSLparser/preprocessor/ppvisitor.cpp \
    utils/voreenblas/voreenblascpu.cpp

SOURCES += \
    ../../ext/tinyxml/tinyxml.cpp \
    ../../ext/tinyxml/tinystr.cpp \
    ../../ext/tinyxml/tinyxmlerror.cpp \
    ../../ext/tinyxml/tinyxmlparser.cpp
SOURCES += \
    ../../ext/md5/md5.c

SHADER_SOURCES_MODS += \
    glsl/modules/mod_compositing.frag \
    glsl/modules/mod_curvature.frag \
    glsl/modules/mod_depth.frag \
    glsl/modules/mod_filtering.frag \
    glsl/modules/mod_gradients.frag \
    glsl/modules/mod_normdepth.frag \
    glsl/modules/mod_raysetup.frag \
    glsl/modules/mod_sampler2d.frag \
    glsl/modules/mod_sampler3d.frag \
    glsl/modules/mod_segmentation.frag \
    glsl/modules/mod_shading.frag \
    glsl/modules/mod_transfunc.frag \
    glsl/modules/vrn_shaderincludes.frag

SHADER_SOURCES_UTILS += \
    glsl/utils/blendwithimage.frag \
    glsl/utils/copyimage.frag \
    glsl/utils/passthrough.vert \
    glsl/utils/vrn_rendertexture.frag

visual_studio: !win32-msvc2010: SOURCES += $$SHADER_SOURCES $$SHADER_SOURCES_MODS $$SHADER_SOURCES_UTILS

HEADERS += \
    ../../include/voreen/core/coremodule.h \
    ../../include/voreen/core/version.h \
    ../../include/voreen/core/voreenapplication.h \
    ../../include/voreen/core/voreencoredefine.h \
    ../../include/voreen/core/voreenmodule.h
HEADERS += \
    ../../include/voreen/core/animation/animatedprocessor.h \
    ../../include/voreen/core/animation/animation.h \
    ../../include/voreen/core/animation/animationobserver.h \
    ../../include/voreen/core/animation/interpolationfunction.h \
    ../../include/voreen/core/animation/interpolationfunctionbase.h \
    ../../include/voreen/core/animation/interpolationfunctionfactory.h \
    ../../include/voreen/core/animation/propertykeyvalue.h \
    ../../include/voreen/core/animation/propertytimeline.h \
    ../../include/voreen/core/animation/serializationfactories.h \
    ../../include/voreen/core/animation/templatepropertytimeline.h \
    ../../include/voreen/core/animation/templatepropertytimelinestate.h \
    ../../include/voreen/core/animation/timelineobserver.h \
    ../../include/voreen/core/animation/undoableanimation.h \
    ../../include/voreen/core/animation/interpolation/basicfloatinterpolation.h \
    ../../include/voreen/core/animation/interpolation/basicintinterpolation.h \
    ../../include/voreen/core/animation/interpolation/boolinterpolationfunctions.h \
    ../../include/voreen/core/animation/interpolation/camerainterpolationfunctions.h \
    ../../include/voreen/core/animation/interpolation/floatinterpolationfunctions.h \
    ../../include/voreen/core/animation/interpolation/intinterpolationfunctions.h \
    ../../include/voreen/core/animation/interpolation/ivec2interpolationfunctions.h \
    ../../include/voreen/core/animation/interpolation/ivec3interpolationfunctions.h \
    ../../include/voreen/core/animation/interpolation/ivec4interpolationfunctions.h \
    ../../include/voreen/core/animation/interpolation/mat2interpolationfunctions.h \
    ../../include/voreen/core/animation/interpolation/mat3interpolationfunctions.h \
    ../../include/voreen/core/animation/interpolation/mat4interpolationfunctions.h \
    ../../include/voreen/core/animation/interpolation/quatinterpolationfunctions.h \
    ../../include/voreen/core/animation/interpolation/shadersourceinterpolationfunctions.h \
    ../../include/voreen/core/animation/interpolation/stringinterpolationfunctions.h \
    ../../include/voreen/core/animation/interpolation/transfuncinterpolationfunctions.h \
    ../../include/voreen/core/animation/interpolation/vec2interpolationfunctions.h \
    ../../include/voreen/core/animation/interpolation/vec3interpolationfunctions.h \
    ../../include/voreen/core/animation/interpolation/vec4interpolationfunctions.h \
    ../../include/voreen/core/animation/interpolation/volumecollectioninterpolationfunctions.h \
    ../../include/voreen/core/animation/interpolation/volumehandleinterpolationfunctions.h
HEADERS += \
    ../../include/voreen/core/datastructures/datetime.h \
    ../../include/voreen/core/datastructures/imagesequence.h \
    ../../include/voreen/core/datastructures/rendertarget.h \
    ../../include/voreen/core/datastructures/tensor.h \
    ../../include/voreen/core/datastructures/geometry/facegeometry.h \
    ../../include/voreen/core/datastructures/geometry/geometry.h \
    ../../include/voreen/core/datastructures/geometry/geometryfactory.h \
    ../../include/voreen/core/datastructures/geometry/meshgeometry.h \
    ../../include/voreen/core/datastructures/geometry/meshlistgeometry.h \
    ../../include/voreen/core/datastructures/geometry/pointgeometry.h \
    ../../include/voreen/core/datastructures/geometry/pointlistgeometry.h \
    ../../include/voreen/core/datastructures/geometry/pointsegmentlistgeometry.h \
    ../../include/voreen/core/datastructures/geometry/scalargeometry.h \
    ../../include/voreen/core/datastructures/geometry/vertexgeometry.h \
    ../../include/voreen/core/datastructures/transfunc/transfunc.h \
    ../../include/voreen/core/datastructures/transfunc/transfuncfactory.h \
    ../../include/voreen/core/datastructures/transfunc/transfuncintensity.h \
    ../../include/voreen/core/datastructures/transfunc/transfuncintensitygradient.h \
    ../../include/voreen/core/datastructures/transfunc/transfuncmappingkey.h \
    ../../include/voreen/core/datastructures/transfunc/transfuncprimitive.h \
    ../../include/voreen/core/datastructures/volume/gradient.h \
    ../../include/voreen/core/datastructures/volume/histogram.h \
    ../../include/voreen/core/datastructures/volume/modality.h \
    ../../include/voreen/core/datastructures/volume/volume.h \
    ../../include/voreen/core/datastructures/volume/volumeatomic.h \
    ../../include/voreen/core/datastructures/volume/volumecollection.h \
    ../../include/voreen/core/datastructures/volume/volumecontainer.h \
    ../../include/voreen/core/datastructures/volume/volumeelement.h \
    ../../include/voreen/core/datastructures/volume/volumederiveddata.h \
    ../../include/voreen/core/datastructures/volume/volumederiveddatafactory.h \
    ../../include/voreen/core/datastructures/volume/volumefactory.h \
    ../../include/voreen/core/datastructures/volume/volumefusion.h \
    ../../include/voreen/core/datastructures/volume/volumegl.h \
    ../../include/voreen/core/datastructures/volume/diskrepresentation.h \
    ../../include/voreen/core/datastructures/volume/volumehandle.h \
    ../../include/voreen/core/datastructures/volume/volumehandledecorator.h \
    ../../include/voreen/core/datastructures/volume/volumehash.h \
    ../../include/voreen/core/datastructures/volume/volumeoperator.h \
    ../../include/voreen/core/datastructures/volume/volumerepresentation.h \
    ../../include/voreen/core/datastructures/volume/volumetexture.h \
    ../../include/voreen/core/datastructures/volume/operators/volumeoperatorcalcerror.h \
    ../../include/voreen/core/datastructures/volume/operators/volumeoperatorconvert.h \
    ../../include/voreen/core/datastructures/volume/operators/volumeoperatorhalfsample.h \
    ../../include/voreen/core/datastructures/volume/operators/volumeoperatorinvert.h \
    ../../include/voreen/core/datastructures/volume/operators/volumeoperatorisuniform.h \
    ../../include/voreen/core/datastructures/volume/operators/volumeoperatormedian.h \
    ../../include/voreen/core/datastructures/volume/operators/volumeoperatorminmax.h \
    ../../include/voreen/core/datastructures/volume/operators/volumeoperatormirror.h \
    ../../include/voreen/core/datastructures/volume/operators/volumeoperatormorphology.h \
    ../../include/voreen/core/datastructures/volume/operators/volumeoperatornormalize.h \
    ../../include/voreen/core/datastructures/volume/operators/volumeoperatornumsignificant.h \
    ../../include/voreen/core/datastructures/volume/operators/volumeoperatorresample.h \
    ../../include/voreen/core/datastructures/volume/operators/volumeoperatorresize.h \
    ../../include/voreen/core/datastructures/volume/operators/volumeoperatorsubset.h \
    ../../include/voreen/core/datastructures/volume/operators/volumeoperatorswapendianness.h \
    ../../include/voreen/core/datastructures/volume/operators/volumeoperatortranspose.h 

HEADERS += \
    ../../include/voreen/core/interaction/booltoggleinteractionhandler.h \
    ../../include/voreen/core/interaction/buttonpressinteractionhandler.h \
    ../../include/voreen/core/interaction/camerainteractionhandler.h \
    ../../include/voreen/core/interaction/firstpersonnavigation.h \
    ../../include/voreen/core/interaction/flythroughnavigation.h \
    ../../include/voreen/core/interaction/idmanager.h \
    ../../include/voreen/core/interaction/interactionhandler.h \
    ../../include/voreen/core/interaction/interactionhandlerfactory.h \
    ../../include/voreen/core/interaction/mwheelnumpropinteractionhandler.h \
    ../../include/voreen/core/interaction/pickingmanager.h \
    ../../include/voreen/core/interaction/slicecamerainteractionhandler.h \
    ../../include/voreen/core/interaction/trackballnavigation.h \
    ../../include/voreen/core/interaction/voreentrackball.h
HEADERS += \
    ../../include/voreen/core/io/datvolumereader.h \
    ../../include/voreen/core/io/datvolumewriter.h \
    ../../include/voreen/core/io/progressbar.h \
    ../../include/voreen/core/io/rawvolumereader.h \
    ../../include/voreen/core/io/textfilereader.h \
    ../../include/voreen/core/io/timetofinishreporter.h \
    ../../include/voreen/core/io/vevovolumereader.h \
    ../../include/voreen/core/io/visiblehumanreader.h \
    ../../include/voreen/core/io/volumereader.h \
    ../../include/voreen/core/io/volumeserializer.h \
    ../../include/voreen/core/io/volumeserializerpopulator.h \
    ../../include/voreen/core/io/volumewriter.h \
    ../../include/voreen/core/io/vvdformat.h \
    ../../include/voreen/core/io/vvdvolumereader.h \
    ../../include/voreen/core/io/vvdvolumewriter.h \
    ../../include/voreen/core/io/serialization/abstractserializable.h \
    ../../include/voreen/core/io/serialization/resourcefactory.h \
    ../../include/voreen/core/io/serialization/serializable.h \
    ../../include/voreen/core/io/serialization/serializablefactory.h \
    ../../include/voreen/core/io/serialization/serialization.h \
    ../../include/voreen/core/io/serialization/serializationexceptions.h \
    ../../include/voreen/core/io/serialization/xmldeserializer.h \
    ../../include/voreen/core/io/serialization/xmlprocessor.h \
    ../../include/voreen/core/io/serialization/xmlserializationconstants.h \
    ../../include/voreen/core/io/serialization/xmlserializer.h \
    ../../include/voreen/core/io/serialization/xmlserializerbase.h \
    ../../include/voreen/core/io/serialization/meta/aggregationmetadata.h \
    ../../include/voreen/core/io/serialization/meta/coremetadatafactory.h \
    ../../include/voreen/core/io/serialization/meta/metadatabase.h \
    ../../include/voreen/core/io/serialization/meta/metadatacontainer.h \
    ../../include/voreen/core/io/serialization/meta/networkannotation.h \
    ../../include/voreen/core/io/serialization/meta/primitivemetadata.h \
    ../../include/voreen/core/io/serialization/meta/positionmetadata.h \
    ../../include/voreen/core/io/serialization/meta/realworldmappingmetadata.h \
    ../../include/voreen/core/io/serialization/meta/selectionmetadata.h \
    ../../include/voreen/core/io/serialization/meta/windowstatemetadata.h \
    ../../include/voreen/core/io/serialization/meta/zoommetadata.h
HEADERS += \
    ../../include/voreen/core/network/networkconverter.h \
    ../../include/voreen/core/network/networkevaluator.h \
    ../../include/voreen/core/network/networkgraph.h \
    ../../include/voreen/core/network/networkserializer.h \
    ../../include/voreen/core/network/portconnection.h \
    ../../include/voreen/core/network/processornetwork.h \
    ../../include/voreen/core/network/processornetworkobserver.h \
    ../../include/voreen/core/network/propertystate.h \
    ../../include/voreen/core/network/propertystatecollection.h \
    ../../include/voreen/core/network/workspace.h
HEADERS += \
    ../../include/voreen/core/ports/allports.h \
    ../../include/voreen/core/ports/coprocessorport.h \
    ../../include/voreen/core/ports/genericcoprocessorport.h \
    ../../include/voreen/core/ports/genericport.h \
    ../../include/voreen/core/ports/geometryport.h \
    ../../include/voreen/core/ports/port.h \
    ../../include/voreen/core/ports/renderport.h \
    ../../include/voreen/core/ports/textport.h \
    ../../include/voreen/core/ports/volumeport.h
HEADERS += \
    ../../include/voreen/core/ports/conditions/portcondition.h \
    ../../include/voreen/core/ports/conditions/portconditionvolumetype.h 
HEADERS += \
    ../../include/voreen/core/processors/cache.h \
    ../../include/voreen/core/processors/canvasrenderer.h \
    ../../include/voreen/core/processors/geometryrendererbase.h \
    ../../include/voreen/core/processors/imageprocessor.h \
    ../../include/voreen/core/processors/imageprocessorbypassable.h \
    ../../include/voreen/core/processors/processor.h \
    ../../include/voreen/core/processors/processorfactory.h \
    ../../include/voreen/core/processors/processorwidget.h \
    ../../include/voreen/core/processors/processorwidgetfactory.h \
    ../../include/voreen/core/processors/profiling.h \
    ../../include/voreen/core/processors/renderprocessor.h \
    ../../include/voreen/core/processors/volumecreatebase.h \
    ../../include/voreen/core/processors/volumeprocessor.h \
    ../../include/voreen/core/processors/volumeraycaster.h \
    ../../include/voreen/core/processors/volumerenderer.h
HEADERS += \
    ../../include/voreen/core/properties/action.h \
    ../../include/voreen/core/properties/allactions.h \
    ../../include/voreen/core/properties/allproperties.h \
    ../../include/voreen/core/properties/boolproperty.h \
    ../../include/voreen/core/properties/buttonproperty.h \
    ../../include/voreen/core/properties/callmemberaction.h \
    ../../include/voreen/core/properties/cameraproperty.h \
    ../../include/voreen/core/properties/condition.h \
    ../../include/voreen/core/properties/eventproperty.h \
    ../../include/voreen/core/properties/filedialogproperty.h \
    ../../include/voreen/core/properties/floatproperty.h \
    ../../include/voreen/core/properties/fontproperty.h \
    ../../include/voreen/core/properties/intproperty.h \
    ../../include/voreen/core/properties/matrixproperty.h \
    ../../include/voreen/core/properties/numericproperty.h \
    ../../include/voreen/core/properties/optionproperty.h \
    ../../include/voreen/core/properties/property.h \
    ../../include/voreen/core/properties/propertyfactory.h \
    ../../include/voreen/core/properties/propertyowner.h \
    ../../include/voreen/core/properties/propertyvector.h \
    ../../include/voreen/core/properties/propertywidget.h \
    ../../include/voreen/core/properties/propertywidgetfactory.h \
    ../../include/voreen/core/properties/shaderproperty.h \
    ../../include/voreen/core/properties/stringexpressionproperty.h \
    ../../include/voreen/core/properties/stringproperty.h \
    ../../include/voreen/core/properties/targetaction.h \
    ../../include/voreen/core/properties/templateproperty.h \
    ../../include/voreen/core/properties/templatepropertycondition.h \
    ../../include/voreen/core/properties/transfuncproperty.h \
    ../../include/voreen/core/properties/vectorproperty.h \
    ../../include/voreen/core/properties/volumecollectionproperty.h \
    ../../include/voreen/core/properties/volumehandleproperty.h \
    ../../include/voreen/core/properties/voxeltypeproperty.h \
    ../../include/voreen/core/properties/link/buttonlinkevaluator.h \
    ../../include/voreen/core/properties/link/corelinkevaluatorfactory.h \
    ../../include/voreen/core/properties/link/dependencylinkevaluator.h \
    ../../include/voreen/core/properties/link/dependencylinkevaluators.h \
    ../../include/voreen/core/properties/link/linkevaluatorbase.h \
    ../../include/voreen/core/properties/link/linkevaluatorboolinvert.h \
    ../../include/voreen/core/properties/link/linkevaluatorfactory.h \
    ../../include/voreen/core/properties/link/linkevaluatorid.h \
    ../../include/voreen/core/properties/link/linkevaluatoridgeneric.h \
    ../../include/voreen/core/properties/link/linkevaluatoridnormalized.h \
    ../../include/voreen/core/properties/link/propertylink.h
HEADERS += \
    ../../include/voreen/core/utils/exception.h \
    ../../include/voreen/core/utils/glsl.h \
    ../../include/voreen/core/utils/hashing.h \
    ../../include/voreen/core/utils/observer.h \
    ../../include/voreen/core/utils/stringconversion.h \
    ../../include/voreen/core/utils/variant.h \
    ../../include/voreen/core/utils/voreenpainter.h \
    ../../include/voreen/core/utils/cmdparser/command.h \
    ../../include/voreen/core/utils/cmdparser/command_loglevel.h \
    ../../include/voreen/core/utils/cmdparser/commandlineparser.h \
    ../../include/voreen/core/utils/cmdparser/multiplecommand.h \
    ../../include/voreen/core/utils/cmdparser/predefinedcommands.h \
    ../../include/voreen/core/utils/cmdparser/singlecommand.h \
    ../../include/voreen/core/utils/GLSLparser/glslannotation.h \
    ../../include/voreen/core/utils/GLSLparser/glslprogram.h \
    ../../include/voreen/core/utils/GLSLparser/grammarsymbol.h \
    ../../include/voreen/core/utils/GLSLparser/lexer.h \
    ../../include/voreen/core/utils/GLSLparser/parser.h \
    ../../include/voreen/core/utils/GLSLparser/parseraction.h \
    ../../include/voreen/core/utils/GLSLparser/parsetreenode.h \
    ../../include/voreen/core/utils/GLSLparser/production.h \
    ../../include/voreen/core/utils/GLSLparser/symbol.h \
    ../../include/voreen/core/utils/GLSLparser/symboltable.h \
    ../../include/voreen/core/utils/GLSLparser/terminalsmap.h \
    ../../include/voreen/core/utils/GLSLparser/token.h \
    ../../include/voreen/core/utils/GLSLparser/annotations/annotationlexer.h \
    ../../include/voreen/core/utils/GLSLparser/annotations/annotationnodes.h \
    ../../include/voreen/core/utils/GLSLparser/annotations/annotationparser.h \
    ../../include/voreen/core/utils/GLSLparser/annotations/annotationterminals.h \
    ../../include/voreen/core/utils/GLSLparser/annotations/annotationvisitor.h \
    ../../include/voreen/core/utils/GLSLparser/generator/annotationgrammar.h \
    ../../include/voreen/core/utils/GLSLparser/generator/glslgrammar.h \
    ../../include/voreen/core/utils/GLSLparser/generator/grammar.h \
    ../../include/voreen/core/utils/GLSLparser/generator/lritem.h \
    ../../include/voreen/core/utils/GLSLparser/generator/parsertable.h \
    ../../include/voreen/core/utils/GLSLparser/generator/ppgrammar.h \
    ../../include/voreen/core/utils/GLSLparser/glsl/glslcondition.h \
    ../../include/voreen/core/utils/GLSLparser/glsl/glsldeclaration.h \
    ../../include/voreen/core/utils/GLSLparser/glsl/glslexpression.h \
    ../../include/voreen/core/utils/GLSLparser/glsl/glslexternaldeclaration.h \
    ../../include/voreen/core/utils/GLSLparser/glsl/glslfunctioncall.h \
    ../../include/voreen/core/utils/GLSLparser/glsl/glslfunctiondefinition.h \
    ../../include/voreen/core/utils/GLSLparser/glsl/glslfunctionprototype.h \
    ../../include/voreen/core/utils/GLSLparser/glsl/glsllexer.h \
    ../../include/voreen/core/utils/GLSLparser/glsl/glslparameter.h \
    ../../include/voreen/core/utils/GLSLparser/glsl/glslparser.h \
    ../../include/voreen/core/utils/GLSLparser/glsl/glslparsetreenode.h \
    ../../include/voreen/core/utils/GLSLparser/glsl/glslqualifier.h \
    ../../include/voreen/core/utils/GLSLparser/glsl/glslstatement.h \
    ../../include/voreen/core/utils/GLSLparser/glsl/glslstructspecifier.h \
    ../../include/voreen/core/utils/GLSLparser/glsl/glslsymbol.h \
    ../../include/voreen/core/utils/GLSLparser/glsl/glslterminals.h \
    ../../include/voreen/core/utils/GLSLparser/glsl/glsltranslation.h \
    ../../include/voreen/core/utils/GLSLparser/glsl/glsltoken.h \
    ../../include/voreen/core/utils/GLSLparser/glsl/glsltypespecifier.h \
    ../../include/voreen/core/utils/GLSLparser/glsl/glslvalue.h \
    ../../include/voreen/core/utils/GLSLparser/glsl/glslvariable.h \
    ../../include/voreen/core/utils/GLSLparser/glsl/glslvisitor.h \
    ../../include/voreen/core/utils/GLSLparser/preprocessor/ppexpression.h \
    ../../include/voreen/core/utils/GLSLparser/preprocessor/pplexer.h \
    ../../include/voreen/core/utils/GLSLparser/preprocessor/ppmacro.h \
    ../../include/voreen/core/utils/GLSLparser/preprocessor/ppparser.h \
    ../../include/voreen/core/utils/GLSLparser/preprocessor/ppparsetreenode.h \
    ../../include/voreen/core/utils/GLSLparser/preprocessor/ppstatement.h \
    ../../include/voreen/core/utils/GLSLparser/preprocessor/ppsymbol.h \
    ../../include/voreen/core/utils/GLSLparser/preprocessor/ppterminals.h \
    ../../include/voreen/core/utils/GLSLparser/preprocessor/ppvisitor.h \
    ../../include/voreen/core/utils/voreenblas/voreenblas.h \
    ../../include/voreen/core/utils/voreenblas/voreenblascpu.h \
    ../../include/voreen/core/utils/voreenblas/ellpackmatrix.h

HEADERS += \
    ../../ext/tinyxml/tinyxml.h \
    ../../ext/tinyxml/tinystr.h
HEADERS += \
    ../../ext/md5/md5.h \

HEADERS = $$unique(HEADERS)
SOURCES = $$unique(SOURCES)

####################################################
# Modules which can be enabled/disabled by defines
####################################################

# uncomment this for building a stripped library
# unix:QMAKE_POST_LINK=strip $(TARGET)

# this must come after all SOURCES, HEADERS and FORMS have been added
contains(DEFINES, VRN_WITH_SVNVERSION) : revtarget.depends = $$SOURCES $$HEADERS $$FORMS

# installation
unix {
  !isEmpty(INSTALL_PREFIX) {
    WORKSPACES = quadview simple standard walnut
    SCRIPTS = fps.py snapshot.py
    VOLUMES = nucleon.dat nucleon.raw walnut.dat walnut.raw
    TFS = nucleon.tfi walnut.tfi
    TEXTURES = anterior2.png axial_b.png axial_t.png coronal_b.png coronal_f.png \
               error.tga inferior2.png lateral2.png sagittal_l.png sagittal_r.png septal2.png

    FONTS = Vera.ttf VeraMono.ttf
    DOCS = CREDITS.txt Changelog.txt LICENSE-academic.txt LICENSE.txt readme.txt \
           doc/gettingstarted/

    target.path = $$INSTALLPATH_LIB

    shaders.path = $$INSTALLPATH_SHARE/shaders
    shaders.files += $$SHADER_SOURCES
    shader_modules.path = $$INSTALLPATH_SHARE/shaders/modules
    shader_modules.files += $$SHADER_SOURCES_MODS
    shader_utils.path = $$INSTALLPATH_SHARE/shaders/utils
    shader_utils.files += $$SHADER_SOURCES_UTILS

    for(i, WORKSPACES): WORKSPACES_FULL += $$join(i, " ", "../../data/workspaces/", ".vws")
    workspaces.path = $$INSTALLPATH_SHARE/workspaces
    workspaces.files += $$WORKSPACES_FULL

    for(i, SCRIPTS): SCRIPTS_FULL += $$join(i, " ", "../../data/scripts/")
    scripts.path = $$INSTALLPATH_SHARE/scripts
    scripts.files += $$SCRIPTS_FULL

    for(i, VOLUMES): VOLUMES_FULL += $$join(i, " ", "../../data/volumes/", "")
    volumes.path = $$INSTALLPATH_SHARE/volumes
    volumes.files += $$VOLUMES_FULL

    for(i, TFS): TFS_FULL += $$join(i, " ", "../../data/transferfuncs/", "")
    tfs.path = $$INSTALLPATH_SHARE/transferfuncs
    tfs.files += $$TFS_FULL

    for(i, TEXTURES): TEXTURES_FULL += $$join(i, " ", "../../data/textures/", "")
    textures.path = $$INSTALLPATH_SHARE/textures
    textures.files += $$TEXTURES_FULL

    for(i, FONTS): FONTS_FULL += $$join(i, " ", "../../data/fonts/", "")
    fonts.path = $$INSTALLPATH_SHARE/fonts
    fonts.files += $$FONTS_FULL

    for(i, DOCS): DOCS_FULL += $$join(i, " ", "../../", "")
    documentation.path = $$INSTALLPATH_DOC
    documentation.files += $$DOCS_FULL

    icons.path = $$INSTALLPATH_SHARE
    icons.files += ../../resource/vrn_share/icons/icon-64.png

    INSTALLS += target shaders shader_modules shader_utils workspaces scripts volumes \
                tfs textures fonts documentation icons
  }
}

