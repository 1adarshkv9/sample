SUMMARY = "CAN Application"
SECTION = "App"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

DEPENDS += " \
    json-c \
    libsocketcan \
    dbus-wrapper \
"

SRC_URI = "file://src/can_app.c \
            file://inc/can_app.h \
            file://inc/log.h \
            file://src/can.c \
            file://inc/can.h \
            file://CMakeLists.txt \
"

S = "${WORKDIR}"

inherit cmake

EXTRA_OECMAKE = ""

do_install() {
    install -d ${D}${bindir}
    install -m 0755 can_app ${D}${bindir}
}
