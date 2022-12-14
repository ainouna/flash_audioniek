#!/bin/bash
# ---------------------------------------------------------------------------
# This script prepares the root of a neutrino image pending further
# processing.
#
# Author: Audioniek, based on previous work by schishu and bpanther"
#
# Date: 07-12-2014"
# ---------------------------------------------------------------------------
# Changes:
# 20141015: Audioniek   Fortis 4th generation receivers added.
# 20190208: Audioniek   Cuberevo Mini added.
# 20200609: Audioniek   dp6010 -> fx6010.
# 20210523: Audioniek   Remove special handling for CubeRevos.
# 20210701: Audioniek   ufs922 only does common part.
# 20210714: Audioniek   ufs910 only does common part.
#
# ---------------------------------------------------------------------------

RELEASEDIR=$1

common() {
  echo -n " Copying release image..."
  find $RELEASEDIR -mindepth 1 -maxdepth 1 -exec cp -at$TMPROOTDIR -- {} +
  if [ -e $RELEASEDIR/etc/.rccode ]; then
    cp $RELEASEDIR/etc/.rccode $TMPROOTDIR/etc/.rccode
  fi
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
  fi

  echo -n " Moving kernel..."
  mv $TMPROOTDIR/boot/uImage $TMPKERNELDIR/uImage
  rm -fr $TMPROOTDIR/boot
  echo " done."
}

# Prepare neutrino root according to box type
case $BOXTYPE in
  atemio520)
    common
#    echo -n " Moving /usr/lib..."
#    mkdir -p $TMPVARDIR/usr/lib
#    mv -t $TMPVARDIR/usr $TMPROOTDIR/usr/lib/
##    rmdir $TMPROOTDIR/usr/lib/
#    ln -sf /var/usr/lib $TMPROOTDIR/usr/lib 
#    echo " done."
    ;;
  fs9000|hs9510|hs8200|hs7110|hs7810a|hs7119|hs7819|dp2010|dp7000|dp7001|dp7050|ep8000|epp8000|fx6010|gpv8000)
    common;;
  spark|spark7162)
    common;;
  ufs910)
    common
    echo " done."
    ;;
  ufs922)
    common
   echo " done."
    ;;
  ufc960)
    common

    echo -n " Setting up init_mini_fo..."
    mkdir $TMPROOTDIR/root_rw
    mkdir $TMPROOTDIR/storage
    cp $TOOLSDIR/init_mini_fo $TMPROOTDIR/sbin/
    chmod 777 $TMPROOTDIR/sbin/init_mini_fo
    # --- STORAGE FOR MINI_FO ---
    mkdir $TMPVARDIR/root_ro
    echo " done."

#    echo -n " Moving var directory..."
#    mv $TMPROOTDIR/var $TMPVARDIR/
#    echo " done."

    echo -n " Adapting var/etc/fstab..."
    sed -i 's|/dev/sda.*||g' $TMPROOTDIR/var/etc/fstab
    #echo "/dev/mtdblock4	/var	jffs2	defaults	0	0" >> $TMPROOTDIR/var/etc/fstab
    echo " done."
    ;;
  ufs912|ufs913)
    common
    cp $RELEASEDIR/.version $TMPROOTDIR
    rm -fr $TMPROOTDIR/boot

    echo -n " Moving firmwares..."
    mv $TMPROOTDIR/lib/firmware/* $TMPFWDIR
    echo " done."

    if [ -e $TMPROOTDIR/var/etc/fstab ]; then
      echo -n " Adapting var/etc/fstab..."
      if [ "$BOXTYPE" == "ufs912" ]; then
         echo "/dev/mtdblock3	/lib/firmware	jffs2	defaults	0	0" >> $TMPROOTDIR/var/etc/fstab
         #echo "/dev/mtdblock5	/swap	jffs2	defaults	0	0" >> $TMPROOTDIR/var/etc/fstab
      else
        echo "/dev/mtdblock8	/lib/firmware	jffs2	defaults	0	0" >> $TMPROOTDIR/var/etc/fstab
        #echo "/dev/mtdblock10	/swap	jffs2	defaults	0	0" >> $TMPROOTDIR/var/etc/fstab
      fi
    else
      if [ "$BOXTYPE" == "ufs912" ]; then
        echo -n " Adapting etc/fstab..."
        echo "/dev/mtdblock3	/lib/firmware	jffs2	defaults	0	0" >> $TMPROOTDIR/etc/fstab
        #echo "/dev/mtdblock5	/swap	jffs2	defaults	0	0" >> $TMPROOTDIR/etc/fstab
      else
        echo "/dev/mtdblock8	/lib/firmware	jffs2	defaults	0	0" >> $TMPROOTDIR/etc/fstab
        #echo "/dev/mtdblock10	/swap	jffs2	defaults	0	0" >> $TMPROOTDIR/etc/fstab
      fi
    fi
    echo " done."
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
    common;;
esac
