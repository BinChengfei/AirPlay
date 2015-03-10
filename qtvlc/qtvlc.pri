INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS         += $$PWD/Audio.h\
                                                $$PWD/Common.h\
                                                $$PWD/Enums.h\
                                                $$PWD/Error.h\
                                                $$PWD/Instance.h\
                                                $$PWD/Media.h\
                                                $$PWD/MediaList.h\
                                                $$PWD/MediaListPlayer.h\
                                                $$PWD/MediaPlayer.h\
                                                $$PWD/MetaManager.h\
                                                $$PWD/SharedExportCore.h\
                                                $$PWD/Video.h\
                                                $$PWD/VideoDelegate.h\
                                                $$PWD/Config.h

SOURCES			+= $$PWD/Audio.cpp\
                                                $$PWD/Common.cpp\
                                                $$PWD/Enums.cpp\
                                                $$PWD/Error.cpp\
                                                $$PWD/Instance.cpp\
                                                $$PWD/Media.cpp\
                                                $$PWD/MediaList.cpp\
                                                $$PWD/MediaListPlayer.cpp\
                                                $$PWD/MediaPlayer.cpp\
                                                $$PWD/MetaManager.cpp\
                                                $$PWD/Video.cpp\
                                                $$PWD/VideoDelegate.cpp

LIBS += -lvlc

# moc doesn't detect Q_OS_LINUX correctly, so add this to make it work
linux*:DEFINES += __linux__
