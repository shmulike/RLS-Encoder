#ifndef tl
#define tl

#if (arduino >=100)
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif


class RLS_Encoder {
	public:
		// Constructor
		RLS_Encoder(); 
		
		// Methods
		
		void begin(int baudRate=115200);
		void blink(int N);
		void get_status();
		void calibrate();
		void set_read();
		void start_response();
		void stop_response();
		void reset();
		int Encoder_Pos();
		float get_pos();
	
	private:
		void unlock();		
		void save_conf();
		uint32_t get_raw_data();
		void serialFlush();
};
#endif
