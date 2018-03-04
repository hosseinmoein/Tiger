## Hossein Moein
## July 17 2009

LOCAL_LIB_DIR = ../lib/$(BUILD_PLATFORM)
LOCAL_BIN_DIR = ../bin/$(BUILD_PLATFORM)
LOCAL_OBJ_DIR = ../obj/$(BUILD_PLATFORM)
LOCAL_INCLUDE_DIR = ../include
PROJECT_LIB_DIR = ../../../lib/$(BUILD_PLATFORM)
PROJECT_INCLUDE_DIR = ../../../include

# -----------------------------------------------------------------------------

SRCS =

HEADERS = $(LOCAL_INCLUDE_DIR)/DMScu_VectorRange.h \
          $(LOCAL_INCLUDE_DIR)/DMScu_StepVectorRange.h \
          $(LOCAL_INCLUDE_DIR)/DMScu_MathOperators.h

LIB_NAME =
TARGET_LIB =

TARGETS += $(LOCAL_BIN_DIR)/vec_range_tester \
           $(LOCAL_BIN_DIR)/mathopt_tester

# -----------------------------------------------------------------------------

LFLAGS += -Bstatic -L$(LOCAL_LIB_DIR) -L$(PROJECT_LIB_DIR)

LIBS = $(LFLAGS) $(PLATFORM_LIBS)
INCLUDES += -I. -I$(LOCAL_INCLUDE_DIR) -I$(PROJECT_INCLUDE_DIR)
DEFINES = -D_REENTRANT -DDMS_INCLUDE_SOURCE \
          -DP_THREADS -D_POSIX_PTHREAD_SEMANTICS -DDMS_$(BUILD_DEFINE)__

# -----------------------------------------------------------------------------

# object file
#
LIB_OBJS =

# -----------------------------------------------------------------------------

# set up C++ suffixes and relationship between .cc and .o files
#
.SUFFIXES: .cc

$(LOCAL_OBJ_DIR)/%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

.cc :
	$(CXX) $(CXXFLAGS) $< -o $@ -lm $(TLIB) -lg++
#	$(CXX) $(CXXFLAGS) $< -o $@ -lm $(TLIB)

# -----------------------------------------------------------------------------

all: $(TARGETS)


$(TARGET_LIB): $(LIB_OBJS)
	ar -clrs $(TARGET_LIB) $(LIB_OBJS)

VEC_RANGE_TESTER_OBJ = $(LOCAL_OBJ_DIR)/vec_range_tester.o
$(LOCAL_BIN_DIR)/vec_range_tester: $(VEC_RANGE_TESTER_OBJ)
	$(CXX) -o $@ $(VEC_RANGE_TESTER_OBJ) $(LIBS)

MATHOPT_TESTER_OBJ = $(LOCAL_OBJ_DIR)/mathopt_tester.o
$(LOCAL_BIN_DIR)/mathopt_tester: $(TARGET_LIB) $(MATHOPT_TESTER_OBJ)
	$(CXX) -o $@ $(MATHOPT_TESTER_OBJ) $(LIBS)

# -----------------------------------------------------------------------------

depend:
	makedepend $(CXXFLAGS) -Y $(SRCS)

clean:

clobber:
	rm -f $(TARGETS) $(VEC_RANGE_TESTER_OBJ) $(MATHOPT_TESTER_OBJ)

install_lib:

install_hdr:
	cp -pf $(HEADERS) $(PROJECT_INCLUDE_DIR)/.

# -----------------------------------------------------------------------------

## Local Variables:
## mode:Makefile
## tab-width:4
## End:
