# Name of the executable created
Target := SFMLPong

BinPath 			:= ./
BuildPath 			:= ./obj

SOURCE_DIRS			:= $(shell find src -type d -not -path "src")
SourcePath			:= $(shell find src -name '*.cpp')

CXX					:= g++
CXXFLAGS 			:= -ffast-math -g -Wall -std=c++11
CPPFLAGS        	:= -I/usr/include
LDFLAGS				:= -L.
LIBS 				:= -lsfml-audio -lsfml-system -lsfml-graphics -lsfml-window

EXECUTABLE 			:= $(BinPath)/$(Target)
SRC 	   			:= $(wildcard $(SourcePath)/*.cpp)					#no hace nada
OBJ					:= $(patsubst src/%.cpp,obj/%.o,$(SourcePath))

SOURCE_DIRS 		:= $(patsubst src/%,obj/%,$(SOURCE_DIRS))

#MAKE OPTIONS
.PHONY: all clean

all: prepare $(OBJ)
	$(info ----------------------------------------------)
	$(info Linking executable $(Target)...)
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(OBJ) -o $(EXECUTABLE) $(LDFLAGS) $(LIBS)
	$(info ----------------------------------------------)	
	$(info Compile OK)

obj/%.o: src/%.cpp
	$(info Compiling-> $@)
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

prepare:
	$(info ==============================================)
	$(info Creating folder structure)
	$(info ==============================================)
	@mkdir -p $(BinPath)

clean:
	$(info ==============================================)
	$(info Cleaning Objects and Binaries... )
	$(info ==============================================)
	@$(RM) $(OBJ)
	@$(RM) $(EXECUTABLE)
