
//==============================================================================//
//	Portable NTP Clock Timer Mood Light "TimeRGB" I/O Sketch for ESP32-S3-Zero	//
//				Implemented by Shisuibi ～Grand Master Sorcerian～				//
//==============================================================================//


//==============================================================================//
#ifdef		UpperDefinition
//==============================================================================//


//==============================================================================//
#define		AdjustInterval	(1000000 / (1000 * 4 * 6))	//	調整間隔
//==============================================================================//


//==============================================================================//
#endif
//------------------------------------------------------------------------------//
#ifdef		LowerDefinition
//==============================================================================//


//==============================================================================//
static Uint08 aiClockSegLed[6][4] = {					//	7セグLED（位置）
	{	SegLedPos0A,	SegLedPos0A,	SegLedPos0A,	SegLedPos0D,	},
	{	SegLedPos0B,	SegLedPos0F,	SegLedPos0B,	SegLedPos0E,	},
	{	SegLedPos0F,	SegLedPos0B,	SegLedPos0C,	SegLedPos0F,	},
	{	SegLedPos0C,	SegLedPos0E,	SegLedPos0D,	SegLedPos0A,	},
	{	SegLedPos0E,	SegLedPos0C,	SegLedPos0E,	SegLedPos0B,	},
	{	SegLedPos0D,	SegLedPos0D,	SegLedPos0F,	SegLedPos0C,	},
};
//------------------------------------------------------------------------------//
static Uint32 aiTimerMillis[2];							//	システム時刻ミリ秒

static Uint08 aiTimerCentis[2];							//	システム時刻（厘）
static Uint08 aiTimerSecond[2];							//	システム時刻（秒）

static Uint08 aiTimerMinute[2];							//	システム時刻（分）
static Uint08 aiTimerHour24[2];							//	システム時刻（時）
//==============================================================================//


//==============================================================================//
static void ClockWiFiDis(void) {
	Sint16 i, j;

	for(i = 0;i < 6;i++) {
		aiSegLedNum[X] = aiSegLedNum[Z] = aiClockSegLed[i][0];
		aiSegLedNum[Y] = aiSegLedNum[W] = aiClockSegLed[i][1];

		for(j = 0;j < AdjustInterval;j++) SegLedDisp();
	}
}
//------------------------------------------------------------------------------//
static void ClockWiFiEna(void) {
	Sint16 i, j;

	for(i = 0;i < 6;i++) {
		aiSegLedNum[X] = aiSegLedNum[Z] = aiClockSegLed[5 - i][0];
		aiSegLedNum[Y] = aiSegLedNum[W] = aiClockSegLed[5 - i][1];

		for(j = 0;j < AdjustInterval;j++) SegLedDisp();
	}
}
//------------------------------------------------------------------------------//
static void ClockAdjust(void) {
	struct tm TimeInfo;
	Sint16 i, j;

	SegLedReset();

#ifdef		WiFiSSIDPSWD
	WiFi.begin(WiFiSSIDPSWD);
#else
	WiFi.begin();
#endif

	while(WiFi.status() != WL_CONNECTED) {
		SegLedClear();

		for(i = 0;i < 6;i++) {
			aiSegLedNum[X] = aiClockSegLed[i][2];
			aiSegLedNum[Y] = aiClockSegLed[i][3];

			for(j = 0;j < AdjustInterval;j++) SegLedDisp();
		}

		SegLedReset();
	}

	while(!getLocalTime(&TimeInfo)) {
		SegLedClear();

		for(i = 0;i < 6;i++) {
			aiSegLedNum[Z] = aiClockSegLed[i][2];
			aiSegLedNum[W] = aiClockSegLed[i][3];

			for(j = 0;j < AdjustInterval;j++) SegLedDisp();
		}

		SegLedReset();
	}

	aiTimerHour24[0] = TimeInfo.tm_hour;	aiTimerMinute[0] = TimeInfo.tm_min;
	aiTimerSecond[0] = TimeInfo.tm_sec;		aiTimerCentis[0] = 0;

	aiTimerMillis[0] = millis();
	WiFi.disconnect(True);

	SegLedClear();
}
//------------------------------------------------------------------------------//
static void ClockChange(void) {
	if(SegModeRead() == False) {
		if((aiTimerHour24[0] = aiSegLedNum[X] * 10 + aiSegLedNum[Y]) > 23) aiTimerHour24[0] = 0;
		if((aiTimerMinute[0] = aiSegLedNum[Z] * 10 + aiSegLedNum[W]) > 59) aiTimerMinute[0] = 0;

		aiTimerSecond[0] = aiTimerCentis[0] = 0;
		aiTimerMillis[0] = millis();
	} else {
		if((aiTimerMinute[1] = aiSegLedNum[X] * 10 + aiSegLedNum[Y]) > 59) aiTimerMinute[1] = 0;
		if((aiTimerSecond[1] = aiSegLedNum[Z] * 10 + aiSegLedNum[W]) > 59) aiTimerSecond[1] = 0;

		aiTimerHour24[1] = aiTimerCentis[1] = 0;
		aiTimerMillis[1] = millis();
	}
}
//==============================================================================//


//==============================================================================//
static void ClockReset(Sint08 iMode) {
	aiTimerMillis[iMode] = millis();

	aiTimerCentis[iMode] = aiTimerSecond[iMode] = 0;
	aiTimerMinute[iMode] = aiTimerHour24[iMode] = 0;
}
//------------------------------------------------------------------------------//
static void ClockTimer(void) {
	Uint32 iMillis;
	Sint08 i;

	for(i = 0;i < 2;i++) {
		iMillis = millis() - aiTimerMillis[i];

		while(iMillis > 999) {
			iMillis -= 1000;	aiTimerMillis[i] += 1000;

			if(++aiTimerSecond[i] > 59) {
				aiTimerSecond[i] = 0;

				if(++aiTimerMinute[i] > 59) {
					aiTimerMinute[i] = 0;

					if(++aiTimerHour24[i] > 23) aiTimerHour24[i] = 0;
				}
			}
		}

		aiTimerCentis[i] = (Uint08)(iMillis / 10);
	}
}
//------------------------------------------------------------------------------//
static void ClockDisp(void) {
	Uint08 iSM = SegModeRead();
	if(SetTimeRead() != False) return;

	if(iSM == False) {
		aiSegLedNum[X] = aiTimerHour24[iSM] / 10;	if(aiTimerSecond[iSM] & 0x08) aiSegLedNum[X] |= DecimalPoint;
		aiSegLedNum[Y] = aiTimerHour24[iSM] % 10;	if(aiTimerSecond[iSM] & 0x04) aiSegLedNum[Y] |= DecimalPoint;
		aiSegLedNum[Z] = aiTimerMinute[iSM] / 10;	if(aiTimerSecond[iSM] & 0x02) aiSegLedNum[Z] |= DecimalPoint;
		aiSegLedNum[W] = aiTimerMinute[iSM] % 10;	if(aiTimerSecond[iSM] & 0x01) aiSegLedNum[W] |= DecimalPoint;
	} else {
		aiSegLedNum[X] = aiTimerMinute[iSM] / 10;	if(aiTimerCentis[iSM] > 80) aiSegLedNum[X] |= DecimalPoint;
		aiSegLedNum[Y] = aiTimerMinute[iSM] % 10;	if(aiTimerCentis[iSM] > 60) aiSegLedNum[Y] |= DecimalPoint;
		aiSegLedNum[Z] = aiTimerSecond[iSM] / 10;	if(aiTimerCentis[iSM] > 40) aiSegLedNum[Z] |= DecimalPoint;
		aiSegLedNum[W] = aiTimerSecond[iSM] % 10;	if(aiTimerCentis[iSM] > 20) aiSegLedNum[W] |= DecimalPoint;
	}
}
//==============================================================================//


//==============================================================================//
static void ClockInit(void) {
	ClockReset(0);	ClockReset(1);
	configTzTime("JST-9", "ntp.nict.jp", "ntp.jst.mfeed.ad.jp");
}
//------------------------------------------------------------------------------//
static void ClockMove(void) {
	ClockTimer();
	ClockDisp();
}
//==============================================================================//


//==============================================================================//
#endif
//==============================================================================//

