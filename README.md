# Decode JPEG to YUV & display on HEO by EGT

This software is to show a demo that thousands of JPEG
files are decoded to YUV format in a very short time
and show the YUV data on the Microchip LCDC hardware 
overlay HEO layer directly by EGT on Linux system.

The each JPEG image size is 800x360, tested on SAM9X6 
Curiosity board(SAM9X6-D1G), the performance is about 
30fps with such size.

# How to build source
```
	clone this source repo
	cd build
	modify makefile, update sdk path to yours
	make
```

# Upload the demo files
```
	copy build/jpeg2yuv to target
	copy build/start.sh to target
	copy images folder to target
```

# Run the demo
```
	./start.sh
```

***
## Test
- Tested on SAM9x60-Curiosity with TM5000 display.
- Tested on SAM9x75-Curiosity Rev 1 with LVDS-DISPLAY-WVGA Revision 3.
  The same code and images are tested on two boards.
