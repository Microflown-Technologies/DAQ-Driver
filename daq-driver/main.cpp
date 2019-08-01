#include "VoyagerAbstractionLayer.h"

int main() {

	SerialDriverInterface m_serial;
	VoyagerAbstractionLayer m_voyager(&m_serial);


	m_serial.write("hallo", 5);



	//m_voyager.setGain(Aux1, HIGH);


	return 0;
}




