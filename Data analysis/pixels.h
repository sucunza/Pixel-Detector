//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Jul 24 12:38:47 2019 by ROOT version 6.16/00
// from TTree pixels/pixels
// found on file: HISTOS.root
//////////////////////////////////////////////////////////

#ifndef pixels_h
#define pixels_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "vector"
#include "vector"
#include "vector"

class pixels {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   std::vector<int>     *id;
   std::vector<bool>    *isEdge;
   std::vector<std::vector<Int_t> > *neighbours;
   std::vector<int>     *rows;
   std::vector<int>     *cols;
   std::vector<int>     *total_frames;
   std::vector<bool>    *hit;
   std::vector<bool>    *hot;
   std::vector<double>  *raw_data;
   std::vector<double>  *common_mode;
   std::vector<double>  *pedestal;
   std::vector<double>  *signal;
   std::vector<double>  *noise;
   std::vector<int>     *x_pos;
   std::vector<int>     *y_pos;

   // List of branches
   TBranch        *b_id;   //!
   TBranch        *b_isEdge;   //!
   TBranch        *b_neighbours;   //!
   TBranch        *b_rows;   //!
   TBranch        *b_cols;   //!
   TBranch        *b_total_frames;   //!
   TBranch        *b_hit;   //!
   TBranch        *b_hot;   //!
   TBranch        *b_raw_data;   //!
   TBranch        *b_common_mode;   //!
   TBranch        *b_pedestal;   //!
   TBranch        *b_signal;   //!
   TBranch        *b_noise;   //!
   TBranch        *b_x_pos;   //!
   TBranch        *b_y_pos;   //!

   pixels(TTree *tree=0);
   virtual ~pixels();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual void     Cluster();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef pixels_cxx
pixels::pixels(TTree *tree) : fChain(0)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("HISTOS.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("HISTOS.root");
      }
      f->GetObject("pixels",tree);

   }
   Init(tree);
}

pixels::~pixels()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t pixels::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t pixels::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void pixels::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   id = 0;
   isEdge = 0;
   neighbours = 0;
   rows = 0;
   cols = 0;
   total_frames = 0;
   hit = 0;
   hot = 0;
   raw_data = 0;
   common_mode = 0;
   pedestal = 0;
   signal = 0;
   noise = 0;
   x_pos = 0;
   y_pos = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("id", &id, &b_id);
   fChain->SetBranchAddress("isEdge", &isEdge, &b_isEdge);
   fChain->SetBranchAddress("neighbours", &neighbours, &b_neighbours);
   fChain->SetBranchAddress("rows", &rows, &b_rows);
   fChain->SetBranchAddress("cols", &cols, &b_cols);
   fChain->SetBranchAddress("total_frames", &total_frames, &b_total_frames);
   fChain->SetBranchAddress("hit", &hit, &b_hit);
   fChain->SetBranchAddress("hot", &hot, &b_hot);
   fChain->SetBranchAddress("raw_data", &raw_data, &b_raw_data);
   fChain->SetBranchAddress("common_mode", &common_mode, &b_common_mode);
   fChain->SetBranchAddress("pedestal", &pedestal, &b_pedestal);
   fChain->SetBranchAddress("signal", &signal, &b_signal);
   fChain->SetBranchAddress("noise", &noise, &b_noise);
   fChain->SetBranchAddress("x_pos", &x_pos, &b_x_pos);
   fChain->SetBranchAddress("y_pos", &y_pos, &b_y_pos);
   Notify();
}

Bool_t pixels::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void pixels::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t pixels::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef pixels_cxx
