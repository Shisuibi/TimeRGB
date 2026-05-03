
//==============================================================================//
//	Portable NTP Clock Timer Mood Light "TimeRGB" I/O Sketch for ESP32-S3-Zero	//
//				Implemented by Shisuibi --Grand Master Sorcerian--				//
//==============================================================================//


//==============================================================================//
#ifdef		UpperDefinition
//==============================================================================//


//==============================================================================//
#define		NUL							0x00			//	ASCIIコード（NUL）
#define		LF							0x0A			//	ASCIIコード（LF）
#define		CR							0x0D			//	ASCIIコード（CR）
#define		SP							0x20			//	ASCIIコード（SP）

#define		CodeSysReset				0x01			//	ASCIIコード（Ctrl+A）
//------------------------------------------------------------------------------//
#define		SerialBaudRateTx			115200			//	通信端末ボーレート（送信）
#define		SerialSegSizeTx				0x0080			//	通信端末切片文字数（送信）
//------------------------------------------------------------------------------//
#define		BootBtnInterval				1000			//	ブート押釦間隔
//==============================================================================//


//==============================================================================//
#endif
//------------------------------------------------------------------------------//
#ifdef		LowerDefinition
//==============================================================================//


//==============================================================================//
static Cint08* pEscSeqClrScrn = "\e[H\e[J";				//	ESC順序（テキスト画面消去）
//------------------------------------------------------------------------------//
static Cint08* apAssignHelpText[] = {					//	ヘルプ画面テキスト
	"+-----------------------------------------------------------+",
	"|                Ctrl Key Assign (C = Ctrl)                 |",
	"+-----+-----------------------+-----+-----------------------+",
	"| C+A | System Reset          | C+G | Wi-Fi (Adjust Time)   |",
	"| C+S | Set Time (4Digit Num) +-----+-----------------------+",
	"| C+D | Clock/Timer (7SegLED) | C+K | Help (Ctrl Key)       |",
	"| C+F | NeoPixel (RGB LED)    | C+L | Clear Screen          |",
	"+-----+-----------------------+-----+-----------------------+",
	NULL,
};
//==============================================================================//


//==============================================================================//
static Cint08* pTransSysReset   = "ColdBoot[RST]";		//	システム再起動
static Cint08* pTransSetTime    = "Set Time[NUM]";		//	時刻設定

static Cint08* pTransSegModeClk = "7Seg LED[CLK]";		//	7セグLED（時刻）
static Cint08* pTransSegModeTmr = "7Seg LED[TMR]";		//	7セグLED（計時）

static Cint08* pTransRgbModeOff = "NeoPixel[OFF]";		//	NeoPixel（消灯）
static Cint08* pTransRgbModeOn  = "NeoPixel[ ON]";		//	NeoPixel（点灯）

static Cint08* pTransNtpWiFiDis = "NTP WiFi[DIS]";		//	NTP WiFi（禁止）
static Cint08* pTransNtpWiFiEna = "NTP WiFi[ENA]";		//	NTP WiFi（許可）
//==============================================================================//


//==============================================================================//
static Cint08* pTimeRgbName = "/TimeRGB";				//	TimeRGBファイル名称
static File TimeRgbFile;								//	TimeRGBファイル識別子
//------------------------------------------------------------------------------//
static Uint08 iHelpTextCount;							//	ヘルプ画面表示番号
static Uint32 iSerialMicros;							//	通信端末マイクロ秒
//------------------------------------------------------------------------------//
static Uint08 iPrevBootBtn = True;						//	ブート押釦過去
static Uint32 iBootBtnMillis = 0;						//	ブート押釦ミリ秒
//==============================================================================//


//==============================================================================//
static void TransData(Uint08 iData) {
	Serial.write(iData);
}
//------------------------------------------------------------------------------//
static void TransString(Cint08* pString) {
	while(*pString != NUL) TransData(*pString++);
}
//------------------------------------------------------------------------------//
static void TransMessage(Cint08* pMessage) {
	TransString(pMessage);
	TransData(CR);	TransData(LF);
}
//==============================================================================//


//==============================================================================//
static void ResetGpio(Sint08 iReset) {
	if(iReset == False) {
		BotBtnInput();
		NeoPixOutput();
	}

	NeoPixWrite(0x00, 0x00, 0x00);
}
//------------------------------------------------------------------------------//
static void ResetCtrl(Sint08 iReset) {
	if(iReset == False) {
		NtpWiFiLow();
	}

	NumTimeWrite(0);	SetTimeLow();
	SegModeLow();	RgbModeLow();
}
//==============================================================================//


//==============================================================================//
static void TransFileInit(void) {
	if(!SPIFFS.begin()) {
		TransMessage("Formatting file system");
		SPIFFS.format();	SPIFFS.begin();
	}
}
//------------------------------------------------------------------------------//
static void TransFileLoad(void) {
	if(TimeRgbFile = SPIFFS.open(pTimeRgbName, FILE_READ)) {
		TimeRgbFile.read(&iSysModeBit, sizeof(Uint08));
		TimeRgbFile.close();
	}
}
//------------------------------------------------------------------------------//
static void TransFileSave(void) {
	if(TimeRgbFile = SPIFFS.open(pTimeRgbName, FILE_WRITE)) {
		TimeRgbFile.write(&iSysModeBit, sizeof(Uint08));
		TimeRgbFile.close();
	}
}
//==============================================================================//


//==============================================================================//
static void TransSysReset(void) {
	ResetGpio(True);	ResetCtrl(True);
	TransMessage(pTransSysReset);
}
//------------------------------------------------------------------------------//
static void TransSetTime(void) {
	SetTimeHigh();	SegLedReset();
	TransMessage(pTransSetTime);
}
//------------------------------------------------------------------------------//
static void TransLedMode(void) {
	LedModeWrite(LedModeRead() + 1);

	if(SegModeRead() == False)		TransMessage(pTransSegModeClk);
	else						{	TransMessage(pTransSegModeTmr);		ClockReset(1);	}

			if(LedModeRead() == 0) TransMessage(pTransRgbModeOff);
	else	if(LedModeRead() == 2) TransMessage(pTransRgbModeOn );
}
//------------------------------------------------------------------------------//
static void TransSegMode(void) {
	if(SegModeRead() != False)	{	TransMessage(pTransSegModeClk);		SegModeLow();	}
	else						{	SegModeHigh();	TransMessage(pTransSegModeTmr);		ClockReset(1);	}
}
//------------------------------------------------------------------------------//
static void TransRgbMode(void) {
	if(RgbModeRead() != False)	{	TransMessage(pTransRgbModeOff);		RgbModeLow();	}
	else						{	RgbModeHigh();	TransMessage(pTransRgbModeOn);		}
}
//------------------------------------------------------------------------------//
static void TransNtpWiFi(void) {
	NeoPixWrite(0x00, 0x00, 0x00);

	if(NtpWiFiRead() != False)	{	TransMessage(pTransNtpWiFiDis);		NtpWiFiLow();	ClockWiFiDis();		}
	else						{	NtpWiFiHigh();	TransMessage(pTransNtpWiFiEna);		ClockWiFiEna();		}

	TransFileSave();
}
//------------------------------------------------------------------------------//
static void TransHelpText(void) {
	if(iHelpTextCount == 0xFF) iHelpTextCount = 0x00;
}
//------------------------------------------------------------------------------//
static void TransClrScrn(void) {
	TransString(pEscSeqClrScrn);
}
//==============================================================================//


//==============================================================================//
static void (* apSerialCtrl[SP])(void) = {
	NULL         ,	TransSysReset,	NULL         ,	NULL         ,
	TransSegMode ,	NULL         ,	TransRgbMode ,	TransNtpWiFi ,
	NULL         ,	NULL         ,	NULL         ,	TransHelpText,
	TransClrScrn ,	NULL         ,	NULL         ,	NULL         ,

	NULL         ,	NULL         ,	NULL         ,	TransSetTime ,
	NULL         ,	NULL         ,	NULL         ,	NULL         ,
	NULL         ,	NULL         ,	NULL         ,	NULL         ,
	NULL         ,	NULL         ,	NULL         ,	NULL         ,
};
//------------------------------------------------------------------------------//
static void TransControl(Uint08 iData) {
	(* apSerialCtrl[iData])();
}
//------------------------------------------------------------------------------//
static void TransCheck(Uint08 iData) {
	if((iData < SP)&&(apSerialCtrl[iData] != NULL)) {
		if((SetTimeRead() == False)||(iData == CodeSysReset)) {
			TransControl(iData);
			return;
		}
	}

	if(SetTimeRead() == False) return;
	if((iData < '0')||(iData > '9')) return;

	aiSegLedNum[NumTimeRead()] = iData - '0';
	NumTimeWrite(NumTimeRead() + 1);

	if(NumTimeRead() != 0) return;
	ClockChange();	SetTimeLow();
}
//==============================================================================//


//==============================================================================//
static void TransClear(void) {
	iHelpTextCount = 0xFF;
}
//------------------------------------------------------------------------------//
static Sint08 TransInterval(void) {
	if(		(micros() - iSerialMicros) >
			((1000000 * SerialSegSizeTx) / (SerialBaudRateTx >> 3))		) return(True);

	return(False);
}
//------------------------------------------------------------------------------//
static void TransWrite(void) {
	if((iHelpTextCount != 0xFF)&&(TransInterval() != False)) {
		if(apAssignHelpText[iHelpTextCount] != NULL)	TransMessage(apAssignHelpText[iHelpTextCount++]);
		else											iHelpTextCount = 0xFF;

		iSerialMicros = micros();
	}
}
//------------------------------------------------------------------------------//
static void TransRead(void) {
	if(Serial.available() == 0) return;
	TransCheck(Serial.read());
}
//------------------------------------------------------------------------------//
static void TransBoot(void) {
	if(SetTimeRead() != False) {	iBootBtnMillis = 0;		return;		}

	if(iBootBtnMillis > 0) {
				if(BotBtnRead() == True) {
			TransLedMode();		iBootBtnMillis = 0;
		} else	if((millis() - iBootBtnMillis) > BootBtnInterval) {
			TransNtpWiFi();		iBootBtnMillis = 0;
		}
	}

	if((iPrevBootBtn == True)&&(BotBtnRead() == False)) iBootBtnMillis = millis();
	iPrevBootBtn = BotBtnRead();
}
//==============================================================================//


//==============================================================================//
static void TransInit(void) {
	Serial.begin(SerialBaudRateTx);
	TransClear();	iSerialMicros = micros();

	ResetGpio(False);	ResetCtrl(False);
	TransFileInit();	TransFileLoad();

	if(NtpWiFiRead() != False) ClockAdjust();
}
//------------------------------------------------------------------------------//
static void TransMove(void) {
	TransWrite();
	TransRead();
	TransBoot();
}
//==============================================================================//


//==============================================================================//
#endif
//==============================================================================//

