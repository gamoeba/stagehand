#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
UPDATE_URL="http://www.gamoeba.com/stagehand_updates"
PLATFORM=`uname`
AVAIL_VERSION=`wget -qO- $UPDATE_URL/version_$PLATFORM.txt`
CURRENT_VERSION=`cat $DIR/CURRENT_VERSION`
NEWVER=`echo $AVAIL_VERSION'>'$CURRENT_VERSION | bc -l`

if (( $NEWVER == 1 ));then
	PROMPT="Update available. Download new version $AVAIL_VERSION?"
	UPDATE_FILE=$PLATFORM"_update"$AVAIL_VERSION".zip"
	zenity --question --text "$PROMPT" 
	if [ $? -eq 0 ]
	then 
		wget $UPDATE_URL/$UPDATE_FILE -O $DIR/$UPDATE_FILE && unzip -o $DIR/$UPDATE_FILE && rm $DIR/$UPDATE_FILE
	fi
fi
