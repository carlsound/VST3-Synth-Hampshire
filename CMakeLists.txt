
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
    include/hampshireGlobalParameterState.h
    source/hampshireGlobalParameterState.cpp
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
    include/voiceBase.h
    #---
    include/voiceProcessor.h
    #---
    include/hampshireVoiceProcessorImplementation32.h
    include/hampshireVoiceProcessorImplementation64.h
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
if(MAC)
	set(target Hampshire)
elseif((WIN) AND (CMAKE_SIZEOF_VOID_P EQUAL "8"))
    set(target Hampshire_x64)
elseif(WIN)
	set(target Hampshire_x86)
endif()

smtg_add_vst3plugin(${target} ${SDK_ROOT} ${synth_sources})
set_target_properties(${target} PROPERTIES ${SDK_IDE_MYPLUGINS_FOLDER})
target_link_libraries(${target} PRIVATE base sdk)

if(MAC)
    smtg_set_bundle(${target} INFOPLIST "${CMAKE_CURRENT_LIST_DIR}/resource/Info.plist" PREPROCESS)
elseif(WIN)
    target_sources(${target} PRIVATE resource/plug.rc)
endif()