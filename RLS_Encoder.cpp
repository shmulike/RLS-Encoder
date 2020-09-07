#include "RLS_Encoder.h"
#define onBoardLED 13
#define PPR17 131072.0		// 17Bits 2^17
#define PPR18 262144.0		// 18Bits 2^18
#define delay_time 3

// Constructor
RLS_Encoder::RLS_Encoder(){
	//pinMode(onBoardLED, OUTPUT);
}

//Methods
void RLS_Encoder::blink(int N){
	pinMode(13, OUTPUT);
	for (int i=0; i<N; i++){
		digitalWrite(onBoardLED, HIGH);
		delay(200);
		digitalWrite(onBoardLED, LOW);
		delay(200);
	}
}

void RLS_Encoder::begin(int baudRate){
	Serial2.begin(115200);
	while(!Serial2);
	//Serial.begin(baudRate);
	//while(!Serial);
	//Serial.println("-->Encoder Started.");
}

void RLS_Encoder::get_status(){
	Serial2.flush();
	Serial2.write(0x69); delay(delay_time);
	byte b1 = Serial2.read();
	byte b2 = Serial2.read();
	Serial.print("--> Encoder status: ");
	Serial.print(b1);
	Serial.print(":");
	Serial.print(b2);
	Serial.println(";");
}

void RLS_Encoder::unlock(){
	Serial2.write(0xCD); delay(delay_time);
	Serial2.write(0xEF); delay(delay_time);
	Serial2.write(0x89); delay(delay_time);
	Serial2.write(0xAB); delay(delay_time);
}

void RLS_Encoder::calibrate(){
	// Start calibration sequence
	Serial.println("--> Starting calibration sequence: ");
	RLS_Encoder::unlock();
	Serial2.write(0x41); delay(delay_time);
	Serial.println("--> calibration sequence Ended.");
	RLS_Encoder::get_status();
}

void RLS_Encoder::set_read(){
	RLS_Encoder::unlock();
	Serial2.write(0x54); delay(delay_time);		// Continues Read = 'T'
	Serial2.write(0x01); delay(delay_time);		// Auto-start enable = 1
	Serial2.write(0x33); delay(delay_time);		// Short response = '3'
	Serial2.write(0x00); delay(delay_time);		// Period (250 micro-sec) (byte 8)
	Serial2.write(0xFA); delay(delay_time);		// Period (250 micro-sec) (byte 9)
	Serial.println("--> Encoder Set to Continues read mode '1'.\n\tDont forget to start using 'start_response()'");
	RLS_Encoder::save_conf();
	//RLS_Encoder::get_status();
}

void RLS_Encoder::start_response(){
	//RLS_Encoder::unlock();
	Serial2.write(0x53); delay(delay_time);		// Continues Read = 'S'
	Serial.println("--> Encoder Set to start continues read.");
	RLS_Encoder::save_conf();
	//RLS_Encoder::get_status();
}

void RLS_Encoder::stop_response(){
	//RLS_Encoder::unlock();
	Serial2.write(0x50); delay(delay_time);		// Continues Read = 'P'
	Serial.println("--> Encoder Set to stop continues read.");
	RLS_Encoder::save_conf();
	//RLS_Encoder::get_status();
}

void RLS_Encoder::reset(){
	RLS_Encoder::unlock();
	Serial2.write(0x72); delay(delay_time);		// reset configuration parameters = 'r'
	Serial.println("--> Encoder reset to factory settings.");
}

int RLS_Encoder::Encoder_Pos(){
	return Serial2.parseInt();
}

void RLS_Encoder::serialFlush(){
	char t;
	while (Serial2.available() > 0)
		t = Serial2.read();
}

float RLS_Encoder::get_pos(){
	RLS_Encoder::serialFlush();
	//delay(5);
	while (Serial2.available() < 3);
	byte b1 = Serial2.read();
	byte b2 = Serial2.read();
	byte b3 = Serial2.read();
	uint32_t pos = b3 + (b2<<8) + (b1<<16);

	pos = (pos>>6);
	float pos_ang = (float)(pos*360.0/PPR18);
	return pos_ang;
}

void RLS_Encoder::save_conf(){
	RLS_Encoder::unlock();
	Serial2.write(0x63); delay(delay_time);		// save configuration parameters = 'c'
	Serial.println("--> Encoders Configurations saved.\nYou have to reset the Encoder.");
}

uint32_t RLS_Encoder::get_raw_data(){
	byte b1 = Serial2.read();
	byte b2 = Serial2.read();
	byte b3 = Serial2.read();
	uint32_t pos = b3 + (b2<<8) + (b1<<16);
	return (pos>>7);
}


