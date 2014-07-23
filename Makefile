all: i2cledcontrol initialize

i2cledcontrol: i2cledcontrol.cpp
	g++ -o i2cledcontrol i2cledcontrol.cpp

initialize: initialize.cpp
	g++ -o initialize initialize.cpp

clean:
	rm -f i2cledcontrol initialize