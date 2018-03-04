## Hossein Moein
## August 2, 2009

LOCAL_LIB_DIR = ../lib/$(BUILD_PLATFORM)
LOCAL_BIN_DIR = ../bin/$(BUILD_PLATFORM)
LOCAL_OBJ_DIR = ../obj/$(BUILD_PLATFORM)
LOCAL_INCLUDE_DIR = ../include
PROJECT_LIB_DIR = ../../lib/$(BUILD_PLATFORM)
PROJECT_BIN_DIR = ../../bin/$(BUILD_PLATFORM)
PROJECT_INCLUDE_DIR = ../../include

# -----------------------------------------------------------------------------

SRCS =
HEADERS = $(LOCAL_INCLUDE_DIR)/MathOperators.h \
          $(LOCAL_INCLUDE_DIR)/SymmMatrixBase.h \
          $(LOCAL_INCLUDE_DIR)/SymmMatrixBase.tcc \
          $(LOCAL_INCLUDE_DIR)/MatrixBase.h \
          $(LOCAL_INCLUDE_DIR)/MatrixBase.tcc \
          $(LOCAL_INCLUDE_DIR)/DenseMatrixBase.h \
          $(LOCAL_INCLUDE_DIR)/DenseMatrixBase.tcc \
          $(LOCAL_INCLUDE_DIR)/Matrix.h \
          $(LOCAL_INCLUDE_DIR)/Matrix.tcc

LIB_NAME =
TARGET_LIB =

TARGETS += $(LOCAL_BIN_DIR)/matrix_tester

INSTALL_TARGETS =

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
.SUFFIXES: .cc .pl

$(LOCAL_OBJ_DIR)/%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

.cc :
	$(CXX) $(CXXFLAGS) $< -o $@ -lm $(TLIB) -lg++

# -----------------------------------------------------------------------------

all: $(TARGETS)

MATRIX_TESTER_OBJ = $(LOCAL_OBJ_DIR)/matrix_tester.o
$(LOCAL_BIN_DIR)/matrix_tester: $(MATRIX_TESTER_OBJ)
	$(CXX) -o $@ $(MATRIX_TESTER_OBJ) $(LIBS)

# -----------------------------------------------------------------------------

depend:
	makedepend $(CXXFLAGS) -Y $(SRCS)

clobber:
	rm -f $(TARGETS) $(MATRIX_TESTER_OBJ)

install_lib:
	cp -pf $(TARGET_LIB) $(PROJECT_LIB_DIR)/.

install_hdr:
	cp -pf $(HEADERS) $(PROJECT_INCLUDE_DIR)/.

install_bin:
	cp -pf $(INSTALL_TARGETS) $(PROJECT_BIN_DIR)/.

# -----------------------------------------------------------------------------

## Local Variables:
## mode:Makefile
## tab-width:4
## End:
