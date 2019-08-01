#include "pk.h"

UShort_t swap( UShort_t val ) 
{
return (val << 8) | ((val >> 8) & 0xFF);
}

//-------------------------------------

PKinfo::PKinfo(UInt_t r, UInt_t c, UInt_t frame_s,Double_t VHV,Double_t VBPW, Double_t VSOI2) {
		vhv=VHV;
		vbpw=VBPW;
		vsoi2=VSOI2;
		rows=r;
		cols=c;
		frame_size=frame_s;
		number_of_frames=0;
		info=new TString(80);
		params=new TArrayC(40);
		
	}
	
	
