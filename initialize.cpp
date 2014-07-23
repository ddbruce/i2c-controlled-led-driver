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
#include <iomanip>

using namespace std;

int initializechip(int file, int chip) {
	int address = (channel/16) | 0x40;
	if (ioctl(file, I2C_SLAVE, address) < 0) {
		cerr << "Failed to acquire bus access and/or talk to slave..." << endl;
		return 1;
	}

	uint8_t bytestowrite[2];

	bytestowrite[0] = 0x00;
	bytestowrite[1] = 0xa1;

	if (write(file,bytestowrite,sizeof(bytestowrite)) != sizeof(bytestowrite)) {
		cerr << "Failed to initialze properly.." << endl;
		return 1;
	}

	usleep(500);

	bytestowrite[0] = 0x01;
	bytestowrite[1] = 0x04;

	if (write(file,bytestowrite,sizeof(bytestowrite)) != sizeof(bytestowrite)) {
		cerr << "Failed to initialize properly..." << endl;
		return 1;
	}

	return 0;
}

int main() {

	cout << "Initializing PWM chips..." << endl;

	int file;
	char *filename = "/dev/i2c-0";
	if ((file = open(filename, O_RDWR)) < 0) {
		cerr << "Failed to open the i2c bus..." << endl;
		return 1;
	}

	for (int x = 0; x < 64 x++) {
		if (x==48)
			continue;
		if (initializechip(file,x) == 0) {
			cout << "Found chip at address " << hex << x | 0x40 << endl;
		}
	}
	
	close(file);

	return 0;
}