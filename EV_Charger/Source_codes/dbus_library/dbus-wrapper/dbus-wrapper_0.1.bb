SUMMARY = "dbus wrapper library"
SECTION = "dbus"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

DEPENDS += " \
    json-c \
    libsocketcan \
    dbus-glib\
"

SRC_URI = "\
            file://CMakeLists.txt \
            file://inc/dbus_interface.h\
            file://inc/dbus_wrapper.h\
            file://src/dbus_wrapper.c\
"

S = "${WORKDIR}"

EXTRA_OECMAKE = " "

INHIBIT_PACKAGE_STRIP = "1"
INHIBIT_SYSROOT_STRIP = "1"
SOLIBS = ".so*"
SOLIBSDEV += ".so"
FILES_SOLIBSDEV = ""

inherit pkgconfig cmake

do_install() {
    install -d ${D}${libdir}
    install -m 0755 libdbus-wrapper.so ${D}${libdir}

    install -d ${D}${includedir}
    install -m 0755 ${S}/inc/dbus_interface.h ${D}${includedir}
    install -m 0755 ${S}/inc/dbus_wrapper.h ${D}${includedir}
}


FILES_${PN} += "${libdir} ${includedir}"
