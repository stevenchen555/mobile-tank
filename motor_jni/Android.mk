LOCAL_PATH:= $(call my-dir)

#motor
include $(CLEAR_VARS)
LOCAL_MODULE_TAGS := samples
LOCAL_MODULE:= motor
LOCAL_SRC_FILES:= motor.cpp
LOCAL_SHARED_LIBRARIES := libutils
LOCAL_STATIC_LIBRARIES :=
LOCAL_C_INCLUDES += \
	$(JNI_H_INCLUDE)
LOCAL_CFLAGS +=
include $(BUILD_SHARED_LIBRARY)
#motor

#ultrasound
include $(CLEAR_VARS)
LOCAL_MODULE_TAGS := samples
LOCAL_MODULE:= ultrasound
LOCAL_SRC_FILES:= ultrasound.cpp
LOCAL_SHARED_LIBRARIES := libutils
LOCAL_STATIC_LIBRARIES :=
LOCAL_C_INCLUDES += \
	$(JNI_H_INCLUDE)
LOCAL_CFLAGS +=
include $(BUILD_SHARED_LIBRARY)
#ultrasound
