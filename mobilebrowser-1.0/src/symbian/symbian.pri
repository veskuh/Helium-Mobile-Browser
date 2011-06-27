# Symbian specifics
INCLUDEPATH += $$PWD

symbian {
    HEADERS += $$PWD/sym_iap_util.h
    LIBS += -lesock \
        -linsock \
        -lconnmon
    TARGET.CAPABILITY = NetworkServices
    TARGET.EPOCHEAPSIZE = 0x20000 \
        0x2000000
    TARGET.UID3 = 0xE55B478A
}
