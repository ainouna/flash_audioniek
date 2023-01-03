#!/bin/bash
# -----------------------------------------------------------------------
#  This script creates the files needed to run Enigma2/Neutrino off an
#  USB stick for these Homecast receivers"
#  - HS8100 series
#  - HS9000 series
#
# Author: Schischu/Audioniek"
# Date: 27-08-2022"
#
# -----------------------------------------------------------------------
# Changes:
#
# -----------------------------------------------------------------------

# -----------------------------------------------------------------------
# It is assumed that an image was already built prior to executing this
#  script!"
# -----------------------------------------------------------------------
#

# Set up the variables
TMPDUMDIR=$TMPDIR/DUMMY
MKFSEXT3="mke2fs -t ext3"
MKCRC32=crc32

#OUTFILE="$BOXTYPE"_"$INAME""$IMAGE"_kernel_"$OUTTYPE"_"P$PATCH"_"$GITVERSION".upd
#OUTFILETS="$BOXTYPE"_TS_"$INAME""$IMAGE"_kernel_"$OUTTYPE"_"P$PATCH"_"$GITVERSION".upd
OUTZIPFILE="$HOST"_"$INAME""$IMAGE"_"$MEDIAFW"_"$OUTTYPE"_"P$PATCH"_"$GITVERSION".zip
FLASH_FILE="FALSE"

if [ -e $OUTDIR ]; then
  rm -f $OUTDIR/*
elif [ ! -d $OUTDIR ]; then
  mkdir -p $OUTDIR
fi

if [ -e $TMPDUMDIR ]; then
  rm -rf $TMPDUMDIR/*
elif [ ! -d $TMPDUMDIR ]; then
  mkdir -p $TMPDUMDIR
fi

echo -n " - Prepare kernel file..."
#cp $TMPKERNELDIR/uImage $OUTDIR/uImage
cp $TMPKERNELDIR/uImage $TMPROOTDIR/boot/uImage
echo " done."

#echo -n " - Checking kernel size..."
#SIZEK=`stat $TMPKERNELDIR/uImage -t --format %s`
#SIZEKD=`printf "%d" $SIZEK`
#SIZEKH=`printf "%08X" $SIZEK`
#KERNELSIZE=`printf "%x" $SIZEK`
#if [[ $SIZEKD > "4194304" ]]; then
#  echo
#  echo -e "\033[01;31m"
#  echo "-- ERROR! -------------------------------------------------------------"
#  echo
#  echo " KERNEL TOO BIG: 0x$SIZEKH instead of max. 0x00400000 bytes." > /dev/stderr
#  echo " Exiting..."
#  echo
#  echo "-----------------------------------------------------------------------"
#  echo -e "\033[00m"
#  exit
#else
#  echo " OK: $SIZEKD (0x$SIZEKH, max. 0x00400000) bytes."
#fi
#echo " done."

cp $TMPKERNELDIR/uImage $OUTDIR/uImage

# remove old MAKEDEV
if [ -e $TMPROOTDIR/dev/MAKEDEV ]; then
  rm -f $TMPROOTDIR/dev/MAKEDEV
fi
if [ -e $TMPROOTDIR/sbin/MAKEDEV ]; then
  rm -f $TMPROOTDIR/sbin/MAKEDEV
fi

echo -n " - Preparing root image..."
dd if=/dev/zero of=$OUTDIR/root.img bs=1M count=256 2> /dev/null
# Create a ext3 partition for the complete root
cd $TMPROOTDIR
$MKFSEXT3 -q -F -L $IMAGE $OUTDIR/root.img
# mount the image file
sudo mount -o loop $OUTDIR/root.img $TMPDUMDIR
# copy the image to it
sudo cp -rf . $TMPDUMDIR
sudo rm -rf lost+found
if [ -d $TMPDUMDIR/lost+found ];then
  sudo rmdir --ignore-fail-on-non-empty $TMPDUMDIR/lost+found
fi
sudo umount $TMPDUMDIR
cd $CURDIR
echo " done."

echo -n " - Add bootmenu.lst file ..."
cp $TOOLSDIR/opt9600_bootmenu.lst $OUTDIR/bootmenu.lst
echo " done."

echo -n " - Creating MD5 checksums..."
if [ "$FLASH_FILE"  == "TRUE" ]; then
  md5sum -b $OUTDIR/$OUTFILE | awk -F' ' '{print $1}' > $OUTDIR/$OUTFILE.md5
  md5sum -b $OUTDIR/root.img | awk -F' ' '{print $1}' > $OUTDIR/root.img.md5
  if [ "$BOXTYPE" == "opt9600" ]; then
    md5sum -b $OUTDIR/$OUTFILETS | awk -F' ' '{print $1}' > $OUTDIR/$OUTFILETS.md5
  fi
else
  md5sum -b $OUTDIR/uImage | awk -F' ' '{print $1}' > $OUTDIR/uImage.md5
  md5sum -b $OUTDIR/root.img | awk -F' ' '{print $1}' > $OUTDIR/root.img.md5
fi
echo " done."

echo -n " - Creating .ZIP output file..."
cd $OUTDIR
if [ "$FLASH_FILE"  == "TRUE" ]; then
  zip -j $OUTZIPFILE $OUTDIR/$OUTFILE root.img $OUTDIR/$OUTFILE.md5 root.img.md5 > /dev/null
  if [ "$BOXTYPE" == "opt9600" ]; then
    zip -j $OUTZIPFILE $OUTDIR/$OUTFILETS $OUTDIR/$OUTFILETS.md5 > /dev/null
  fi
else
  zip -j $OUTZIPFILE $OUTDIR/uImage $OUTDIR/root.img $OUTDIR/uImage.md5 $OUTDIR/root.img.md5 > /dev/null
fi
cd $CURDIR
echo " done."

if [ -e $OUTDIR/$OUTZIPFILE ]; then
  echo -e "\033[01;32m"
  echo "-- Instructions -------------------------------------------------------"
  echo
  echo " The receiver must be equipped with a bootloader that is"
  echo " is capable of running the image off an USB stick."
  echo
  echo " To run the image off an USB stick, copy the files bootmenu.lst,"
  echo " uImage and root.img to the root directory of a FAT32"
  echo " formatted USB stick."
  echo
  echo " Insert the USB stick in any of the box's USB A type ports, and"
  echo " switch on the receiver. It will then run the image off the USB stick."
  echo -e "\033[00m"
fi

# Clean up
rm -f $TMPKERNELDIR/uImage
rm -f $TMPROOTDIR/uImage
