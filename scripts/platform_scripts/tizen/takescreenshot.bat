set png_image=%1
set xwd_image="%1".xwd
sdb root on
sdb shell xwd -root -out /tmp/screen.xwd
sdb pull /tmp/screen.xwd %xwd_image%
sdb root off
convert %xwd_image% %png_image%
