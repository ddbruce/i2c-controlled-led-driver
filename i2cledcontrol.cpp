#include <iostream>
#include <cstdlib>
#include <stdint.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

using namespace std;

#define LED0_ON_L 0x6
#define LED0_ON_H 0x7
#define LED0_OFF_L 0x8
#define LED0_OFF_H 0x9

//there are 16 "channels" on each "board"
//there are 3 "channels" per "strip"
//each "board" has one PWM chip


int setPWM(int channel, int value, int file) {
	int address = (channel/16) | 0x40;
	if (ioctl(file, I2C_SLAVE, address) < 0) {
		cerr << "Failed to acquire bus access and/or talk to slave..." << endl;
		return 1;
	}

	uint8_t bytestowrite[5];

	bytestowrite[0] = LED0_ON_L+4*(channel%16);
	bytestowrite[1] = 0;
	bytestowrite[2] = 0;
	bytestowrite[3] = value*16;
	bytestowrite[4] = (value*16) >> 8;


	if (write(file,bytestowrite,5) != 5) {
		cerr << "Failed to write to the i2c bus..." << endl;
		return 1;
	}

	return 0;
}

int main(int argc, char *argv[]) {

	if (argc != 3) {
		cerr << "Incorrect number of arguments..." << endl;
		return 1;
	}

	int strip = atoi(argv[1]);
	long colors = strtol(argv[2], NULL, 16);
	uint8_t red = colors >> 16;
	uint8_t green = (colors >> 8) & 0xff;
	uint8_t blue = colors & 0xff;

	//cout << "R=" << int(red) << " G=" << int(green) << " B=" << int(blue) << endl;

	int file;
	char *filename = "/dev/i2c-0";
	if ((file = open(filename, O_RDWR)) < 0) {
		cerr << "Failed to open the i2c bus..." << endl;
		return 1;
	}

	setPWM(strip*3,red,file);
	setPWM(strip*3+1,green,file);
	setPWM(strip*3+2,blue,file);

	close(file);

	return 0;
}