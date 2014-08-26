#!/bin/bash
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
UPDATE_URL="http://www.gamoeba.com/stagehand_updates"
PLATFORM=`uname`
AVAIL_VERSION=`wget -qO- $UPDATE_URL/version_$PLATFORM.txt`
CURRENT_VERSION=`cat $SCRIPT_DIR/CURRENT_VERSION`
NEWVER=`echo $AVAIL_VERSION'>'$CURRENT_VERSION | bc -l`

if (( $NEWVER == 1 ));then
	PROMPT="Update available. Download new version $AVAIL_VERSION?"
	UPDATE_FILE=$PLATFORM"_update"$AVAIL_VERSION".zip"
	zenity --question --text "$PROMPT" 
	if [ $? -eq 0 ]
	then 
		wget $UPDATE_URL/$UPDATE_FILE -O $SCRIPT_DIR/$UPDATE_FILE && unzip -d $SCRIPT_DIR -o $SCRIPT_DIR/$UPDATE_FILE && rm $SCRIPT_DIR/$UPDATE_FILE
	fi
fi
