#!/bin/bash
clear
echo "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
echo "+"
echo "+  flash.sh"
echo "+"
echo "+ This script creates the file(s) you need to run the image built last"
echo "+ in the receiver's flash memory, the internal hard disk or from a USB"
echo "+ stick."
echo "+"
echo "+ Author : Audioniek, based on previous work by schishu, bpanther"
echo "+          and others."
echo "+ Date   : 29-10-2022"
echo "+"
echo "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
echo
# ---------------------------------------------------------------------------
# Changes:
# 20140726 Audioniek   Setting own language on HS8200 did not work; moved
#                      upward.
# 20140726 Audioniek   French added as third fixed language on hs8200.
# 20140831 Audioniek   Neutrino flash for Fortis 1st generation receivers
#                      added (requires latest fup).
# 20140906 Audioniek   Tangos neutrino added.
# 20140907 Audioniek   Neutrino flash for HS7110 & HS7810A receivers added.
# 20140912 Audioniek   Flash for HS7119 & HS7819 receivers added.
# 20140914 Audioniek   Corrected some typos, add all languages option for
#                      HS8200.
# 20141015 Audioniek   Fortis 4th generation receivers added.
# 20141208 Audioniek   Bug fixed with Fortis dp6010.
# 20150806 Audioniek   Tvheadend added.
# 20150911 Audioniek   Exit when building Topfield installer fails.
# 20160416 Audioniek   ./lastChoice handling adapted to handle E2..
#                      environment variables.
# 20160512 Audioniek   Check for E2 in flash for hs7420 was missing.
# 20161126 Audioniek   Adapted to work with cdk_new as well.
# 20161205 Audioniek   Fixed tfinstaller with cdk_new.
# 20161216 Audioniek   Skip output type selection if built with cdk_new
#                      and destination already set.
# 20161216 Audioniek   Enable USB for Fortis 2G receivers with 32MB of flash.
# 20161217 Audioniek   Enable USB for Fortis 3G receivers.
# 20170207 Audioniek   Improved check on existence of a flashable image.
# 20170211 Audioniek   Support for buildsystem added.
# 20170310 Audioniek   Support for Kathrein UFS912 improved/debugged.
# 20170922 Audioniek   Support for Kathrein UFS910 Neutrino USB added.
# 20171223 Audioniek   Support for Tvheadend built by buildsystem added.
# 20180114 Audioniek   Handle tfinstaller built from buildsystem.
# 20190113 Audioniek   Add batch mode.
# 20190208 Audioniek   Flash layout changed for Neutrino on CubeRevo's.
# 20190326 Audioniek   All CubeRevo USB changed to make_tar_gz.
# 20190518 Audioniek   Add vitamin_hd5000.
# 20190530 Audioniek   Add adb_box.
# 20190828 Audioniek   adb_box also strips languages on flash.
# 20191103 Audioniek   Rename Patches directory to patches.
# 20191208 Audioniek   Add Fortis DP2010.
# 20191214 Audioniek   Fix Linux version display.
# 20191214 Audioniek   More precise companion file checking.
# 20191222 Audioniek   STAPI companion files moved to /root/modules.
# 20200116 Audioniek   Add Fortis DP7000 and Fortis 4G USB.
# 20200402 Audioniek   Update neutrino: remove -mp.
# 20200609 Audioniek   dp6010 -> fx6010.
# 20200620 Audioniek   Fix error with default resellerID for dp7001.
# 20200620 Audioniek   Add Edision argus VIP1 and VIP2 USB.
# 20200620 Audioniek   Add Edision argus VIP1 V1.
# 20200915 Audioniek   Add Fortis DP7050, EP8000 and GPV8000.
# 20201017 Audioniek   Force output type to USB if buildsystem config
#                      contains DESTINATION=USB.
# 20201201 Audioniek   Add Opticum HD 9600 (TS).
# 20210226 Audioniek   Remove Tvheadend support.
# 20210316 Audioniek   Fix small problem with cuberevo_mini2.
# 20210405 Audioniek   Strip languages on UFS913 added.
# 20210423 Audioniek   atevio7500 -> hs8200.
# 20210426 Audioniek   octagon1008 -> hs9510.
# 20210523 Audioniek   Add cuberevo_mini_fta.
# 20210601 Audioniek   Corrected two Octagon resellerID's.
# 20210604 Audioniek   Strip languages on UFS912 added.
# 20210609 Audioniek   Add UFS913 USB.
# 20210704 Audioniek   Fix receivertypes with a minus in their boxtype.
# 20210714 Audioniek   Allow Enigma2 "flash" for ufs910.
# 20210910 Audioniek   Add Atemio AM 520 HD.
# 20210910 Audioniek   Add Opticum HD 9600 Mini.
# 20220127 Audioniek   Add Opticum HD 9600 (TS) Prima.
# 20220628 Audioniek   Fix unsetting RESELLERID with Atemio AM 520 HD.
# 20220719 Audioniek   Add Atemio AM 530 HD, titan for AM 520 HD.
# 20220806 Audioniek   Indicate default choice differently.
# 20220827 Audioniek   Add Homecast HS8100 series.
# 20221029 Audioniek   Add Homecast HS9000/9100 series.
# 20221127 Audioniek   Better handling of USB plus built-in hard disk.
# ---------------------------------------------------------------------------

# Set up some variables
export CURDIR=`pwd`
export BASEDIR=`cd .. && pwd`
export TUFSBOXDIR=$BASEDIR/tufsbox
CDKDIR=$BASEDIR
export CDKDIR
export FLASHDIR=$BASEDIR/flash
export SCRIPTDIR=$FLASHDIR/scripts
export TOOLSDIR=$FLASHDIR/flash_tools
export TMPDIR=$FLASHDIR/tmp
export TMPROOTDIR=$TMPDIR/ROOT
export TMPVARDIR=$TMPDIR/VAR
export TMPFWDIR=$TMPDIR/FW
export TMPKERNELDIR=$TMPDIR/KERNEL
export OUTDIR=$FLASHDIR/out
export TFINSTALLERDIR=$CDKDIR/tfinstaller

# Check if an image was actually built
# built from buildsystem:  config, ./deps/build_complete and release directory should exist
if [ ! -e $CDKDIR/config ] \
|| [ ! -e $CDKDIR/.deps/build_complete ] \
|| [ ! -d $BASEDIR/tufsbox/release ]; then
  echo "-- PROBLEM! -----------------------------------------------------------"
  echo
  echo " Please build an image first. Exiting..."
  echo
  echo "-----------------------------------------------------------------------"
  export ERROR="yes"
  exit
fi

if [ `id -u` != 0 ]; then
  echo
  echo "-- PROBLEM! -----------------------------------------------------------"
  echo
  echo " You are not running this script with sudo or fakeroot."
  echo " Try it again with \"sudo $0\" in case of an USB image."
  echo " For a flash image \"fakeroot $0\" will do fine."
  echo
  echo " Exiting..."
  echo
  echo "-----------------------------------------------------------------------"
  export ERROR="yes"
  exit
fi

# Create/cleanup work directory structure
if [ -e $TMPDIR ]; then
  rm -rf $TMPDIR/*
elif [ ! -d $TMPDIR ]; then
  mkdir $TMPDIR
fi

if [ -e $TMPROOTDIR ]; then
  rm -rf $TMPROOTDIR/*
elif [ ! -d $TMPROOTDIR ]; then
  mkdir -p $TMPROOTDIR
fi

if [ -e $TMPVARDIR ]; then
  rm -rf $TMPVARDIR/*
elif [ ! -d $TMPVARDIR ]; then
  mkdir -p $TMPVARDIR
fi

if [ -e $TMPKERNELDIR ]; then
  rm -rf $TMPKERNELDIR/*
elif [ ! -d $TMPKERNELDIR ]; then
  mkdir -p $TMPKERNELDIR
fi

if [ -e $TMPFWDIR ]; then
  rm -rf $TMPFWDIR/*
elif [ ! -d $TMPFWDIR ]; then
  mkdir -p $TMPFWDIR
fi

if [ -e $OUTDIR ]; then
  rm -rf $OUTDIR/*
elif [ ! -d $OUTDIR ]; then
  mkdir -p $OUTDIR
fi

# Evaluate -b / --batchmode command line parameter
set="$@"
for i in $set;
do
  if [ $"$i" == -b ] || [ $"$i" == --batchmode ]; then
    shift
    BATCH_MODE="yes"
    break
  fi
done
export BATCH_MODE

# Set image name
if [ "$BATCH_MODE" == "yes" ]; then
  INAME=Audioniek
  INAME="$INAME"_
else
  INAME=
fi
export INAME

# Determine which image has been built last
cp $CDKDIR/config $FLASHDIR/config
if [ `grep -e "IMAGE=enigma2" $FLASHDIR/config` ]; then
  IMAGE=enigma2
  IMAGEN="Enigma2"
elif [ `grep -e "IMAGE=neutrino" $FLASHDIR/config` ]; then
  IMAGE=neutrino
  IMAGEN="Neutrino"
elif [ `grep -e "IMAGE=titan" $FLASHDIR/config` ]; then
  IMAGE=titan
  IMAGEN="Titan"
fi
export IMAGE
export IMAGEN

# Determine receiver type
export BOXTYPE=`grep -e "BOXTYPE" $FLASHDIR/config | awk '{print substr($0,9)}'`

# Determine media framework
MFW=`grep -e "MEDIAFW=" $FLASHDIR/config | awk '{print substr($0,9)}'`
if [ "$MFW" == "gstreamer" ]; then
  MEDIAFW=gst
elif [ "$MFW" == "eplayer3" ]; then
  MEDIAFW=epl3
elif [ "$MFW" == "gst-eplayer3" ]; then
  MEDIAFW=gst-epl3
elif [ "$MFW" == "gst-eplayer3-dual" ]; then
  MEDIAFW=dual
else
  MEDIAFW=builtin
fi
export MEDIAFW

# Determine patch level and last part of linux version number
export PATCH=`grep -e "KERNEL_STM=p0" ./config | awk '{print substr($0,length($0)-2,length($0))}'`
FNAME="0$PATCH"_"$BOXTYPE"
cd $CDKDIR/patches/build-$IMAGE
ls $CDKDIR/build_tmp > $FLASHDIR/lastconfig
cd $CURDIR
export SUBVERS=`grep -e "linux-sh4-2.6.32." $FLASHDIR/lastconfig | awk '{print substr($0,length($0)-12,2)}'`
rm $FLASHDIR/lastconfig

# Determine/ask for output type (USB or flash)
if [ "$BATCH_MODE" == "yes" ]; then
  if [ `grep -e "DESTINATION=USB" $FLASHDIR/config` ]; then
    export OUTTYPE="USB"
  else
    export OUTTYPE="flash"
  fi
  export ON_HDD=
  if [ `grep -e "DESTINATION=USB_HDD" $FLASHDIR/config` ]; then
    export ON_HDD="(with built-in hard disk)"
  fi
  rm $FLASHDIR/config
else
  if [ `grep -e "DESTINATION=USB" $FLASHDIR/config` ]; then
    export OUTTYPE="USB"
  else
    echo "-- Output destination -------------------------------------------------"
    echo
    echo " Where would you like your $IMAGEN image to run?"
    echo "   1)  on a USB stick"
    echo "   2*) in the receivers flash memory"
    read -p " Select target (1-2)? "
    case "$REPLY" in
      1) export OUTTYPE="USB";;
      *) export OUTTYPE="flash";;
    esac
  fi
fi

# Check if the receiver can accept an Enigma2 image in flash
if [ "$IMAGE" == "enigma2" ] && [ "$OUTTYPE" == "flash" ] && [ ! "$BATCH_MODE" == "yes" ]; then
  case "$BOXTYPE" in
    fs9000|hs9510|hs7110|hs7420|hs7810a|cuberevo|cuberevo_mini|cuberevo_mini2|cuberevo_250hd|cuberevo_mini_fta|cuberevo_2000hd|cuberevo_3000hd|cuberevo_9500|hl101|vip1_v1|vip1_v2|vip2|opt9600|opt9600mini|opt9600prima|hchs8100|hchs9000)
      echo
      echo "-- Message ------------------------------------------------------------"
      echo
      echo " Sorry, Enigma2 requires more flash memory than available on your"
      echo " $BOXTYPE receiver."
      echo
      case "$BOXTYPE" in
        fs9000|hs9510|hs7110|hs7420|hs7810a|cuberevo|cuberevo_mini|cuberevo_mini2|cuberevo_250hd|cuberevo_mini_fta|cuberevo_2000hd|cuberevo_3000hd|cuberevo_9500)
          echo " Consider running Enigma2 from a USB stick or building Neutrino/Titan.";;
#        ufs910||hl101|vip1_v1|vip1_v2|vip2|opt9600|opt9600mini|opt9600prima)
        *)
          echo " Consider running Enigma2 from a USB stick.";;
      esac
      echo
      echo " Exiting..."      
      echo "-----------------------------------------------------------------------"
      export ERROR="yes"
      exit;;
  esac
fi

# Check if there is support for the receiver combined with imagetype
if [ "$IMAGE" == "enigma2" ] && [ "$OUTTYPE" == "USB" -o "$OUTTYPE" == "USB_HDD" ]; then
  case "$BOXTYPE" in
    spark|spark7162)
      echo
      echo "-- Message ----------------------------------------------------------------"
      echo
      echo " Currently there is no enigma2-USB support for your receiver $BOXTYPE."
      echo
      echo " Sorry."
      echo
      echo " Exiting..."
      echo "---------------------------------------------------------------------------"
      export ERROR="yes"
      exit;;
    *)
      ;;
  esac
elif [ "$IMAGE" == "neutrino" ] && [ "$OUTTYPE" == "USB" -o "$OUTTYPE" == "USB_HDD" ]; then
  case "$BOXTYPE" in
    hs8200|cuberevo|cuberevo_mini|cuberevo_mini2|cuberevo_250hd|cuberevo_mini_fta|fs9000|hs9510|hs7110|hs7420|hs7810a|hs7119|hs7429|hs7819|spark|spark7162|ufc960|ufs910|ufs912|ufs913|ufs922|vip1_v1|vip1_v2|vip2|opt9600|opt9600mini|opt9600prima|hchs8100|hchs9000)
      ;;
    *)
      echo
      echo "-- Message ----------------------------------------------------------------"
      echo
      echo " Currently there is no Neutrino-USB support for your receiver $BOXTYPE."
      echo
      echo " Sorry."
      echo
      echo " Exiting..."
      echo "---------------------------------------------------------------------------"
      export ERROR="yes"
      exit;
  esac
elif [ "$IMAGE" == "titan" ] && [ "$OUTTYPE" == "flash" ]; then
  case "$BOXTYPE" in
    atemio520|hs8200|hs7429)
      ;;
    *)
      echo
      echo "-- Message ----------------------------------------------------------------"
      echo
      echo " Currently there is no Titan-flash support for your receiver $BOXTYPE."
      echo
      echo " Sorry."
      echo
      echo " Exiting..."
      echo "---------------------------------------------------------------------------"
      export ERROR="yes"
      exit;
  esac
elif [ "$IMAGE" == "titan" ] && [ "$OUTTYPE" == "USB" -o "$OUTTYPE" == "USB_HDD" ]; then
  case "$BOXTYPE" in
    hs8200)
      ;;
    *)
      echo
      echo "-- Message ----------------------------------------------------------------"
      echo
      echo " Currently there is no Titan-USB support for your receiver $BOXTYPE."
      echo
      echo " Sorry."
      echo
      echo " Exiting..."
      echo "---------------------------------------------------------------------------"
      export ERROR="yes"
      exit;
  esac
fi

# Check if the required flash tool programs are there; if not, compile them
$SCRIPTDIR/create_flash_tools.sh $TOOLSDIR

# Determine receiver host name
if [ -f $TMPROOTDIR/etc/hostname ]; then
  HOST=`cat $TMPROOTDIR/etc/hostname`
elif [ -f $TMPROOTDIR/var/etc/hostname ]; then
  HOST=`cat $TMPROOTDIR/var/etc/hostname`
else
  HOST=$BOXTYPE
fi
export HOST

# Determine Neutrino GIT version 
if [ "$IMAGE" == "neutrino" ]; then
  if [ -d $BASEDIR/build_source/libstb-hal-ddt ]; then
    HAL_REV=_HAL-rev`cd $BASEDIR/build_source/libstb-hal-ddt && git log | grep "^commit" | wc -l`
  elif [ -d $BASEDIR/build_source/libstb-hal-github ]; then
    HAL_REV=_HAL-rev`cd $BASEDIR/build_source/libstb-hal-github && git log | grep "^commit" | wc -l`
  elif [ -d $BASEDIR/build_source/libstb-hal-martii-github ]; then
    HAL_REV=_HAL-rev`cd $BASEDIR/build_source/libstb-hal-martii-github && git log | grep "^commit" | wc -l`
  elif [ -d $BASEDIR/build_source/libstb-hal-tangos ]; then
    HAL_REV=_HAL-rev`cd $BASEDIR/build_source/libstb-hal-tangos && git log | grep "^commit" | wc -l`
  elif [ -d $BASEDIR/build_source/libstb-hal ]; then
    HAL_REV=_HAL-rev`cd $BASEDIR/build_source/libstb-hal && git log | grep "^commit" | wc -l`
  elif [ -d $BASEDIR/build_source/neutrino-hd2.git ]; then
    HAL_REV=
  else
    HAL_REV=_HAL-revXXX
  fi

  if [ -d $BASEDIR/build_source/neutrino-ddt ]; then
    NMP_REV=_NMP-rev`cd $BASEDIR/build_source/neutrino-ddt && git log | grep "^commit" | wc -l`-ddt
  elif [ -d $BASEDIR/build_source/neutrino-github ]; then
    NMP_REV=_NMP-rev`cd $BASEDIR/build_source/neutrino-github && git log | grep "^commit" | wc -l`-github
  elif [ -d $BASEDIR/build_source/neutrino-martii-github ]; then
    NMP_REV=_NMP-rev`cd $BASEDIR/build_source/neutrino-martii-github && git log | grep "^commit" | wc -l`-martii-github
  elif [ -d $BASEDIR/build_source/neutrino-tangos ]; then
    NMP_REV=_NMP-rev`cd $BASEDIR/build_source/neutrino-tangos && git log | grep "^commit" | wc -l`-tangos
  elif [ -d $BASEDIR/build_source/neutrino ]; then
    NMP_REV=_NMP-rev`cd $BASEDIR/build_source/neutrino && git log | grep "^commit" | wc -l`
  elif [ -d $BASEDIR/build_source/neutrino-hd2.git ]; then
    NMP_REV=_NMP-rev`cd $BASEDIR/build_source/neutrino-hd2.git && git log | grep "^commit" | wc -l`-hd2
  else
    NMP_REV=_NMP-revXXX
  fi
fi
export GITVERSION=CDK-rev`(cd $CDKDIR && git log | grep "^commit" | wc -l)`"$HAL_REV""$NMP_REV"

# Build tfinstaller if not done yet
TFINSTALL="present"
if [ $BOXTYPE == "tf7700" ]; then
    TFINSTALL="built"
fi
UFSINSTALL="present"
if [ $BOXTYPE == "ufs910" -o $BOXTYPE == "ufs922" ]; then
    UFSINSTALL="built"
fi

# All is OK so far, display summary
if [ ! "$BATCH_MODE" == "yes" ]; then
  clear
  echo "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
  echo "+"
  echo "+  Summary"
  echo "+  ======="
  echo "+"
  echo "+  Receiver           : $BOXTYPE"
  if [ $BOXTYPE == "tf7700" ]; then
    echo "+  Topfield installer : $TFINSTALL"
  fi
  if [ $BOXTYPE == "ufs910" -o $BOXTYPE == "ufs922" ]; then
    echo "+  UFS installer      : $UFSINSTALL"
  fi
  echo "+  Linux version      : linux-sh4-2.6.32.$SUBVERS"
  echo "+  Kernel patch level : P0$PATCH"
  echo "+  Image              : $IMAGEN"
  echo "+  Will run in/on     : $OUTTYPE $ON_HDD"
  echo "+"
  echo "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
  echo
fi

# Prepare root
echo "-- Prepare root -------------------------------------------------------"
echo
echo " Prepare $IMAGEN root for $BOXTYPE."
echo
if [[ ( "$BOXTYPE" == "adb_box" || "$BOXTYPE" == "hs8200" || "$BOXTYPE" == "ufs912" || "$BOXTYPE" == "ufs913" ) && "$OUTTYPE" == "flash" && "$IMAGE" == "enigma2" ]]; then
  # The root will be optionally stripped of all language support except de (German), fr (French)
  # and en (English) because the flash space is rather limited on these receivers.
  # A fourth language can be specified here in ISO code (suggestion is your own language,
  # two lower case letters). To leave all languages in, specify 'all' here:
  export OWNLANG=nl
  # and the country to go with it (ISO code, two uppercase letters, often the same letters
  # as the language; in case of OWNLANG=all it is ignored):
  export OWNCOUNTRY=NL
fi
$SCRIPTDIR/$OUTTYPE/prepare_root_"$IMAGE"_"$OUTTYPE".sh $TUFSBOXDIR/release
echo
echo " Root preparation completed."
echo

# Check .elf/.bin companion file sizes
if [ $BOXTYPE == "dp2010" -o $BOXTYPE == "fx6010" -o $BOXTYPE == "dp7000" -o $BOXTYPE == "dp7001" -o $BOXTYPE == "dp7050" -o $BOXTYPE == "ep8000" -o $BOXTYPE == "epp8000" -o $BOXTYPE == "gpv8000" ]; then
  echo "Check bin..."
#  if [ $IMAGEN == "Enigma2" ]; then
    AUDIOBINSIZE=`stat -c %s $TUFSBOXDIR/release/root/modules/companion_h205_audio.bin`
    VIDEOBINSIZEA=`stat -c %s $TUFSBOXDIR/release/root/modules/companion_h205_video_Ax.bin`
    VIDEOBINSIZEB=`stat -c %s $TUFSBOXDIR/release/root/modules/companion_h205_video_Bx.bin`
#  elif [ $IMAGEN == "Neutrino" ]; then
#    AUDIOBINSIZE=`stat -c %s $TUFSBOXDIR/release/lib/firmware/companion_h205_audio.bin`
#    VIDEOBINSIZEA=`stat -c %s $TUFSBOXDIR/release/lib/firmware/companion_h205_video_Ax.bin`
#    VIDEOBINSIZEB=`stat -c %s $TUFSBOXDIR/release/lib/firmware/companion_h205_video_Ax.bin`
#  fi
  if [ "$AUDIOBINSIZE" == "" ] || [ "$VIDEOBINSIZEA" == "" ] || [ "$VIDEOBINSIZEB" == "" ] || [ "$AUDIOBINSIZE" == "0" ] || [ "$VIDEOBINSIZEA" == "0" ] || [ "$VIDEOBINSIZEB" == "0" ]; then
  echo -e "\033[01;31m"
  echo "-- ERROR! -------------------------------------------------------------"
  echo
    if [ "$AUDIOBINSIZE" == "" ]; then
      echo " !!! ERROR: File companion_h205_audio.bin is missing !!!"
    fi
    if [ "$AUDIOBINSIZE" == "0" ]; then
      echo " !!! ERROR: File size of companion_h205_audio.bin is zero !!!"
    fi
    if [ "$VIDEOBINSIZEA" == "" ]; then
      echo " !!! ERROR: File companion_h205_video_Ax.bin is missing !!!"
    fi
    if [ "$VIDEOBINSIZEA" == "0" ]; then
      echo " !!! ERROR: File size of companion_h205_video_Ax.bin is zero !!!"
    fi
    if [ "$VIDEOBINSIZEB" == "" ]; then
      echo " !!! ERROR: File companion_h205_video_Bx.bin is missing !!!"
    fi
    if [ "$VIDEOBINSIZEB" == "0" ]; then
      echo " !!! ERROR: File size of companion_h205_video_Bx.bin is zero !!!"
    fi
    echo
    echo " Make sure that you use the correct .bin files in the"
    echo " directory $CDKDIR/root/boot."
    echo
    echo " Exiting..."
    echo "-----------------------------------------------------------------------"
    echo -e "\033[00m"
    export ERROR="yes"
    exit 2
  fi
else
  if [ $IMAGEN == "Enigma2" ] || [ $IMAGEN == "Titan" ]; then
    AUDIOELFSIZE=`stat -c %s $TUFSBOXDIR/release/boot/audio.elf`
    VIDEOELFSIZE=`stat -c %s $TUFSBOXDIR/release/boot/video.elf`
  elif [ $IMAGEN == "Neutrino" ]; then
    AUDIOELFSIZE=`stat -c %s $TUFSBOXDIR/release/lib/firmware/audio.elf`
    VIDEOELFSIZE=`stat -c %s $TUFSBOXDIR/release/lib/firmware/video.elf`
  fi
  if [ "$AUDIOELFSIZE" == "" ] || [ "$VIDEOELFSIZE" == "" ] || [ "$AUDIOELFSIZE" == "0" ] || [ "$VIDEOELFSIZE" == "0" ]; then
    echo -e "\033[01;31m"
    echo "-- ERROR! -------------------------------------------------------------"
    echo
      if [ "$AUDIOELFSIZE" == "" ]; then
        echo " !!! ERROR: File audio.elf is missing !!!"
      fi
      if [ "$AUDIOELFSIZE" == "0" ]; then
        echo " !!! ERROR: File size of audio.elf is zero !!!"
      fi
    if [ "$VIDEOELFSIZE" == "" ]; then
      echo " !!! ERROR: File video.elf is missing !!!"
    fi
    if [ "$VIDEOELFSIZE" == "0" ]; then
      echo " !!! ERROR: File size of video.elf is zero !!!"
    fi
    echo
    echo " Make sure that you use the correct .elf files in the"
    echo " directory $CDKDIR/root/boot."
    echo
    echo " Exiting..."
    echo "-----------------------------------------------------------------------"
    echo -e "\033[00m"
    export ERROR="yes"
    exit 2
  fi
fi

# Check if the devs have been made
if [ ! -e $TMPROOTDIR/dev/mtd0 ]; then
  echo -e "\033[01;31m"
  echo "-- ERROR! -------------------------------------------------------------"
  echo
  echo " !!! ERROR: DEVS ARE MISSING !!!"
  echo " APPARENTLY MAKEDEV IN prepare_root.sh FAILED."
  echo
  echo " Exiting..."
  echo "-----------------------------------------------------------------------"
  echo -e "\033[00m"
  exit 2
fi

# Build output files, depending on receiver type, image type and output type
echo "-- Create output file(s) ----------------------------------------------"
echo
echo " Build $IMAGEN output file(s) for $BOXTYPE running in/on $OUTTYPE."
echo

if [ "$OUTTYPE" == "flash" ]; then
# Handle Fortis resellerID
case $BOXTYPE in
  atemio520|atemio530|hs8200|fs9000|hs9510|hs7110|hs7420|hs7810a|hs7119|hs7429|hs7819|dp2010|dp7000|dp7001|ep8000|epp8000|fx6010|gpv8000)
    RESELLERID=$1
    if [[ "$RESELLERID" == "" ]]; then
      case $BOXTYPE in
        atemio520)
          RESELLERID=252902A5
          FORTISBOX="Atemio AM 520 HD";;
        atemio530)
          RESELLERID=252902AA
          FORTISBOX="Atemio AM 530 HD";;
        hs8200)
          RESELLERID=230200A0
          FORTISBOX="Octagon SF1028P HD Noblence";;
        fs9000)
          RESELLERID=20020000
          FORTISBOX="Octagon SF1018P HD Alliance";;
        hs9510)
          RESELLERID=20020300
          FORTISBOX="Octagon SF1008P HD Intelligence";;
        hs7110)
          RESELLERID=250202A0
          FORTISBOX="Octagon SF918 SE+ HD Difference";;
        hs7420)
          RESELLERID=250203A0
          FORTISBOX="Octagon SF1008P SE+ HD Intelligence";;
        hs7810a)
          RESELLERID=250200A0
          FORTISBOX="Octagon SF1008 SE+ HD Intelligence";;
        hs7119)
          RESELLERID=270100A0
          FORTISBOX="Octagon SF918G SE+ HD Difference";;
        hs7429)
          RESELLERID=270130A0
          FORTISBOX="Octagon SF1008G+ SE+ HD Intelligence";;
        hs7819)
          RESELLERID=270120A0
          FORTISBOX="Octagon SF1008G SE+ HD Intelligence";;
        dp2010)
          RESELLERID=29090300
          FORTISBOX="Forever HD 3434 PVR Cardiff";;
        dp7000)
          RESELLERID=29090200
          FORTISBOX="Forever HD 9898 PVR Cardiff";;
        dp7001)
          RESELLERID=29060100
          FORTISBOX="Rebox RE-4220HD S-PVR";;
        dp7050)
          RESELLERID=29091000
          FORTISBOX="Forever HD 2424 PVR Cardiff";;
        ep8000)
          RESELLERID=2A010100
          FORTISBOX="XCruiser XDSR420HD Avant";;
        epp8000)
          RESELLERID=2A020000
          FORTISBOX="Rebox RE-8220HD S-PVR";;
        fx6010)
          RESELLERID=29060000
          FORTISBOX="Rebox RE-2220HD S-PVR";;
        gpv8000)
          RESELLERID=29032000
          FORTISBOX="Openbox SX9 HD Combo";;
      esac
      echo " No resellerID specified, using default $RESELLERID"
      echo " (equals $FORTISBOX)."
      echo
      echo " Note: other resellerID may be specified as arg1"
      echo " on the command line:"
      echo " $0 [resellerID]"
      echo
      echo " Optional resellerID must either be 4 or 8 hex characters".
    else
      echo " Using resellerID $RESELLERID."
    fi
    echo
    export RESELLERID
    if [ ! -e $TOOLSDIR/dummy.squash.signed.padded ]; then
      cd $TOOLSDIR
      ./fup > /dev/null
      cd $CURDIR
    fi
  esac

  case $BOXTYPE in
    adb_box)
      $SCRIPTDIR/$OUTTYPE/"$BOXTYPE"_"$OUTTYPE".sh
      if [ "$IMAGE" == "enigma2" ]; then
        unset OWNLANG
        unset OWNCOUNTRY
      fi;;
    atemio520)
      $SCRIPTDIR/$OUTTYPE/$IMAGE/"$BOXTYPE"_"$IMAGE"_"$OUTTYPE".sh
      unset RESELLERID;;
    hs8200)
      $SCRIPTDIR/$OUTTYPE/"$BOXTYPE"_"$OUTTYPE".sh
      unset RESELLERID
      if [ "$IMAGE" == "enigma2" ]; then
        unset OWNLANG
        unset OWNCOUNTRY
      fi;;
    cuberevo|cuberevo_mini|cuberevo_mini2|cuberevo_250hd|cuberevo_mini_fta|cuberevo_2000hd|cuberevo_3000hd|cuberevo_9500hd)
      $SCRIPTDIR/$OUTTYPE/$IMAGE/cuberevo_"$IMAGE"_"$OUTTYPE".sh;;
    fs9000|hs9510)
      $SCRIPTDIR/$OUTTYPE/$IMAGE/fortis_1G_"$IMAGE"_"$OUTTYPE".sh
      unset RESELLERID;;
    hs7420|hs7110|hs7810a)
      $SCRIPTDIR/$OUTTYPE/$IMAGE/fortis_2G_"$IMAGE"_"$OUTTYPE".sh
      unset RESELLERID;;
    hs7429|hs7119|hs7819)
      $SCRIPTDIR/$OUTTYPE/fortis_3G_"$OUTTYPE".sh
      unset RESELLERID;;
    dp2010|fx6010|dp7000|dp7001|dp7050|ep8000|epp8000|gpv8000)
      $SCRIPTDIR/$OUTTYPE/fortis_4G_"$OUTTYPE".sh
      unset RESELLERID;;
    spark|spark7162)
      $SCRIPTDIR/$OUTTYPE/spark_"$OUTTYPE".sh;;
    tf7700)
      $SCRIPTDIR/$OUTTYPE/"$BOXTYPE"_"$OUTTYPE".sh;;
    ufc960)
      $SCRIPTDIR/$OUTTYPE/$IMAGE/ufc960_"$OUTTYPE"_"$IMAGE".sh;;
    ufs910)
      $SCRIPTDIR/$OUTTYPE/ufs910_"$OUTTYPE".sh;;
    ufs912|ufs913)
      $SCRIPTDIR/$OUTTYPE/"$BOXTYPE"_"$OUTTYPE".sh
      if [ "$IMAGE" == "enigma2" ]; then
        unset OWNLANG
        unset OWNCOUNTRY
      fi;;
    ufs922)
#      $SCRIPTDIR/$OUTTYPE/$IMAGE/"nor"_"$IMAGE"_"$OUTTYPE".sh;;
      $SCRIPTDIR/$OUTTYPE/ufs922_"$OUTTYPE".sh;;
    vitamin_hd5000)
      $SCRIPTDIR/$OUTTYPE/vitamin_hd5000_"$OUTTYPE".sh;;
    *)
      echo " Sorry, there is no $OUTTYPE support for receiver $BOXTYPE available."
      echo
      echo " Exiting..."
      echo "-----------------------------------------------------------------------"
      exit 2;;
  esac
else #USB
  case $BOXTYPE in
    adb_box)
#      $SCRIPTDIR/$OUTTYPE/"$BOXTYPE"_"$OUTTYPE".sh;;
      $SCRIPTDIR/$OUTTYPE/make_tar_gz.sh;;
	atemio520)
      $SCRIPTDIR/$OUTTYPE/make_tar_gz.sh;;
    hs8200|fs9000|hs9510)
      $SCRIPTDIR/$OUTTYPE/make_tar_gz.sh;;
    hs7110|hs7420|hs7810a|hs7119|hs7429|hs7819|dp2010|dp7000|dp7001|ep8000|epp8000|fx6010|gpv8000)
      $SCRIPTDIR/$OUTTYPE/"fortis_234G"_"$OUTTYPE".sh;;
    cuberevo|cuberevo_mini|cuberevo_mini2|cuberevo_250hd|cuberevo_mini_fta|cuberevo_2000hd|cuberevo_3000hd|cuberevo_9500hd)
      $SCRIPTDIR/$OUTTYPE/make_tar_gz.sh;;
    ufs910|ufs912|ufs913|ufs922|ufc960)
      $SCRIPTDIR/$OUTTYPE/make_tar_gz.sh;;
    hl101|vip1_v1|vip1_v2|vip2)
      $SCRIPTDIR/$OUTTYPE/make_tar_gz.sh;;
    vitamin_hd5000)
      $SCRIPTDIR/$OUTTYPE/"$BOXTYPE"_"$OUTTYPE".sh;;
    opt9600|opt9600mini|opt9600prima)
      $SCRIPTDIR/$OUTTYPE/opt9600_"$OUTTYPE".sh;;
    hchs8100|hchs9000)
      $SCRIPTDIR/$OUTTYPE/hchs8100_"$OUTTYPE".sh;;
    *)
      echo " Sorry, there is no $OUTTYPE support for receiver $BOXTYPE available."
      echo
      echo " Exiting..."
      echo "-----------------------------------------------------------------------"
      exit 2;;
  esac
fi
echo

# Wrap up
cd $CURDIR
echo "-- Result -------------------------------------------------------------"
echo
echo " Output file(s) created in $OUTDIR:"
echo
ls -ohg $OUTDIR > ./dirlist
cat ./dirlist
rm ./dirlist
echo
echo "-- Finished -----------------------------------------------------------"

# Clean up variables
unset CURDIR
unset BASEDIR
unset TUFSBOXDIR
unset TFINSTALLERDIR
unset CDKDIR
unset SCRIPTDIR
unset TOOLSDIR
unset TMPDIR
unset TMPROOTDIR
unset TMPKERNELDIR
unset TMPVARDIR
unset OUTDIR
unset PATCH
unset IMAGE
unset IMAGEN
unset OUTTYPE
unset HOST
unset GITVERSION
unset BATCH_MODE
unset INAME
unset MEDIAFW

if [ -e dummy.squash.signed.padded ]; then
  rm -f dummy.squash.signed.padded
fi
# vim:ts=4
