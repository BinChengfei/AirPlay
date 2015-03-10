TEMPLATE = lib

CONFIG += shared
QT -= gui

DEFINES += QT_QTSINGLEAPPLICATION_EXPORT

TARGET = QtSingleApplication
target.path = _install

include(../../config.pri)
!isEmpty(THE_3RD_LIB_PATH){
    TARGET = $$THE_3RD_LIB_PATH/$$TARGET
}
!isEmpty(LIB_INSTALL_PATH){
    target.path = $$LIB_INSTALL_PATH
}
VERSION = 0.1

include(qtsinglecoreapplication.pri)

#INSTALLS += target
