SDK            := /home/wayne/sdk/arm9-202304/arm-buildroot-linux-gnueabi_sdk-buildroot
CXX            := $(SDK)/bin/arm-buildroot-linux-gnueabi-g++ -std=c++17
SRC_DIR        := ../src
EGT_CXXFLAGS   := $(shell $(SDK)/bin/pkg-config libegt --cflags)
EGT_LDFLAGS    := $(shell $(SDK)/bin/pkg-config libegt --libs)
#JPEG_CXXFLAGS   := $(shell $(SDK)/bin/pkg-config libjpeg --cflags)
#JPEG_LDFLAGS    := $(shell $(SDK)/bin/pkg-config libjpeg --libs)
LIBS           := -lplanes -ljpeg

SRC            := $(SRC_DIR)/*
TARGET         := jpeg2yuv

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(EGT_CXXFLAGS) $^ -o $@ $(EGT_LDFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)
