
//==============================================================================//
//	Portable NTP Clock Timer Mood Light "TimeRGB" I/O Sketch for ESP32-S3-Zero	//
//				Implemented by Shisuibi ～Grand Master Sorcerian～				//
//==============================================================================//


//==============================================================================//
#ifdef		UpperDefinition
//==============================================================================//


//==============================================================================//
enum {
	X, Y, Z, XY = Z, W, XYZ = W, XYZW,					//	マトリクス参照
};
//------------------------------------------------------------------------------//
#define		False						0				//	論理値（偽）
#define		True						1				//	論理値（真）
//------------------------------------------------------------------------------//
#define		StringSizeS					0x0010			//	文字列幅（短）
#define		StringSizeL					0x0100			//	文字列幅（長）
//------------------------------------------------------------------------------//
typedef		const char					Cint08;			//	符号無し文字（08bit）
//------------------------------------------------------------------------------//
typedef		unsigned char				Uint08;			//	符号無し整数（08bit）
typedef		signed char					Sint08;			//	符号有り整数（08bit）

typedef		unsigned short				Uint16;			//	符号無し整数（16bit）
typedef		signed short				Sint16;			//	符号有り整数（16bit）

typedef		unsigned long				Uint32;			//	符号無し整数（32bit）
typedef		signed long					Sint32;			//	符号有り整数（32bit）
//------------------------------------------------------------------------------//
typedef		signed long					Sfix08;			//	固定小数点数（08bit）
typedef		signed long					Sfix16;			//	固定小数点数（16bit）
typedef		signed long					Sfix24;			//	固定小数点数（24bit）
//------------------------------------------------------------------------------//
typedef		float						Sflt32;			//	浮動小数点数（32bit）
typedef		double						Sflt64;			//	浮動小数点数（64bit）
//==============================================================================//


//==============================================================================//
enum {
	GpioBotBtn,

	GpioLed7SE,		GpioLed7SD,		GpioLed7SP,		GpioLed7SC,
	GpioLed7SG,		GpioLed7S4,		GpioLed7SB,		GpioLed7S3,

	GpioLed7S2,		GpioLed7SF,		GpioLed7SA,		GpioLed7S1,
	NotAvail13,		NotAvail14,		NotAvail15,		NotAvail16,

	NotAvail17,		NotAvail18,		NotAvail19,		NotAvail20,
	GpioNeoPix,
};
//------------------------------------------------------------------------------//
#define		BotBtnInput()				{	pinMode(GpioBotBtn, INPUT_PULLUP);	}
#define		BotBtnRead()				(	digitalRead(GpioBotBtn)		)
//------------------------------------------------------------------------------//
#define		NeoPixOutput()				{	pinMode(GpioNeoPix, OUTPUT);	}
#define		NeoPixWrite(iCR, iCG, iCB)	{	neopixelWrite(GpioNeoPix, iCR, iCG, iCB);	}
//==============================================================================//


//==============================================================================//
//				|	Bit 07/03	|	Bit 06/02	|	Bit 05/01	|	Bit 04/00	//
//--------------+---------------+---------------+---------------+---------------//
//	SysModeBit	|	SysNtpWiFi	|	----------	|	SysRgbMode	|	SysSegMode	//
//				|	----------	|	SysSetTime	|	SysNumTime	|	SysNumTime	//
//--------------+---------------+---------------+---------------+---------------//
#define		NumTimeRead()				(	iSysModeBit & 0x03	)

#define		NumTimeWrite(iNumTime)		{	iSysModeBit = (iSysModeBit & 0xFC) |	\
											((iNumTime) & 0x03);	}
//------------------------------------------------------------------------------//
#define		SetTimeRead()				(	(iSysModeBit & 0x04) >> 2	)

#define		SetTimeLow()				{	iSysModeBit &= 0xFB;	}
#define		SetTimeHigh()				{	iSysModeBit |= 0x04;	}
//------------------------------------------------------------------------------//
#define		LedModeRead()				(	(iSysModeBit & 0x30) >> 4	)

#define		LedModeWrite(iLedMode)		{	iSysModeBit = (iSysModeBit & 0xCF) |	\
										(((iLedMode) & 0x03) << 4);	}
//------------------------------------------------------------------------------//
#define		SegModeRead()				(	(iSysModeBit & 0x10) >> 4	)

#define		SegModeLow()				{	iSysModeBit &= 0xEF;	}
#define		SegModeHigh()				{	iSysModeBit |= 0x10;	}
//------------------------------------------------------------------------------//
#define		RgbModeRead()				(	(iSysModeBit & 0x20) >> 5	)

#define		RgbModeLow()				{	iSysModeBit &= 0xDF;	}
#define		RgbModeHigh()				{	iSysModeBit |= 0x20;	}
//------------------------------------------------------------------------------//
#define		NtpWiFiRead()				(	(iSysModeBit & 0x80) >> 7	)

#define		NtpWiFiLow()				{	iSysModeBit &= 0x7F;	}
#define		NtpWiFiHigh()				{	iSysModeBit |= 0x80;	}
//==============================================================================//


//==============================================================================//
#endif
//------------------------------------------------------------------------------//
#ifdef		LowerDefinition
//==============================================================================//


//==============================================================================//
//	初回Wi-Fi接続時だけ設定
//	This setting is only required the first time you connect to Wi-Fi
//------------------------------------------------------------------------------//
//	#define	 	WiFiSSIDPSWD			"SSID", "PSWD"	//	Wi-Fi識別子・パスワード
//------------------------------------------------------------------------------//
//	【注意】技適マークが無い無線機器を日本国内で使用すると電波法違反になる場合があります
//	[Caution] Using wireless devices without a Technical Conformity Mark in Japan may be a violation of the Radio Law
//==============================================================================//


//==============================================================================//
static Uint08 iSysModeBit;								//	システム状態ビット
//==============================================================================//


//==============================================================================//
#endif
//==============================================================================//

