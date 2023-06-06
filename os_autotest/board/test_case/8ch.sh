# move to (0,0)
#echo -ne \\x00\\x00\\x08\\x80\\x00\\x00 > /dev/hidg0; usleep 100
# mouse right button down
#echo -ne \\x02\\x00\\x00\\x00\\x00\\x00 > /dev/hidg0; usleep 100
# mouse right button up
#echo -ne \\x00\\x00\\x00\\x00\\x00\\x00 > /dev/hidg0; usleep 100
# information
#echo -ne \\x00\\x10\\x00\\x12\\x00\\x00 > /dev/hidg0; usleep 100
# mouse left button down
#echo -ne \\x01\\x00\\x00\\x00\\x00\\x00 > /dev/hidg0; usleep 100
# mouse left button up
#echo -ne \\x00\\x00\\x00\\x00\\x00\\x00 > /dev/hidg0; usleep 100

#rm *.png

#./capture 1 1920 1080 1.png

#./crop 1.png 1180 385 1742 420 1.ip.png

#ip=$(./ocr 1.ip.png)c
#echo $ip

delay = 700

#move to TestStart button and click
echo -ne \\x00\\x00\\x08\\x80\\x00\\x00 > /dev/hidg0; usleep $(delay)
echo -ne \\x00\\xbc\\xa2\\x07\\x00\\x00 > /dev/hidg0; usleep $(delay)

echo -ne \\x01\\x00\\x00\\x00\\x00\\x00 > /dev/hidg0; usleep $(delay)
echo -ne \\x00\\x00\\x00\\x00\\x00\\x00 > /dev/hidg0; usleep $(delay)

usleep 3000

#move to P2P prompt and click
echo -ne \\x00\\x00\\x08\\x80\\x00\\x00 > /dev/hidg0; usleep 100
echo -ne \\x00\\xfa\\x10\\x15\\x00\\x00 > /dev/hidg0; usleep 100

echo -ne \\x01\\x00\\x00\\x00\\x00\\x00 > /dev/hidg0; usleep 100
echo -ne \\x00\\x00\\x00\\x00\\x00\\x00 > /dev/hidg0; usleep 100

usleep 1000

#click MAC
echo -ne \\x01\\x00\\x00\\x00\\x00\\x00 > /dev/hidg0; usleep 100
echo -ne \\x00\\x00\\x00\\x00\\x00\\x00 > /dev/hidg0; usleep 100

usleep 1000

#move to OK and click
echo -ne \\x00\\x00\\x08\\x80\\x00\\x00 > /dev/hidg0; usleep 100
echo -ne \\x00\\x04\\x81\\x20\\x00\\x00 > /dev/hidg0; usleep 100

echo -ne \\x01\\x00\\x00\\x00\\x00\\x00 > /dev/hidg0; usleep 100
echo -ne \\x00\\x00\\x00\\x00\\x00\\x00 > /dev/hidg0; usleep 100

usleep 1000

#move to blank prompt and click
echo -ne \\x00\\x00\\x08\\x80\\x00\\x00 > /dev/hidg0; usleep 100
echo -ne \\x00\\xfa\\x10\\x15\\x00\\x00 > /dev/hidg0; usleep 100

echo -ne \\x01\\x00\\x00\\x00\\x00\\x00 > /dev/hidg0; usleep 100
echo -ne \\x00\\x00\\x00\\x00\\x00\\x00 > /dev/hidg0; usleep 100

usleep 1000

#click Videoloss
echo -ne \\x01\\x00\\x00\\x00\\x00\\x00 > /dev/hidg0; usleep 100
echo -ne \\x00\\x00\\x00\\x00\\x00\\x00 > /dev/hidg0; usleep 100

usleep 3000

#move to RTC prompt and click
echo -ne \\x00\\x00\\x08\\x80\\x00\\x00 > /dev/hidg0; usleep 100
echo -ne \\x00\\x5e\\x81\\x15\\x00\\x00 > /dev/hidg0; usleep 100

echo -ne \\x01\\x00\\x00\\x00\\x00\\x00 > /dev/hidg0; usleep 100
echo -ne \\x00\\x00\\x00\\x00\\x00\\x00 > /dev/hidg0; usleep 100

usleep 1000

#click RS485
echo -ne \\x01\\x00\\x00\\x00\\x00\\x00 > /dev/hidg0; usleep 100
echo -ne \\x00\\x00\\x00\\x00\\x00\\x00 > /dev/hidg0; usleep 100
usleep 2000
#click USB
echo -ne \\x01\\x00\\x00\\x00\\x00\\x00 > /dev/hidg0; usleep 100
echo -ne \\x00\\x00\\x00\\x00\\x00\\x00 > /dev/hidg0; usleep 100
usleep 2000
#click SATA
echo -ne \\x01\\x00\\x00\\x00\\x00\\x00 > /dev/hidg0; usleep 100
echo -ne \\x00\\x00\\x00\\x00\\x00\\x00 > /dev/hidg0; usleep 100
usleep 3000
#click Buzzer
echo -ne \\x01\\x00\\x00\\x00\\x00\\x00 > /dev/hidg0; usleep 100
echo -ne \\x00\\x00\\x00\\x00\\x00\\x00 > /dev/hidg0; usleep 100
usleep 1000
#click LED all
echo -ne \\x01\\x00\\x00\\x00\\x00\\x00 > /dev/hidg0; usleep 100
echo -ne \\x00\\x00\\x00\\x00\\x00\\x00 > /dev/hidg0; usleep 100
usleep 1000
#click device not connected
echo -ne \\x01\\x00\\x00\\x00\\x00\\x00 > /dev/hidg0; usleep 100
echo -ne \\x00\\x00\\x00\\x00\\x00\\x00 > /dev/hidg0; usleep 100
usleep 1000
#click Wireless
echo -ne \\x01\\x00\\x00\\x00\\x00\\x00 > /dev/hidg0; usleep 100
echo -ne \\x00\\x00\\x00\\x00\\x00\\x00 > /dev/hidg0; usleep 100
usleep 1000
#click Test End
echo -ne \\x01\\x00\\x00\\x00\\x00\\x00 > /dev/hidg0; usleep 100
echo -ne \\x00\\x00\\x00\\x00\\x00\\x00 > /dev/hidg0; usleep 100
usleep 1000
echo -ne \\x00\\x00\\x08\\x80\\x00\\x00 > /dev/hidg0; usleep 100

./capture 1 1920 1080 testend.png

./crop testend.png 860 510 1230 570 RTC.png
rtc=$(./ocr RTC.png)
echo RTC : $rtc

./crop testend.png 860 580 1230 640 RS485.png
rs485=$(./ocr RS485.png)
echo RS485 : $rs485

./crop testend.png 800 630 1170 690 USB.png
usb=$(./ocr USB.png)
echo USB : $usb

./crop testend.png 800 690 1170 750 SATA.png
sata=$(./ocr SATA.png)
echo SATA : $sata

#move to LED Record and click
echo -ne \\x00\\x00\\x08\\x80\\x00\\x00 > /dev/hidg0; usleep 100
echo -ne \\x00\\x8c\\x20\\x21\\x00\\x00 > /dev/hidg0; usleep 100

echo -ne \\x01\\x00\\x00\\x00\\x00\\x00 > /dev/hidg0; usleep 100
echo -ne \\x00\\x00\\x00\\x00\\x00\\x00 > /dev/hidg0; usleep 100

led_record=$(./ledcheck_b1)
echo LED Record : $led_record

usleep 2000

#move to LED Playback and click
echo -ne \\x00\\x00\\x08\\x80\\x00\\x00 > /dev/hidg0; usleep 100
echo -ne \\x00\\x68\\x21\\x21\\x00\\x00 > /dev/hidg0; usleep 100

echo -ne \\x01\\x00\\x00\\x00\\x00\\x00 > /dev/hidg0; usleep 100
echo -ne \\x00\\x00\\x00\\x00\\x00\\x00 > /dev/hidg0; usleep 100

led_playback=$(./ledcheck_b1)
echo LED Playback : $led_playback

usleep 2000

#move to LED Remote and click
echo -ne \\x00\\x00\\x08\\x80\\x00\\x00 > /dev/hidg0; usleep 100
echo -ne \\x00\\x08\\x22\\x21\\x00\\x00 > /dev/hidg0; usleep 100

echo -ne \\x01\\x00\\x00\\x00\\x00\\x00 > /dev/hidg0; usleep 100
echo -ne \\x00\\x00\\x00\\x00\\x00\\x00 > /dev/hidg0; usleep 100

led_remote=$(./ledcheck_g3)
echo LED Remote : $led_remote

usleep 2000

#move to LED HDD and click
echo -ne \\x00\\x00\\x08\\x80\\x00\\x00 > /dev/hidg0; usleep 100
echo -ne \\x00\\xbc\\x22\\x21\\x00\\x00 > /dev/hidg0; usleep 100

echo -ne \\x01\\x00\\x00\\x00\\x00\\x00 > /dev/hidg0; usleep 100
echo -ne \\x00\\x00\\x00\\x00\\x00\\x00 > /dev/hidg0; usleep 100

led_hdd=$(./ledcheck_g3)
echo LED HDD : $led_hdd

usleep 2000



usleep 1000

#move to close button and click
echo -ne \\x00\\x00\\x08\\x80\\x00\\x00 > /dev/hidg0; usleep 100
echo -ne \\x00\\xa4\\x81\\x25\\x00\\x00 > /dev/hidg0; usleep 100

echo -ne \\x01\\x00\\x00\\x00\\x00\\x00 > /dev/hidg0; usleep 100
echo -ne \\x00\\x00\\x00\\x00\\x00\\x00 > /dev/hidg0; usleep 100

usleep 2000

./livecheck
livecheck=$(./livecheck)
echo livecheck : $livecheck

usleep 2000

#right button down & up
echo -ne \\x02\\x00\\x00\\x00\\x00\\x00 > /dev/hidg0; usleep 100
echo -ne \\x00\\x00\\x00\\x00\\x00\\x00 > /dev/hidg0; usleep 100

#right button down & up
echo -ne \\x02\\x00\\x00\\x00\\x00\\x00 > /dev/hidg0; usleep 100
echo -ne \\x00\\x00\\x00\\x00\\x00\\x00 > /dev/hidg0; usleep 100

#move to configure button and click
echo -ne \\x00\\x00\\x08\\x80\\x00\\x00 > /dev/hidg0; usleep 100
echo -ne \\x00\\x40\\x21\\x12\\x00\\x00 > /dev/hidg0; usleep 100
echo -ne \\x01\\x00\\x00\\x00\\x00\\x00 > /dev/hidg0; usleep 100
echo -ne \\x00\\x00\\x00\\x00\\x00\\x00 > /dev/hidg0; usleep 100

#move to system information button and click
echo -ne \\x00\\x00\\x08\\x80\\x00\\x00 > /dev/hidg0; usleep 100
echo -ne \\x00\\x8a\\x02\\x19\\x00\\x00 > /dev/hidg0; usleep 100
echo -ne \\x01\\x00\\x00\\x00\\x00\\x00 > /dev/hidg0; usleep 100
echo -ne \\x00\\x00\\x00\\x00\\x00\\x00 > /dev/hidg0; usleep 100

./capture 1 1920 1080 1.png

./crop 1.png 1180 385 1742 420 1.ip.png

ip=$(./ocr 1.ip.png)
echo $ip

#move to close button and click
echo -ne \\x00\\x00\\x08\\x80\\x00\\x00 > /dev/hidg0; usleep 100
echo -ne \\x00\\xa4\\x81\\x25\\x00\\x00 > /dev/hidg0; usleep 100
echo -ne \\x01\\x00\\x00\\x00\\x00\\x00 > /dev/hidg0; usleep 100
echo -ne \\x00\\x00\\x00\\x00\\x00\\x00 > /dev/hidg0; usleep 100

#right buttom down & up
echo -ne \\x02\\x00\\x00\\x00\\x00\\x00 > /dev/hidg0; usleep 100
echo -ne \\x00\\x00\\x00\\x00\\x00\\x00 > /dev/hidg0; usleep 100

#right buttom down & up
echo -ne \\x02\\x00\\x00\\x00\\x00\\x00 > /dev/hidg0; usleep 100
echo -ne \\x00\\x00\\x00\\x00\\x00\\x00 > /dev/hidg0; usleep 100

#move to replay button and click
echo -ne \\x00\\x00\\x08\\x80\\x00\\x00 > /dev/hidg0; usleep 100
echo -ne \\x00\\xfe\\x21\\x12\\x00\\x00 > /dev/hidg0; usleep 100
echo -ne \\x01\\x00\\x00\\x00\\x00\\x00 > /dev/hidg0; usleep 100
echo -ne \\x00\\x00\\x00\\x00\\x00\\x00 > /dev/hidg0; usleep 100

./login.sh

# move cursor to outside of window
echo -ne \\x00\\x00\\x08\\x80\\x00\\x00 > /dev/hidg0; usleep 100
echo -ne \\x00\\x5c\\x23\\x26\\x00\\x00 > /dev/hidg0; usleep 100

usleep 5000000

replaycheck=$(./timestampcheck)
echo replaycheck : $replaycheck



