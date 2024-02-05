SUMMARY = "Central controll application of EV charger"
SECTION = "App"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

DEPENDS += " \
    json-c \
    dbus-wrapper \
"

SRC_URI = "file://src/smart_app.c \
            file://inc/log.h \
            file://inc/smart_app.h \
            file://CMakeLists.txt \
"

S = "${WORKDIR}"

inherit cmake

EXTRA_OECMAKE = ""

do_install() {
    install -d ${D}${bindir}
    install -m 0755 smart_app ${D}${bindir}
}
