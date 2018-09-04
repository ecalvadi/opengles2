# Straight forward Makefile to compile all examples in a row

# If cross-compiling, you may wish to set the following environment variable to the root location of 
# your 'sdk'
# SDKSTAGE=/home/foo/raspberrypi
# Raspberry pi 2 and 3 are de same armv7l in cpu model, but raspberry pi2 are Model 5 and pi3 are model 4
# implement lscpu | grep Model: | cut -c24

ifeq ($(shell uname -m),armv6l)

INCDIR=-I./Common -I$(SDKSTAGE)/opt/vc/include -I$(SDKSTAGE)/opt/vc/include/interface/vcos/pthreads -I$(SDKSTAGE)/opt/vc/include/interface/vmcs_host/linux

LIBS=-lbrcmEGL -lbrcmGLESv2 -lm -lbcm_host -L$(SDKSTAGE)/opt/vc/lib

CFLAGS+=-DRPI_NO_X -Os -mfpu=vfp -mfloat-abi=hard -march=armv6zk -mtune=arm1176jzf-s

else ifeq ($(shell uname -m),armv7l)
INCDIR=-I./Common -I$(SDKSTAGE)/opt/vc/include -I$(SDKSTAGE)/opt/vc/include/interface/vcos/pthreads -I$(SDKSTAGE)/opt/vc/include/interface/vmcs_host/linux

LIBS=-lbrcmEGL -lbrcmGLESv2 -lm -lbcm_host -L$(SDKSTAGE)/opt/vc/lib

CFLAGS+=-DRPI_NO_X -Os -mfpu=neon-vfpv4 -mfloat-abi=hard -march=armv7-a -mtune=cortex-a7 

else

INCDIR=-I./Common

LIBS=-lGLESv2 -lEGL -lm -lX11

CFLAGS+= 

endif

COMMONSRC=./Common/esShader.c    \
          ./Common/esTransform.c \
          ./Common/esShapes.c    \
          ./Common/esUtil.c
COMMONHRD=esUtil.h

CH02SRC=./Chapter_2/Hello_Triangle/Hello_Triangle.c
CH02DST=./bin/CH02_HelloTriangle

default: all

all: ./Chapter_2/Hello_Triangle/CH02_HelloTriangle 

clean:
	find . -name "CH??_*" | xargs rm -f

./Chapter_2/Hello_Triangle/CH02_HelloTriangle: ${COMMONSRC} ${COMMONHDR} ${CH02SRC}
	gcc $(CFLAGS) ${COMMONSRC} ${CH02SRC} -o ${CH02DST} ${INCDIR} ${LIBS}

