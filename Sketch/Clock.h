
//==============================================================================//
//	Portable NTP Clock Timer Mood Light "TimeRGB" I/O Sketch for ESP32-S3-Zero	//
//				Implemented by Shisuibi --Grand Master Sorcerian--				//
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
static Cint08* pClockAdjustWiFi = "WiFi |";				//	時刻調整（WiFi接続）
static Cint08* pClockAdjustNICT = "NICT |";				//	時刻調整（NICT接続）

static Cint08* pClockAdjustExec = "o";					//	時刻調整（接続実行）
static Cint08* pClockAdjustDone = "| Done";				//	時刻調整（接続完了）

static Cint08* pClockAdjustCancel  = "| Cancel";		//	時刻調整（接続取消）
static Cint08* pClockAdjustTimeout = "| Timeout";		//	時刻調整（接続打切）
//------------------------------------------------------------------------------//
static Uint08 aiClockSegLed[6][4] = {					//	7セグLED（位置）
	{	SegLedPos0A,	SegLedPos0A,	SegLedPos0A,	SegLedPos0D,	},
	{	SegLedPos0B,	SegLedPos0F,	SegLedPos0B,	SegLedPos0E,	},
	{	SegLedPos0F,	SegLedPos0B,	SegLedPos0C,	SegLedPos0F,	},
	{	SegLedPos0C,	SegLedPos0E,	SegLedPos0D,	SegLedPos0A,	},
	{	SegLedPos0E,	SegLedPos0C,	SegLedPos0E,	SegLedPos0B,	},
	{	SegLedPos0D,	SegLedPos0D,	SegLedPos0F,	SegLedPos0C,	},
};
//------------------------------------------------------------------------------//
static time_t iCurrTime;								//	現在時刻
static struct tm TimeInfo;								//	時刻情報

static Uint08 iClockSec;								//	ローカル時刻（秒）
static Uint08 iClockMin;								//	ローカル時刻（分）
static Uint08 iClockHour;								//	ローカル時刻（時）

static Uint08 iClockDay;								//	ローカル時刻（日）
static Uint08 iClockMon;								//	ローカル時刻（月）
static Uint08 iClockYear;								//	ローカル時刻（年）

static Uint08 iClockWeek;								//	ローカル時刻（曜）
//------------------------------------------------------------------------------//
static Uint32 iTimerMillis;								//	システム時刻ミリ秒

static Uint08 iTimerCentis;								//	システム時刻（厘）
static Uint08 iTimerSecond;								//	システム時刻（秒）

static Uint08 iTimerMinute;								//	システム時刻（分）
static Uint08 iTimerHour24;								//	システム時刻（時）
//==============================================================================//


//==============================================================================//
static void ClockClear(void) {
	iTimerMillis = millis();

	iTimerCentis = iTimerSecond = 0;
	iTimerMinute = iTimerHour24 = 0;
}
//------------------------------------------------------------------------------//
static void ClockReset(void) {
	if(SegModeRead() == False) {
		iTimerMillis = millis();

		iTimerCentis =         0;	iTimerSecond = iClockSec;
		iTimerMinute = iClockMin;	iTimerHour24 = iClockHour;
	} else ClockClear();
}
//------------------------------------------------------------------------------//
static void ClockLocal(void) {
	time_t iPrevTime = iCurrTime;

	if(time(&iCurrTime) == iPrevTime) return;
	localtime_r(&iCurrTime, &TimeInfo);

	iClockSec  = TimeInfo.tm_sec ;
	iClockMin  = TimeInfo.tm_min ;
	iClockHour = TimeInfo.tm_hour;

	iClockDay  = TimeInfo.tm_mday;
	iClockMon  = TimeInfo.tm_mon ;
	iClockYear = TimeInfo.tm_year;

	iClockWeek = TimeInfo.tm_wday;
}
//------------------------------------------------------------------------------//
static void ClockTimer(void) {
	Uint32 iMillis = millis() - iTimerMillis;

	while(iMillis > 999) {
		iMillis -= 1000;	iTimerMillis += 1000;

		if(++iTimerSecond > 59) {
			iTimerSecond = 0;

			if(++iTimerMinute > 59) {
				iTimerMinute = 0;

				if(++iTimerHour24 > 23) iTimerHour24 = 0;
			}
		}
	}

	iTimerCentis = (Uint08)(iMillis / 10);
}
//------------------------------------------------------------------------------//
static void ClockDisp(void) {
	if(SetTimeRead() != False) return;

	if(SegModeRead() == False) {
		aiSegLedNum[X] = iTimerHour24 / 10;		if((iTimerSecond % 10) & 0x08) aiSegLedNum[X] |= DecimalPoint;
		aiSegLedNum[Y] = iTimerHour24 % 10;		if((iTimerSecond % 10) & 0x04) aiSegLedNum[Y] |= DecimalPoint;
		aiSegLedNum[Z] = iTimerMinute / 10;		if((iTimerSecond % 10) & 0x02) aiSegLedNum[Z] |= DecimalPoint;
		aiSegLedNum[W] = iTimerMinute % 10;		if((iTimerSecond % 10) & 0x01) aiSegLedNum[W] |= DecimalPoint;
	} else {
		aiSegLedNum[X] = iTimerMinute / 10;		if(iTimerCentis > 80) aiSegLedNum[X] |= DecimalPoint;
		aiSegLedNum[Y] = iTimerMinute % 10;		if(iTimerCentis > 60) aiSegLedNum[Y] |= DecimalPoint;
		aiSegLedNum[Z] = iTimerSecond / 10;		if(iTimerCentis > 40) aiSegLedNum[Z] |= DecimalPoint;
		aiSegLedNum[W] = iTimerSecond % 10;		if(iTimerCentis > 20) aiSegLedNum[W] |= DecimalPoint;
	}
}
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
static Sint08 ClockRotate(Uint08 iDigit) {
	Sint16 i, j;
	SegLedClear();

	for(i = 0;i < 6;i++) {
		aiSegLedNum[iDigit + 0] = aiClockSegLed[i][2];
		aiSegLedNum[iDigit + 1] = aiClockSegLed[i][3];

		for(j = 0;j < AdjustInterval;j++) SegLedDisp();
	}

	ClockTimer();

	if(BotBtnRead() == False) {
		while(BotBtnRead() == False);
		TransMessage(pClockAdjustCancel);	return(True);
	}

	if(iTimerMinute > 0) {
		TransMessage(pClockAdjustTimeout);	return(True);
	}

	SegLedReset();
	return(False);
}
//------------------------------------------------------------------------------//
static void ClockAdjust(void) {
	TransMessage("Attempting time adjustment");

#ifdef		WiFiSSIDPSWD
	WiFi.begin(WiFiSSIDPSWD);
#else
	WiFi.begin();
#endif
	SegLedReset();

	for(TransString(pClockAdjustWiFi);WiFi.status() != WL_CONNECTED;TransString(pClockAdjustExec)) {
		if(ClockRotate(X) != False) {	WiFi.disconnect(True);	return;		}
	}

	TransMessage(pClockAdjustDone);

	for(TransString(pClockAdjustNICT);!getLocalTime(&TimeInfo, 100);TransString(pClockAdjustExec)) {
		if(ClockRotate(Z) != False) {	WiFi.disconnect(True);	return;		}
	}

	TransMessage(pClockAdjustDone);

	SegLedClear();
	WiFi.disconnect(True);

	strftime((char*)aiStringBuf, StringSizeL, "%Y/%m/%d %a %H:%M:%S", &TimeInfo);
	TransMessage((Cint08*)aiStringBuf);
}
//------------------------------------------------------------------------------//
static void ClockChange(void) {
	if(SegModeRead() == False) {
		if((iTimerHour24 = aiSegLedNum[X] * 10 + aiSegLedNum[Y]) > 23) iTimerHour24 = 0;
		if((iTimerMinute = aiSegLedNum[Z] * 10 + aiSegLedNum[W]) > 59) iTimerMinute = 0;

		iTimerSecond = iTimerCentis = 0;
		iTimerMillis = millis();
	} else {
		if((iTimerMinute = aiSegLedNum[X] * 10 + aiSegLedNum[Y]) > 59) iTimerMinute = 0;
		if((iTimerSecond = aiSegLedNum[Z] * 10 + aiSegLedNum[W]) > 59) iTimerSecond = 0;

		iTimerHour24 = iTimerCentis = 0;
		iTimerMillis = millis();
	}
}
//==============================================================================//


//==============================================================================//
static void ClockInit(void) {
	iCurrTime = 0;	ClockClear();
	configTzTime("JST-9", "ntp.nict.jp", "ntp.jst.mfeed.ad.jp");
}
//------------------------------------------------------------------------------//
static void ClockMove(void) {
	ClockLocal();
	ClockTimer();
	ClockDisp();
}
//==============================================================================//


//==============================================================================//
#endif
//==============================================================================//

