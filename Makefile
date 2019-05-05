CXX = g++


crs:
	$(CXX)  v3crs.cpp -o $@ -lpthread

n1:
	$(CXX)  v4n1.cpp -o $@ -lpthread

n2:
	$(CXX)  v4n2.cpp -o $@ -lpthread

n3:
	$(CXX)  v4n3.cpp -o $@ -lpthread