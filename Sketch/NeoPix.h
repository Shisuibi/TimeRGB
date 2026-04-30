
//==============================================================================//
//	Portable NTP Clock Timer Mood Light "TimeRGB" I/O Sketch for ESP32-S3-Zero	//
//				Implemented by Shisuibi Å`Grand Master SorcerianÅ`				//
//==============================================================================//


//==============================================================================//
#ifdef		UpperDefinition
//==============================================================================//


//==============================================================================//
#define		NeoPixRGB(fColor)			((Uint08)(255.0 * (fColor)))	//	êFç ïœä∑
//==============================================================================//


//==============================================================================//
#endif
//------------------------------------------------------------------------------//
#ifdef		LowerDefinition
//==============================================================================//


//==============================================================================//
static void HsvToRgb(Sflt32* pHsv, Sflt32* pRgb) {
	Sint08 iH;
	Sflt32 fH = pHsv[X] * 6.0, fS = 1.0 - pHsv[Y];

	fH = fH - (Sflt32)(iH = (Sint08)fH);
	pRgb[X] = pRgb[Y] = pRgb[Z] = pHsv[Z];

	switch(iH) {
	case 0:		pRgb[Y] *= fS  + pHsv[Y] * fH;	pRgb[Z] *= fS;	break;
	case 1:		pRgb[X] *= 1.0 - pHsv[Y] * fH;	pRgb[Z] *= fS;	break;
	case 2:		pRgb[Z] *= fS  + pHsv[Y] * fH;	pRgb[X] *= fS;	break;
	case 3:		pRgb[Y] *= 1.0 - pHsv[Y] * fH;	pRgb[X] *= fS;	break;
	case 4:		pRgb[X] *= fS  + pHsv[Y] * fH;	pRgb[Y] *= fS;	break;
	case 5:		pRgb[Z] *= 1.0 - pHsv[Y] * fH;	pRgb[Y] *= fS;	break;
	}
}
//==============================================================================//


//==============================================================================//
static void NeoPixInit(void) {
	NeoPixWrite(0x00, 0x00, 0x00);
}
//------------------------------------------------------------------------------//
static void NeoPixMove(void) {
	Uint08 iSM = SegModeRead();
	Sflt32 afHsv[XYZ], afRgb[XYZ];

	if((SetTimeRead() != False)||(RgbModeRead() == False)) {
		NeoPixWrite(0x00, 0x00, 0x00);	return;
	}

	afHsv[X] = (Sflt32)((((Uint16)aiTimerSecond[iSM] * 100 + aiTimerCentis[iSM]) << iSM) % 6000) / 6000.0;
	afHsv[Y] = 1.0;
	afHsv[Z] = 0.75 + cos(6.0 * PI * afHsv[X]) * 0.25;

	HsvToRgb(afHsv, afRgb);
	NeoPixWrite(NeoPixRGB(afRgb[X]), NeoPixRGB(afRgb[Y]), NeoPixRGB(afRgb[Z]));
}
//==============================================================================//


//==============================================================================//
#endif
//==============================================================================//

