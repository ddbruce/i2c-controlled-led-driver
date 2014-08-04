/*
Copyright (c) 2014  Daniel D. Bruce

This file is part of i2cledcontrol.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

The developer can be found at:
  daniel@danieldbruce.com

If that doesn't work, contact the Rensselaer Union
Administration office and ask. Chances are they'll have a
good idea as to where the developer went. Note that no
association with Rensselaer Polytechnic Institute shall
construe any copyright obligations to Rensselaer.
*/

#include <iostream>
#include <cstdlib>
#include <stdint.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <unistd.h>

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
	const char *filename = "/dev/i2c-1";
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