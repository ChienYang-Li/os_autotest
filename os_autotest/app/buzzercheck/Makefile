CXXFLAGS += -I$(DIR_INSTALL)/opencv/include/opencv4
CXXFLAGS += -I$(DIR_INSTALL)/alsa-lib/usr/include
CXXFLAGS += -I$(DIR_INSTALL)/fftw/include

LDFLAGS += -L$(DIR_INSTALL)/opencv/lib
LDFLAGS += -L$(DIR_INSTALL)/opencv/lib/opencv4/3rdparty
LDFLAGS += -L$(DIR_INSTALL)/zlib/lib
LDFLAGS += -L$(DIR_INSTALL)/alsa-lib/usr/lib
LDFLAGS += -L$(DIR_INSTALL)/fftw/lib

LIBS += -lopencv_calib3d
LIBS += -lopencv_core
LIBS += -lopencv_dnn
LIBS += -lopencv_features2d
LIBS += -lopencv_flann
LIBS += -lopencv_gapi
LIBS += -lopencv_highgui
LIBS += -lopencv_imgcodecs
LIBS += -lopencv_imgproc
LIBS += -lopencv_ml
LIBS += -lopencv_objdetect
LIBS += -lopencv_photo
LIBS += -lopencv_stitching
LIBS += -lopencv_video
LIBS += -lopencv_videoio

LIBS += -l:libz.a
LIBS += -lasound
LIBS += -lfftw3

include ../env.mk
