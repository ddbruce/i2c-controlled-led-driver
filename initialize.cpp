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