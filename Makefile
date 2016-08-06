CXX = g++
CXXFLAGS = -O2 -g -MMD -MP

SRC = dlltest.cpp ai_tes.cpp rensya_window.cpp debug_window.cpp reversing/reverser.cpp \
	reversing/boss/boss_reverse.cpp reversing/player/player_reverse.cpp \
	reversing/bullet/bullet_reverse.cpp reversing/laser/laser_reverse.cpp \
	reversing/rev_utils.cpp reversing/draw_utils.cpp \
	reversing/rev_replstate.cpp conductor.cpp

OBJDIR = build
DLL_OBJS = $(addprefix $(OBJDIR)/, $(SRC:.cpp=.o))
DEPENDS = $(addprefix $(OBJDIR)/, $(SRC:.cpp=.d))

.PHONY: all clean
.SUFFIXES: .cpp .o

TARGET = a.exe dlltest.dll

all: $(TARGET)

a.exe: main.cpp 
	$(CXX) $(CXXFLAGS) main.cpp

dlltest.dll: $(DLL_OBJS)
	$(CXX) $(CXXFLAGS) -shared -o $@ $(DLL_OBJS) -lGdi32

clean:
	$(RM) $(TARGET) $(DLL_OBJS) $(DEPENDS)

$(OBJDIR)/%.o : %.cpp
#	echo $^
	$(CXX) $(CXXFLAGS) -o $@ -c $< 

-include $(DEPENDS)

# ŽQl http://boysenberrypi.hatenadiary.jp/entry/2014/03/15/113703

