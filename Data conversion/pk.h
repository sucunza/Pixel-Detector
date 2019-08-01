#ifndef PKDEF
#define PKDEF

#include <TObject.h>
#include <TFile.h>
#include <TTree.h>
#include <TString.h>
#include <TArrayC.h>
#include <TArrayF.h>
#include <TArrayS.h>
#include <TH1.h>
#include <TH2.h>

#define INT7

#ifdef INT7
#define ROWS 1408
#define COLS 1408
#define ROW_START   640
#define ROW_STOP    1407
#define COL_START   128
#define COL_STOP    1407
#define ACQ_BY_ROWS kTRUE
#define INVERT kFALSE
#endif

#define FRAME_SIZE ROWS*COLS

class SEvent : public TObject {
   public:
      UInt_t frame_number;
      Short_t samples[FRAME_SIZE];
      SEvent() {return;};
      ClassDef(SEvent,1)
};

class PKinfo :public TObject {
   public:
      Double_t vhv;
      Double_t vbpw;
      Double_t vsoi2;
      UInt_t rows;
      UInt_t cols;
      UInt_t frame_size;
      UInt_t number_of_frames;
      TArrayC *params;
      TString *info;
      PKinfo(UInt_t r, UInt_t c, UInt_t frames,Double_t,Double_t,Double_t);
      PKinfo() {return;};
      ClassDef(PKinfo,1)
};

UShort_t swap( UShort_t val );
#endif
