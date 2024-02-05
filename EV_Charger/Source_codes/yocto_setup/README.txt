HOST Machine setup:
*******************
UBUNTU 18.04 is prefered. Install the below pre-requisite packages
    $ sudo apt-get update
    $ sudo apt-get -y install git-core flex bison gperf libesd0-dev zip libwxgtk2.6-dev zlib1g-dev build-essential gettext texinfo
    $ sudo apt-get -y install fakeroot gnupg libsdl1.2-dev squashfs-tools ubootmkimage expect libncurses5-dev
    $ sudo apt-get -y install minicom lrzsz tftpd-hpa nfs-kernel-server nfs-common portmap patch vim gawk
    $ sudo apt-get -y install xinetd tftpd tftp qtcreator ctags quilt 
    $ sudo apt-get -y install gawk wget git-core diffstat unzip texinfo gcc-multilib build-essential chrpath socat libsdl1.2-dev xterm


Source code Downloading:
************************
Step-1: Create a directory
    $ mkdir ~/yocto
    $ cd ~/yocto

Step-2: Download the PHYTEC BSP Management tool to setp the PHYTEC Yocto
    $ wget ftp://ftp.phytec.de/pub/Software/Linux/Yocto/Tools/phyLinux

Step-3: Provide the execution permission for the above tool and run to initialize the Yocto setup
    $ chmod +x phyLinux
    $ ./phyLinux init

Step-4: While initializing the repo for Yocto source, we need to selet the following option
    # Slect the SOC platform as" imx6ul"
    # Choose "BSP-Yocto-i.MX6UL-PD19.1.0" from the available releases
    # Select the below one from the avilable builds
         phyboard-segin-imx6ul-2: PHYTEC phyBOARD-Segin i.MX6 UltraLite
                                  512MB RAM, NAND
                                  PB-02013-001.A2, PB-02013-110I.A2, PCL-063-23300CI.A2
                                  distro: yogurt
                                  target: phytec-qt5demo-image
        
Now the Yocto source code is downloaded. 


Configuring the Bitbake layers:
*******************************
Step-1: Download the meta layer for RB-i.MX6UL
    $ cd sources
    $ git clone https://github.com/kishorephytec/meta-rbimx6ul.git

Step-2: Checkout the repective branch as per the dowloaded release of the yocto source setup. In the above case, we have downloaded "BSP-Yocto-i.MX6UL-PD19.1.0" release and hence "RB-IMX6UL_PD19.1.0_1P2" branch has to be checkedout 
    $ cd meta-rbimx6ul/
    $ git checkout RB-IMX6UL_PD19.1.0_1P2

Step-3: Configure the meta-rbimx6ul in the bblayers configuration
    $ cd ../../
    $ source sources/poky/oe-init-build-env
    $ bitbake-layers add-layer ../sources/meta-rbimx6ul/
    $ bitbake-layers add-layer ../sources/meta-rbimx6ul/meta-bsp/
    $ bitbake-layers add-layer ../sources/meta-rbimx6ul/meta-core/
    $ bitbake-layers add-layer ../sources/meta-rbimx6ul/meta-libraries/
    $ bitbake-layers add-layer ../sources/meta-rbimx6ul/meta-services/
    $ bitbake-layers add-layer ../sources/meta-rbimx6ul/meta-utils/

Step-4: Change the machine name in the conf/local.conf. Modify MACHINE ?= "phyboard-segin-imx6ul-2" to MACHINE ?= "ruggedboard-imx6ul"
    $ vi conf/local.conf

With this, meta-rbimx6ul layer is configured.


Applying IISC EV charger Patches:
************************************
Setp-1: Go to meta-rbimx6ul directory
	$ cd ~/yocto/sources/meta-rbimx6ul/

Step-2: Apply the patches as follows
	$ git am 0001-IISC-EV-charger-Application.patch


Building and generating the Images:
***********************************
After you downloaded all the metadata with phyLinux init, you have to set up the shell environment variables. 
This needs to be done every time you open a new shell for starting builds. We use the shell script provided by Poky in its default configuration. 

Step-1: Come back to root of your project, in our case "yocto"
    $ cd ~/yocto

step-2: set up the shell environment variables
    $ source sources/poky/oe-init-build-env

Step-3: Now you are ready to build your first image.
    $ bitbake rb-qt5demo-image

Here,
rb-qt5demo-image - Graphical images consisting Qt and Gstremer plugins and other graphic related Stacks and packages
rb-headless-image - smaller non-graphical image

More info and Documents can be found at https://www.phytec.in/software/board-support-packages/yocto-buildsystem/bsp-seite/?bsp=BSP-Yocto-i.MX6UL-PD19.1.0