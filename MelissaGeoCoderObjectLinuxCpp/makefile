# Melissa GeoCoder Object Linux C++

CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++11 -g
LDLIBS = -lmdGeo
LDFLAGS = -L/home/jessicasusilo/test/github/GeoObject-Cpp-Linux/MelissaGeoCoderObjectLinuxCpp/Build
RM = rm -f

OUT_DIR = Build

MelissaGeoCoderObjectLinuxCpp: Main.cpp GeoObject.cpp mdGeo.h mdEnums.h | $(OUT_DIR)
	$(CXX) $(CXXFLAGS) Main.cpp GeoObject.cpp $(LDFLAGS) $(LDLIBS) -o $(OUT_DIR)/$@

clean:
	$(RM) *.o $(OUT_DIR)/MelissaGeoCoderObjectLinuxCpp
