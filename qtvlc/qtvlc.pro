TEMPLATE = lib

CONFIG += shared
QT -= gui

DEFINES += VLCQT_CORE_LIBRARY

TARGET = QtVlc
target.path = _install

include(../../config.pri)
!isEmpty(THE_3RD_LIB_PATH){
    TARGET = $$THE_3RD_LIB_PATH/$$TARGET
}
!isEmpty(LIB_INSTALL_PATH){
    target.path = $$LIB_INSTALL_PATH
}
VERSION = 0.1

include(qtvlc.pri)

#INSTALLS += target
