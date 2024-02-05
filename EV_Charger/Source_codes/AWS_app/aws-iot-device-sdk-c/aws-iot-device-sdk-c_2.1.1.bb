SUMMARY = "aws-iot-device-sdk-c recipe in yocto"
HOMEPAGE = "https://github.com/aws/aws-iot-device-sdk-embedded-C"
SECTION = "base"
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://LICENSE.txt;md5=acc7a1bf87c055789657b148939e4b40"

DEPENDS = "mbedtls"
SRC_URI  = "https://github.com/aws/aws-iot-device-sdk-embedded-C/archive/refs/tags/v2.1.1.zip \
            file://aws_iot_config.h \
            file://subscribe_publish_sample.c \
            file://certs/ \
"
#for 2.1.1
#SRCREV="70071112bd5e1c5b9f150894fafe199637b4f63a"
#SRCREV="8206cb292ed96eb6ba11afa418cb39d60caa114b"

SRC_URI[md5sum] = "96077e39f51cd9ca544495bbda555d8b"
#SRC_URI[sha256sum] = "35bd3d18a9d4d6a76e93370693576d559e6892708b996b44b0fd9e4cada904fe"

# for cpputest
SRC_URI += "https://github.com/cpputest/cpputest/archive/v3.6.tar.gz;md5sum=08ea8185fb0bb52d952c1a71ac9be831"
SRC_URI[v3.6.tar.gz.md5sum] = "08ea8185fb0bb52d952c1a71ac9be831"
SRC_URI[v3.6.tar.gz.sha256sum] = "1d5b745322f36569536bad2dd41fe0e74b208da6475d6a1bfb0c2cb1adeefbbb"

#SRC_URI += "https://github.com/ARMmbed/mbedtls/archive/mbedtls-2.1.1.tar.gz;md5sum=6f5d3e7154ce4e04bcb9b299f614775f"
#SRC_URI[mbedtls-2.1.1.tar.gz.md5sum] = "6f5d3e7154ce4e04bcb9b299f614775f"
#SRC_URI[mbedtls-2.1.1.tar.gz.sha256sum] = "ae458a4987f36819bdf1d39519212f4063780fe448d4155878fccf4e782a715f"


SRC_URI += "https://github.com/ARMmbed/mbedtls/archive/mbedtls-2.14.0.tar.gz;md5sum=1fb29c16adf56dbd98b583d053f6edbf"
SRC_URI[mbedtls-2.1.1.tar.gz.md5sum] = "1fb29c16adf56dbd98b583d053f6edbf"
SRC_URI[mbedtls-2.1.1.tar.gz.sha256sum] = "3e733731eca47ffdc85330cde47774e075f5d1326e413555713dec8bf9c31eb3"

S="${WORKDIR}/aws-iot-device-sdk-embedded-C-2.1.1"
B = "${WORKDIR}/build"
EXTRA_OEMAKE = "'CC=${CC}' 'RANLIB=${RANLIB}' 'ar=${AR}' 'AR=${AR}' 'CFLAGS=${CFLAGS} -I${S}/include -DWITHOUT_XATTR' "

do_compile(){

    cp -rf ${S}/../cpputest-3.6/* ${S}/external_libs/CppUTest/
    #cp -rf ${S}/../mbedtls-mbedtls-2.1.1/* ${S}/external_libs/mbedTLS/
    cp -rf ${S}/../mbedtls-mbedtls-2.14.0/* ${S}/external_libs/mbedTLS/
    cp ${S}/../aws_iot_config.h ${S}/samples/linux/subscribe_publish_sample/
    #cp ${s}/../subscribe_publish_sample.c ${S}/samples/linux/subscribe_publish_sample/

    cd ${S}/samples/linux/subscribe_publish_sample/ 
    oe_runmake
}
do_install(){

    # Samples
    install -d ${D}${datadir}/awsiotsdk/samples/embedded-c/pub_sub
    install -d ${D}${datadir}/awsiotsdk/samples/embedded-c/config
    install -d ${D}${datadir}/awsiotsdk/certs
    
    install ${S}/samples/linux/subscribe_publish_sample/subscribe_publish_sample ${D}${datadir}/awsiotsdk/samples/embedded-c/pub_sub
    install ${S}/../certs/* ${D}${datadir}/awsiotsdk/certs
    install ${S}/../aws_iot_config.h ${D}${datadir}/awsiotsdk/samples/embedded-c/config
}

INSANE_SKIP_${PN} += "ldflags"

FILES_${PN}-samples = "\
    ${datadir}/awsiotsdk/samples/embedded-c/* \
"

FILES_${PN} = "\
    ${datadir}/awsiotsdk/certs/* \
    ${datadir}/awsiotsdk/samples/embedded-c/config/aws_iot_config.h \
    ${datadir}/awsiotsdk/samples/embedded-c/pub_sub/subscribe_publish_sample \
"
