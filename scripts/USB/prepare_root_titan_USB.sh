#!/bin/bash
# ----------------------------------------------------------------------
# This script prepares the root of a titan image intended to run from
# or on a USB stick pending further processing.
#
# Author: Audioniek, based on previous work by schishu and bpanther"
# Date: 11-04-2022"
#
# ---------------------------------------------------------------------------
# Changes:
# 20220619 Audioniek   Initial version
# 20220829 Audioniek   Added Homecast HS8100 series.
# 20220917 Audioniek   Fix make devices (wrong boxmodel)
# 20221029 Audioniek   Added Homecast HS9000 series.

RELEASEDIR=$1

common() {
  echo -n " Copying release image..."
  find $RELEASEDIR -mindepth 1 -maxdepth 1 -exec cp -at$TMPROOTDIR -- {} +
  echo " done."

  echo -n " Creating devices..."
  export TARGET=$BOXTYPE
  cd $TMPROOTDIR/dev/
  if [ -e $TMPROOTDIR/var/etc/init.d/makedev ]; then
    $TMPROOTDIR/var/etc/init.d/makedev start > /dev/null 2> /dev/null
  else
    $TMPROOTDIR/etc/init.d/makedev start > /dev/null 2> /dev/null
  fi
  cd - > /dev/null
  echo " done."

  echo -n " Move kernel..."
  mv $TMPROOTDIR/boot/uImage $TMPKERNELDIR/uImage
  echo " done."
}

# Prepare titan root according to box type
case $BOXTYPE in
  adb_box)
    common
    ;;
  hs8200)
    common
    ;;
  fs9000|hs9510)
    common
    ;;
  hs7110|hs7119|hs7420|hs7429|hs7810a|hs7819)
    common
    ;;
  dp2010|dp6010|dp7000|dp7001|dp7050|ep8000|epp8000|fx6010|gpv8000)
    common
    ;;
  cuberevo|cuberevo_mini|cuberevo_mini2|cuberevo_250hd)
    common
    ;;
  hl101)
    common
    ;;
  opt9600|opt9600mini|opt9600prima)
    common
    ;;
  spark|spark7162)
    common
    ;;
  hchs8100|hchs9000)
    common
    ;;
  ufc960|ufs910|ufs922)
    find $RELEASEDIR -mindepth 1 -maxdepth 1 -exec cp -at$TMPROOTDIR -- {} +

    if [ ! -e $TMPROOTDIR/dev/mtd0 ]; then
      echo -n " Creating devices..."
      cd $TMPROOTDIR/dev/
      if [ -e $TMPROOTDIR/var/etc/init.d/makedev ]; then
        $TMPROOTDIR/var/etc/init.d/makedev start >/dev/null 2>/dev/null
      else
        $TMPROOTDIR/etc/init.d/makedev start >/dev/null 2>/dev/null
      fi
      cd - > /dev/null
      echo " done."
    fi

    echo -n " Add init_mini_fo..."
    mkdir $TMPROOTDIR/root_rw
    mkdir $TMPROOTDIR/storage
    cp ../common/init_mini_fo $TMPROOTDIR/sbin/
    chmod 777 $TMPROOTDIR/sbin/init_mini_fo
    echo " done."

    echo -n " Adapt /var/etc/fstab..."
    sed -i 's|/dev/sda.*||g' $TMPROOTDIR/var/etc/fstab
    #echo "/dev/mtdblock4	/var	jffs2	defaults	0	0" >> $TMPROOTDIR/var/etc/fstab
    echo " done."

    echo -n " Move kernel..."
    mv $TMPROOTDIR/boot/uImage $TMPKERNELDIR/uImage
    echo " done."

    # --- STORAGE FOR MINI_FO ---
    mkdir $TMPSTORAGEDIR/root_ro
    ;;
  ufs912|ufs913)
    common

    echo -n " Fill firmware directory..."
    mv $TMPROOTDIR/lib/firmware/audio.elf $TMPFWDIR/audio.elf
    mv $TMPROOTDIR/lib/firmware/video.elf $TMPFWDIR/video.elf

    rm -f $TMPROOTDIR/boot/*

    if [ "$BOXTYPE" == "ufs912" ];then
       echo "/dev/mtdblock3	/boot	jffs2	defaults	0	0" >> $TMPROOTDIR/var/etc/fstab
       #echo "/dev/mtdblock5	/root	jffs2	defaults	0	0" >> $TMPROOTDIR/etc/fstab
    else
       echo "/dev/mtdblock8	/boot	jffs2	defaults	0	0" >> $TMPROOTDIR/var/etc/fstab
       #echo "/dev/mtdblock10	/root	jffs2	defaults	0	0" >> $TMPROOTDIR/etc/fstab
    fi
    echo " done."
    ;;
  vip1_v1|vip1_v2|vip2)
    common
    ;;
  vitamin_hd5000)
    common
    ;;
 tf7700)
    echo -n " Copying release image..."
    find $RELEASEDIR -mindepth 1 -maxdepth 1 -exec cp -at$TMPROOTDIR -- {} +
    echo " done."

    if [ ! -e $TMPROOTDIR/dev/mtd0 ]; then
      echo -n " Creating devices..."
      cd $TMPROOTDIR/dev/
      if [ -e $TMPROOTDIR/var/etc/init.d/makedev ]; then
        $TMPROOTDIR/var/etc/init.d/makedev start > /dev/null 2> /dev/null
      else
        $TMPROOTDIR/etc/init.d/makedev start > /dev/null 2> /dev/null
      fi
      cd - > /dev/null
      echo " done."
    fi;;
  *)
    echo "Receiver $BOXTYPE not supported."
    echo
    ;;
esac
