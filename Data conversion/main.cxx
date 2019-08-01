#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>

#include <TObject.h>
#include <TFile.h>
#include <TTree.h>

#include <vector>

#include "pk.h"
#include <Rtypes.h>
//SEvent SEV;


Bool_t is_corrupted(UShort_t *samples, UInt_t frame_size){
   for(UInt_t i = 0; i < frame_size; i++){
      if (samples[i] == 0)
         return true;
   }
   return false;
}

//----------------------------------------------------------------------
int main(int argc, char *argv[]) 
{
   UInt_t i,j;

   UInt_t rows,cols,frame_size;
   Char_t ifname[120],ofname[120],parname[120];
   UInt_t swap_bytes=0;

   int c,optcontrol=0;

   while ((c = getopt (argc, argv, "r:c:i:o:s")) != -1)
      switch(c) {
         case 'r' :
            sscanf(optarg,"%d",&rows);
            optcontrol|=1;
            break;
         case 'c' :
            sscanf(optarg,"%d",&cols);
            optcontrol|=2;
            break;
         case 'i' :
            strcpy(ifname,optarg);
            optcontrol|=4;
            break;	
         case 'o' :
            strcpy(ofname,optarg);
            optcontrol|=8;
            break;
         case 's' :
            swap_bytes=1;
            break;	
         case '?' :
         default:
            printf("enter -r<rows> -c<cols> -i<infilename> -o<outfilename [-s<swap_bytes>]\n");
            return(-1);
      }

   if(optcontrol != 0xF) {
      printf("enter -r<rows> -c<cols> -i<infilename> -o<outfilename [-s<swap_bytes>]\n");
      return(-1);
   }

   frame_size=rows*cols;
   UShort_t *samples= new UShort_t [frame_size];

   printf("ROWS: %d COLS: %d ifname: %s ofname: %s swap: %d frame_size: %d\n",rows,cols,ifname,ofname,swap_bytes,frame_size);

   FILE *f=fopen(ifname,"r");
   if(f==NULL) {
      printf("file not found\n");
      return(-1);
   }
   //########################################################
   Char_t parms[40];
   double vhv,vbpw,vsoi2;

   strcpy(parname,ifname); /* ifname: information file name */
   strcat(parname,"_params"); 
   FILE *fp=fopen(parname,"r");
   if(fp==NULL) {
      printf("params file not found\n");
      for(i=0;i<40;i++) parms[i]=0;
      vhv=vbpw=vsoi2=0.0;
   }else{
      int n=80,reg,val;	
      size_t *size=(size_t *)&n;	
      char *bu=(char*)malloc(120);
      char **lineptr=&bu;	
      char tA[80],tB[80],tC[80];
      while(getline(lineptr,size,fp)>0) {
         if(!(i=sscanf(*lineptr,"%s %s %s",tA,tB,tC))) continue;
         if(strstr(*lineptr,"-->")!=NULL) {
            sscanf(tA,"%d",&reg);
            sscanf(tC,"%x",&val);
            parms[(int)reg]=(Char_t)(val & 0xFF);
         }
         if(strstr(*lineptr,"vHV:")!=NULL) {
            sscanf(tB,"%lf",&vhv);
         }
         if(strstr(*lineptr,"vSOI2:")!=NULL) {
            sscanf(tB,"%lf",&vsoi2);
         }	
         if(strstr(*lineptr,"vBPW:")!=NULL) {
            sscanf(tB,"%lf",&vbpw);
         }		
      }
      fclose(fp);
      free(bu);
   }


   PKinfo *run_info = new PKinfo(rows,cols,frame_size,vhv,vbpw,vsoi2);
   run_info->info->Append("dupa Jas poszedl krasc");
   run_info->params->Set(32,parms);
   //########################################################

   TFile *pf=new TFile(ofname,"RECREATE");
   TTree *tr= new TTree("pktree","pomiarek_kapusty");
   tr->GetUserInfo()->Add(run_info);
   
   tr->Branch("frame_size",&frame_size,"frame_size/i"); //ja tak nie chce, ale root chce
   tr->Branch("event_data",samples,"samples[frame_size]/s");

   tr->SetAutoSave(1000); //1000 * 300MB, duzo
   
   for(i=0;;i++){

      if(fread(samples,2,frame_size,f)!=frame_size) break;
      if(swap_bytes) for(j=0;j<frame_size;j++) samples[j]=swap(samples[j]);

      ((PKinfo*)(tr->GetUserInfo()->At(0)))->number_of_frames=i+1;;
      if(!is_corrupted(samples,frame_size))
         tr->Fill();	
      if(!(i%1000)) { printf("~"); fflush(stdout);}
   };

   printf("\n%d events read, %d were not corrupted\n",i, tr->GetEntries());
   pf->Write(); 
   pf->Close();  
   fclose(f); 
   return 0;
}
