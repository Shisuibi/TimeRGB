
//==============================================================================//
//	Portable NTP Clock Timer Mood Light "TimeRGB" I/O Sketch for ESP32-S3-Zero	//
//				Implemented by Shisuibi ～Grand Master Sorcerian～				//
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

	SegLedMax,											//	7セグLED模様上限
};
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
static Uint08 aiSegLedMap[SegLedMax][7] = {				//	7セグLED（模様）
	{	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	LOW ,	},
	{	LOW ,	HIGH,	HIGH,	LOW ,	LOW ,	LOW ,	LOW ,	},
	{	HIGH,	HIGH,	LOW ,	HIGH,	HIGH,	LOW ,	HIGH,	},
	{	HIGH,	HIGH,	HIGH,	HIGH,	LOW ,	LOW ,	HIGH,	},
	{	LOW ,	HIGH,	HIGH,	LOW ,	LOW ,	HIGH,	HIGH,	},
	{	HIGH,	LOW ,	HIGH,	HIGH,	LOW ,	HIGH,	HIGH,	},
	{	HIGH,	LOW ,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	},
	{	HIGH,	HIGH,	HIGH,	LOW ,	LOW ,	LOW ,	LOW ,	},
	{	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	HIGH,	},
	{	HIGH,	HIGH,	HIGH,	HIGH,	LOW ,	HIGH,	HIGH,	},

	{	HIGH,	LOW ,	LOW ,	LOW ,	LOW ,	LOW ,	LOW ,	},
	{	LOW ,	HIGH,	LOW ,	LOW ,	LOW ,	LOW ,	LOW ,	},
	{	LOW ,	LOW ,	HIGH,	LOW ,	LOW ,	LOW ,	LOW ,	},
	{	LOW ,	LOW ,	LOW ,	HIGH,	LOW ,	LOW ,	LOW ,	},
	{	LOW ,	LOW ,	LOW ,	LOW ,	HIGH,	LOW ,	LOW ,	},
	{	LOW ,	LOW ,	LOW ,	LOW ,	LOW ,	HIGH,	LOW ,	},
	{	LOW ,	LOW ,	LOW ,	LOW ,	LOW ,	LOW ,	HIGH,	},
};
//------------------------------------------------------------------------------//
static Uint08 aiAnodeCathode[2] = {						//	7セグLED（陽極・陰極）
	LOW ,	HIGH,										//	自動識別で設定される
};
//------------------------------------------------------------------------------//
static Uint08 aiSegLedNum[XYZW];						//	7セグLED（数値）
//==============================================================================//


//==============================================================================//
static void SegLedDisp(void) {
	Sint08 i, j;

	for(i = 0;i < XYZW;i++) {
		for(j = 0;j < 7;j++) {
			digitalWrite(aiSegLedPin[j], aiAnodeCathode[aiSegLedMap[aiSegLedNum[i] & 0x7F][j]]);
		}

		digitalWrite(aiSegLedPin[7], aiAnodeCathode[(aiSegLedNum[i] & 0x80)?HIGH:LOW]);

		digitalWrite(aiSegLedPin[8 + i], aiAnodeCathode[LOW ]);
		delayMicroseconds(1000);
		digitalWrite(aiSegLedPin[8 + i], aiAnodeCathode[HIGH]);
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
		digitalWrite(aiSegLedPin[8 + i], aiAnodeCathode[LOW ]);
	}
}
//------------------------------------------------------------------------------//
static void SegLedClear(void) {
	Sint08 i;

	for(i = 0;i < XYZW;i++) {
		digitalWrite(aiSegLedPin[8 + i], aiAnodeCathode[HIGH]);
	}
}
//==============================================================================//


//==============================================================================//
static void SegLedInit(void) {
	Sint08 i;

	for(i = 0;i < 12;i++) {
		pinMode(aiSegLedPin[i], OUTPUT);
		digitalWrite(aiSegLedPin[i], aiAnodeCathode[(i < 8)?LOW:HIGH]);
	}

	//	共通陽極（Common Anode）共通陰極（Common Cathode）自動識別
	pinMode(GpioLed7SP, INPUT);
	aiAnodeCathode[0] = digitalRead(GpioLed7SP) ^ LOW ;
	aiAnodeCathode[1] = digitalRead(GpioLed7SP) ^ HIGH;
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

