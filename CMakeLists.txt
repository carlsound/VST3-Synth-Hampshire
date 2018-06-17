
set(synth_sources
    include/hampshireController.h
    source/hampshireController.cpp
    #---
    include/hampshireIDs.h
    #---
    include/hampshireProcessor.h
    source/hampshireProcessor.cpp
    #---
    include/version.h
    #---
    source/hampshireFactory.cpp
    #---
    include/hampshireGlobalParameterStorage.h
    source/hampshireGlobalParameterStorage.cpp
    #--
    include/hampshireVoice.h
    source/hampshireVoice.cpp
    #---
    include/hampshireVoiceStatics.h
    source/hampshireVoiceStatics.cpp
    #---
    include/hampshireVoiceStaticsOnce.h
    source/hampshireVoiceStaticsOnce.cpp
    #---
    #---include/panNoteExpressionType.h
    #---
    #---include/releaseTimeModNoteExpressionType.h
    #---
    #---https://github.com/micknoise/Maximilian
    include/maximilian.h
    include/maximilian.cpp
)

#--- HERE change the target Name for your plug-in (for ex. set(target myDelay))-------
set(target Hampshire)

smtg_add_vst3plugin(${target} ${SDK_ROOT} ${synth_sources})
set_target_properties(${target} PROPERTIES ${SDK_IDE_MYPLUGINS_FOLDER})
target_link_libraries(${target} PRIVATE base sdk)

if(MAC)
    smtg_set_bundle(${target} INFOPLIST "${CMAKE_CURRENT_LIST_DIR}/resource/Info.plist" PREPROCESS)
elseif(WIN)
    target_sources(${target} PRIVATE resource/plug.rc)
endif()
