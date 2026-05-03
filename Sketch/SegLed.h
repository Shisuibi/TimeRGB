
//==============================================================================//
//	Portable NTP Clock Timer Mood Light "TimeRGB" I/O Sketch for ESP32-S3-Zero	//
//				Implemented by Shisuibi --Grand Master Sorcerian--				//
//==============================================================================//


//==============================================================================//
#ifdef		UpperDefinition
//==============================================================================//


//==============================================================================//
enum {
	SegLedNum00,										//	7セグLED（数値00）
	SegLedNum01,										//	7セグLED（数値01）
	SegLedNum02,										//	7セグLED（数値02）
	SegLedNum03,										//	7セグLED（数値03）
	SegLedNum04,										//	7セグLED（数値04）
	SegLedNum05,										//	7セグLED（数値05）
	SegLedNum06,										//	7セグLED（数値06）
	SegLedNum07,										//	7セグLED（数値07）
	SegLedNum08,										//	7セグLED（数値08）
	SegLedNum09,										//	7セグLED（数値09）

	SegLedPos0A,										//	7セグLED（位置0A）
	SegLedPos0B,										//	7セグLED（位置0B）
	SegLedPos0C,										//	7セグLED（位置0C）
	SegLedPos0D,										//	7セグLED（位置0D）
	SegLedPos0E,										//	7セグLED（位置0E）
	SegLedPos0F,										//	7セグLED（位置0F）
	SegLedPos0G,										//	7セグLED（位置0G）

					SegLedTmp17,	SegLedTmp18,	SegLedTmp19,	//	未使用
	SegLedTmp20,	SegLedTmp21,	SegLedTmp22,	SegLedTmp23,	//	未使用
	SegLedTmp24,	SegLedTmp25,	SegLedTmp26,	SegLedTmp27,	//	未使用
	SegLedTmp28,	SegLedTmp29,	SegLedTmp30,	SegLedTmp31,	//	未使用

	SegLedMax,											//	7セグLED模様上限
};
//------------------------------------------------------------------------------//
#define		DecimalPoint				0x20			//	小数点ビット
//==============================================================================//


//==============================================================================//
#endif
//------------------------------------------------------------------------------//
#ifdef		LowerDefinition
//==============================================================================//


//==============================================================================//
static Uint08 aiSegLedPin[12] = {						//	7セグLED（番号）
	GpioLed7SA,		GpioLed7SB,		GpioLed7SC,		GpioLed7SD,
	GpioLed7SE,		GpioLed7SF,		GpioLed7SG,		GpioLed7SP,
	GpioLed7S1,		GpioLed7S2,		GpioLed7S3,		GpioLed7S4,
};
//------------------------------------------------------------------------------//
static Uint08 aiSegLedMap[SegLedMax * 2] = {			//	7セグLED（模様）
	0b00111111,		0b00000110,		0b01011011,		0b01001111,
	0b01100110,		0b01101101,		0b01111101,		0b00000111,
	0b01111111,		0b01101111,		0b00000001,		0b00000010,
	0b00000100,		0b00001000,		0b00010000,		0b00100000,
	0b01000000,		0b00000000,		0b00000000,		0b00000000,
	0b00000000,		0b00000000,		0b00000000,		0b00000000,
	0b00000000,		0b00000000,		0b00000000,		0b00000000,
	0b00000000,		0b00000000,		0b00000000,		0b00000000,

	0b10111111,		0b10000110,		0b11011011,		0b11001111,
	0b11100110,		0b11101101,		0b11111101,		0b10000111,
	0b11111111,		0b11101111,		0b10000001,		0b10000010,
	0b10000100,		0b10001000,		0b10010000,		0b10100000,
	0b11000000,		0b00000000,		0b00000000,		0b00000000,
	0b00000000,		0b00000000,		0b00000000,		0b00000000,
	0b00000000,		0b00000000,		0b00000000,		0b00000000,
	0b00000000,		0b00000000,		0b00000000,		0b00000000,
};
//------------------------------------------------------------------------------//
static Uint08 iAnodeCathode =							//	7セグLED（陽極・陰極）
										LOW;			//	自動識別で設定される
//------------------------------------------------------------------------------//
static Uint08 aiSegLedNum[XYZW];						//	7セグLED（数値）
//==============================================================================//


//==============================================================================//
static void SegLedDisp(void) {
	Uint08 iMap;
	Sint08 i, j;

	for(i = 0;i < XYZW;i++) {
		for(iMap = aiSegLedMap[aiSegLedNum[i]], j = 0;j < 8;j++, iMap >>= 1) {
			digitalWrite(aiSegLedPin[j], iAnodeCathode ^ (iMap & 0x01));
		}

		digitalWrite(aiSegLedPin[8 + i], iAnodeCathode ^ LOW);
		delayMicroseconds(1000);
		digitalWrite(aiSegLedPin[8 + i], iAnodeCathode ^ HIGH);
	}
}
//------------------------------------------------------------------------------//
static void SegLedReset(void) {
	Sint08 i;

	for(i = 0;i < XYZW;i++) {
		aiSegLedNum[i] = SegLedPos0G;
	}

	SegLedDisp();

	for(i = 0;i < XYZW;i++) {
		digitalWrite(aiSegLedPin[8 + i], iAnodeCathode ^ LOW);
	}
}
//------------------------------------------------------------------------------//
static void SegLedClear(void) {
	Sint08 i;

	for(i = 0;i < XYZW;i++) {
		digitalWrite(aiSegLedPin[8 + i], iAnodeCathode ^ HIGH);
	}
}
//==============================================================================//


//==============================================================================//
static void SegLedInit(void) {
	Sint08 i;

	for(i = 0;i < 12;i++) {
		pinMode(aiSegLedPin[i], OUTPUT);
		digitalWrite(aiSegLedPin[i], iAnodeCathode ^ ((i < 8)?LOW:HIGH));
	}

	//	共通陽極（Common Anode）共通陰極（Common Cathode）自動識別
	pinMode(GpioLed7SP, INPUT);
	iAnodeCathode = digitalRead(GpioLed7SP);
	pinMode(GpioLed7SP, OUTPUT);

	SegLedClear();
}
//------------------------------------------------------------------------------//
static void SegLedMove(void) {
	SegLedDisp();
}
//==============================================================================//


//==============================================================================//
#endif
//==============================================================================//

