# Straight forward Makefile to compile all examples in a row

# If cross-compiling, you may wish to set the following environment variable to the root location of 
# your 'sdk'
# SDKSTAGE=/home/foo/raspberrypi

ifeq ($(shell uname -m),armv6l)

INCDIR=-I./Common -I$(SDKSTAGE)/opt/vc/include -I$(SDKSTAGE)/opt/vc/include/interface/vcos/pthreads -I$(SDKSTAGE)/opt/vc/include/interface/vmcs_host/linux

LIBS=-lbrcmEGL -lbrcmGLESv2 -lm -lbcm_host -L$(SDKSTAGE)/opt/vc/lib

CFLAGS+=-DRPI_NO_X -Os -mcpu=arm1176jzf-s -mfloat-abi=hard -mfpu=vfp -mtune=arm1176jzf-s 

CFLAGSDBG=-DRPI_NO_X -w

else ifeq ($(shell uname -m),armv7l)
	ifeq ($(shell lscpu | grep Model: | cut -c24),4)
		INCDIR=-I./Common -I$(SDKSTAGE)/opt/vc/include -I$(SDKSTAGE)/opt/vc/include/interface/vcos/pthreads -I$(SDKSTAGE)/opt/vc/include/interface/vmcs_host/linux
		
		LIBS=-lbrcmEGL -lbrcmGLESv2 -lm -lbcm_host -L$(SDKSTAGE)/opt/vc/lib

		CFLAGS+=-DRPI_NO_X -Os -mcpu=cortex-a53 -mfloat-abi=hard -mfpu=neon-fp-armv8 -mneon-for-64bits -mtune=cortex-a53 

		CFLAGSDBG=-DRPI_NO_X -w

	else
		INCDIR=-I./Common -I$(SDKSTAGE)/opt/vc/include -I$(SDKSTAGE)/opt/vc/include/interface/vcos/pthreads -I$(SDKSTAGE)/opt/vc/include/interface/vmcs_host/linux

		LIBS=-lbrcmEGL -lbrcmGLESv2 -lm -lbcm_host -L$(SDKSTAGE)/opt/vc/lib

		CFLAGS+=-DRPI_NO_X -Os -mcpu=cortex-a7 -mfloat-abi=hard -mfpu=neon-vfpv4 -mtune=cortex-a7 

		CFLAGSDBG=-DRPI_NO_X -w
	endif

else

INCDIR=-I./Common

LIBS=-lGLESv2 -lEGL -lm -lX11

CFLAGS+=-O2 -march=native -mtune=native 

CFLAGSDBG= 

endif

COMMONSRC=./Common/esShader.c    \
          ./Common/esTransform.c \
          ./Common/esShapes.c    \
          ./Common/esUtil.c
COMMONHRD=esUtil.h

CH02SRC=./Chapter_2/Hello_Triangle/Hello_Triangle.c
CH02DST=./bin/CH02_HelloTriangle
CH02DSTDBG=./bin/CH02_HelloTriangle_dbg

DM01SRC=./demos/01_many_textures.c
DM01DST=./bin/DM01_ManyTextures
DM01DSTDBG=./bin/DM01_ManyTextures_dbg

default: debug

all: release

release: ./Chapter_2/Hello_Triangle/CH02_HelloTriangle \
		 ./demos/01_many_textures

debug: ./Chapter_2/Hello_Triangle/CH02_HelloTriangle-dbg \
		./demos/01_many_textures-dbg

clean:
	find . -name "CH??_*" | xargs rm -f 
	find . -name "DM??_*" | xargs rm -f 

./Chapter_2/Hello_Triangle/CH02_HelloTriangle: ${COMMONSRC} ${COMMONHDR} ${CH02SRC}
	gcc $(CFLAGS) ${COMMONSRC} ${CH02SRC} -o ${CH02DST} ${INCDIR} ${LIBS}

./Chapter_2/Hello_Triangle/CH02_HelloTriangle-dbg: ${COMMONSRC} ${COMMONHDR} ${CH02SRC}
	tcc ${COMMONSRC} ${CH02SRC} -o ${CH02DSTDBG} ${INCDIR} ${LIBS} ${CFLAGSDBG} 

./demos/01_many_textures: ${COMMONSRC} ${COMMONHDR} ${DM01SRC}
	gcc $(CFLAGS) ${COMMONSRC} ${DM01SRC} -o ${DM01DST} ${INCDIR} ${LIBS}

./demos/01_many_textures-dbg: ${COMMONSRC} ${COMMONHDR} ${DM01SRC}
	tcc ${COMMONSRC} ${DM01SRC} -o ${DM01DSTDBG} ${INCDIR} ${LIBS} ${CFLAGSDBG} 

