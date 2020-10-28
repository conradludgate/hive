#
# Compiler flags
#
CXX = g++
CXXFLAGS = -std=c++1z -I include
_LIBS = glfw GL dl glut GLU
LIBS = $(addprefix -l, $(_LIBS))

#
# Project files
#
SRCDIR = src
SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(SRCS:$(SRCDIR)/%.cpp=%.o)
EXE  = hive

#
# Debug build settings
#
DBGDIR = debug
DBGEXE = $(DBGDIR)/$(EXE)
DBGOBJS = $(addprefix $(DBGDIR)/, $(OBJS))
DBGCXXFLAGS = -g -O0 -DDEBUG

#
# Release build settings
#
RELDIR = release
RELEXE = $(RELDIR)/$(EXE)
RELOBJS = $(addprefix $(RELDIR)/, $(OBJS))
RELCXXFLAGS = -O3 -DNDEBUG

src/glad.c:
	glad --profile="compatibility" --api="gl=3.3" --generator="c" --spec="gl" --out-path="."

.PHONY: all clean debug prep release remake

# Default build
all: prep release

#
# Debug rules
#
debug: $(DBGEXE)

$(DBGEXE): $(DBGDIR)/glad.o $(DBGOBJS)
	$(CXX) $(CXXFLAGS) $(DBGCXXFLAGS) $(LIBS) -o $(DBGEXE) $^

$(DBGDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) -c $(CXXFLAGS) $(DBGCXXFLAGS) $(LIBS) -o $@ $<

$(DBGDIR)/glad.o: src/glad.c
	$(CXX) -c $(CXXFLAGS) $(DBGCXXFLAGS) $(LIBS) -o $@ $<

#
# Release rules
#
release: $(RELEXE)

$(RELEXE): $(RELDIR)/glad.o $(RELOBJS)
	$(CXX) $(CXXFLAGS) $(RELCXXFLAGS) $(MACROS) $(LIBS) -o $(RELEXE) $^

$(RELDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) -c $(CXXFLAGS) $(RELCXXFLAGS) $(MACROS) $(LIBS) -o $@ $<

$(RELDIR)/glad.o: src/glad.c
	$(CXX) -c $(CXXFLAGS) $(RELCXXFLAGS) $(MACROS) $(LIBS) -o $@ $<

#
# Other rules
#
prep:
	@mkdir -p $(DBGDIR) $(RELDIR)

remake: clean all

clean: # Don't remake glad.cpp
	rm -f $(RELEXE) $(RELOBJS:$(RELDIR)/glad.o=) $(DBGEXE) $(DBGOBJS:$(DBGDIR)/glad.o=)