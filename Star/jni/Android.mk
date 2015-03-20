LOCAL_PATH := $(NDK_PROJECT_PATH)/../CoreEngine/src

include $(CLEAR_VARS)
LOCAL_MODULE := StarAndroid
LOCAL_CFLAGS := -DANDROID_VERSION
LOCAL_CFLAGS += -Werror

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include

LOCAL_CFLAGS += -DDEBUG
LOCAL_LDLIBS := -lz -llog -ldl
LOCAL_CPP_PEATURESSTLPORT_FORCE_REBUILD := true


LOCAL_SRC_FILES := StarSystem.cpp \
    StarNet.cpp \
    StarMath.cpp \
    StarMEM.cpp \
    StarClient.cpp \

include $(BUILD_SHARED_LIBRARY)