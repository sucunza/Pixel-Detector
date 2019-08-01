/*---------------------------------------------------------------------------------*/
/*   This code reads the .root file provided end outputs another .root file with   */
/*    the common mode and pedestal removed from the data. It also provides more    */
/*  information of each individual pixel, which can be used for further analysis   */
/*---------------------------------------------------------------------------------*/

#include <TH2.h>
#include <TH2I.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TH1.h>
#include <TH1D.h>

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <iostream>
#include <vector>
#include <chrono>


#include "pk.h"
using namespace std::chrono;


static struct option long_options[] =
{
   {"ri", required_argument,  0, 'a'},
   {"rf", required_argument,  0, 'b'},
   {"ci", required_argument,  0, 'c'},
   {"cf", required_argument,  0, 'd'},
   {"i",  required_argument,  0, 'i'},
   {"o",  required_argument,  0, 'o'},
   {"inv", no_argument,       0, 'n'},
   {"cm",  required_argument, 0, 'r'},

   {0, 0, 0, 0}
};
std::vector<Int_t> get_neighbours(Int_t pos, Int_t rows, Int_t cols, Int_t row, Int_t col){
   std::vector<Int_t> neighbours(8);
   neighbours[0]=pos+cols-1;
   neighbours[1]=pos+cols;
   neighbours[2]=pos+cols+1;
   neighbours[3]=pos-1;
   neighbours[4]=pos+1;
   neighbours[5]=pos-cols-1;
   neighbours[6]=pos-cols;
   neighbours[7]=pos-cols+1;

   if(row == 0){
      neighbours[5]=-1;
      neighbours[6]=-1;
      neighbours[7]=-1;
   }
   if(col == 0){
      neighbours[0]=-1;
      neighbours[3]=-1;
      neighbours[5]=-1;
   }
   if(row == rows-1){
      neighbours[0]=-1;
      neighbours[1]=-1;
      neighbours[2]=-1;
   }
   if(col == cols-1){
      neighbours[2]=-1;
      neighbours[4]=-1;
      neighbours[7]=-1;
   }
   return neighbours;
}
//This function is used to display the progress percentage
void percentage(UInt_t now, UInt_t max){
   for (long int i = 0; i <= 100; i++){
      if (i*max/100 == now){
         std::cout << i <<"%";
         fflush(stdout);
         std::cout << "  \r";
      }
   }
}//ending of percentage

int main(int argc, char *argv[])
{
   Int_t i,rows,cols,frame_size,total_frames,invert=0;
   Int_t row_start=0,row_stop,col_start=0,col_stop;
   UInt_t fsize;
   Char_t ifname[120],ofname[120];

   int c,optcontrol=0;
   ofname[0]='\0';

   /* ------------------------------------------------------------------------------ */
   /*               retrieving the data from the input parameters                    */
   /* ------------------------------------------------------------------------------ */
   while (1)
   {
      int opt_index = 0;
      if((c = getopt_long_only (argc, argv, "",long_options, &opt_index))==-1) break;
      switch (c)
      {
         case 'a':
            sscanf(optarg,"%d",&row_start);
            break;
         case 'b':
            sscanf(optarg,"%d",&row_stop);
            optcontrol|=1;
            break;
         case 'c':
            sscanf(optarg,"%d",&col_start);
            break;
         case 'd':
            sscanf(optarg,"%d",&col_stop);
            optcontrol|=2;
            break;
         case 'i' :
            strcpy(ifname,optarg);
            optcontrol|=4;
            break;
         case 'o' :
            strcpy(ofname,optarg);
            break;
         case 'n':
            invert=1;
            break;
         case 'r':
            break;
         case '?':
         default:
            printf("? enter: -i<in_file> -ri<row_start> -rf<row_stop> -ci<col_start> -cf<col_stop> [-inv]\n");
            return(-1);
      }
   }

   if(optcontrol!=0x07) {
      printf("? enter: -i<in_file> -ri<row_start> -rf<row_stop> -ci<col_start> -cf<col_stop> [-inv]\n");
      return(-1);
   }
   printf("rstart: %d rstop: %d cstart: %d cstop: %d infile: %s invert: %d\n",row_start,row_stop,col_start,col_stop,ifname,invert);

   if(!ofname[0]) strcpy(ofname,ifname);
   strcat(ofname,"_histos.root");
   printf("%s\n",ofname);

   TFile *pf=new TFile(ifname);
   if (pf->IsZombie()) {
      printf("Error opening file\n");
      exit(-1);
   }

   TTree *tr= (TTree*)pf->Get("pktree");
   PKinfo *run_info=(PKinfo*)(tr->GetUserInfo()->At(0));

   // total_frames=(Int_t)run_info->number_of_frames;
   total_frames = (Int_t) tr->GetEntriesFast();
   rows=(Int_t)run_info->rows;
   cols=(Int_t)run_info->cols;
   frame_size=(Int_t)run_info->frame_size;
   //total_frames=1000;
   Int_t print_pix = 1050; // pixel which will be printed


   UShort_t *samples= new UShort_t [frame_size];

   printf("info: %d %d %d %d %s\n",rows,cols,frame_size,total_frames,run_info->info->Data());
   printf("info params: %x %x\n",run_info->params->At(30),run_info->params->At(31));

   tr->SetBranchAddress("frame_size",&fsize);
   tr->SetBranchAddress("event_data",samples);

   /* ------------------------------------------------------------------------- */
   /*                      SETTING UP PLOTS AND HISTOGRAMS                      */
   /* ------------------------------------------------------------------------- */

   TFile f("HISTOS.root","RECREATE");
   TCanvas canvas("canvas","canv",2000,800);

   // Creating the histograms needed

   // Hits on a single frame, on a 2D matrix
   TH2I *h_2D_hits = new TH2I("h_2D_hits","Hits on a single frame, on a 2D matrix;x;y",rows,row_start,row_stop,cols,col_start,col_stop);
   // Noise of each pixel, on a 2D matrix
   TH2I *h_2D_noise = new TH2I("h_2D_noise","Noise of each pixel, on a 2D matrix;x;y",rows,row_start,row_stop,cols,col_start,col_stop);
   // Noise distribution, over all pixels, over all frames (PKP)
   TH1I *h_1D_noise = new TH1I("h_1D_noise","Noise distribution, over all pixels, over all frames",400,-30,30);
   // Signal distribution, over all pixels, over all frames
   TH1I *h_1D_signal = new TH1I("h_1D_signal","Signal distribution, over all pixels, over all frames, removing common mode",400,-30,30);
   // Signal distribution, over all pixels, over all frames, without common mode (PKP)
   TH1I *h_1D_signalNoCM = new TH1I("h_1D_signalNoCM","Signal distribution, over all pixels, over all frames, without removing common mode",400,-30,30);
   // Common mode distribution of every frame
   TH1I *h_1D_commonMode = new TH1I("h_1D_commonMode","Common mode distribution of every frame;x;y",100,-30,30);
   // Filtered signal of a single frame, on a 2D matrix
   TH2I *h_2D_filteredSignal = new TH2I("h_2D_filteredSignal","Filtered signal of a single frame, on a 2D matrix;x;y",rows,row_start,row_stop,cols,col_start,col_stop);
   // Signal ditribution on a single frame
   TH1I *h_1D_filteredSignal = new TH1I("h_1D_filteredSignal","Signal ditribution on a single frame;energy (ADC);y",400,-30,30);
   // pedestal of every pixel, on a 2D matrix
   TH2I *h_2D_pedestal = new TH2I("h_2D_pedestal","pedestal of every pixel, on a 2D matrix;x;y",rows,row_start,row_stop,cols,col_start,col_stop);
   // pedestal of every pixel, on a 1D spectra
   TH1I *h_1D_pedestal = new TH1I("h_1D_pedestal","pedestal of every pixel, on a 1D spectra;energy (ADC)",1000,0,2500);


   //Initializing variables that represent the position of the pixel
   Int_t x_hit = 0, y_hit = 0;

   // 9960 -> after this value all frames are empty.
   // 8483 -> after this value the noise goes crazy. There are more empty frames on this region also.
   // total_frames = 8483; //total_frames; // used when there are some empty frames on the end


   std::vector<Double_t> signal_vector(frame_size,0);


   std::vector <Double_t> pedestal_vector(frame_size,0);
   std::vector <Double_t> noise_vector(frame_size,0);
   std::vector <std::vector<Double_t> > average(total_frames,std::vector<Double_t> (rows)); // common mode

   Int_t l;
   Int_t k;

   /*                       signal_vector(frame,pixel)                        */
   /* This array will hold all the information of the measurement. The first  */
   /*       component runs though every frame, and the second one runs        */
   /*                          through every pixel                            */

   std::cout << "Calculating the common mode and pedestal..." << std::endl;
   std::cout << "hellikdsjcsdkbckdsjb" << '\n';

   /* ------------------------------------------------------------------------- */
   /*                                  PEDESTAL                                 */
   /*         The pedestal is calculated for each pixel, as the average         */
   /*                       of its signal over all frames                       */
   /* ------------------------------------------------------------------------- */
   /* we loop through events */
   for(i=0;i<total_frames;i++) {
      tr->GetEntry(i); // get a frame

      /* Scaning the detector */
      for(Int_t p = 0; p < frame_size; p++){
         x_hit = p%cols; // X position
         y_hit = p/cols; // Y position
         pedestal_vector[p] += samples[p];
         h_2D_hits->SetBinContent(x_hit + 1, y_hit + 1, samples[p]); // Set the x,y bin to its value
      }

      percentage(i,total_frames);
   }

   for(Int_t p = 0; p < frame_size; p++){
      pedestal_vector[p] = pedestal_vector[p]/total_frames;
      x_hit = p%cols;
      y_hit = p/cols;
      h_2D_pedestal->SetBinContent(x_hit + 1, y_hit + 1,pedestal_vector[p]);
      h_1D_pedestal->Fill(pedestal_vector[p]);
   }

   /* ------------------------------------------------------------------------- */
   /*                                COMMON MODE                                */
   /*   Common mode is obtained for each frame, by calculating the average of   */
   /*      every pixel on that frame. It is the first "noise" to be removed     */
   /* ------------------------------------------------------------------------- */
   for(i=0;i<total_frames;i++) {

     l=0;
     k=0;
      tr->GetEntry(i); // get a frame
      for (Int_t p = 0; p < frame_size; p++) {
         //signal_vector[i][p] = samples[p] - pedestal_vector[p];
         average[i][l] += samples[p]-pedestal_vector[p];
         //average[i] += signal_vector[i][p];
         if(p==(rows-1)+k*rows){l+=1;k+=1;}
      }
      for (Int_t l = 0; l < rows; l++) {
        average[i][l] = average[i][l]/rows;
        h_1D_commonMode->Fill(average[i][l]);
      }
      k=0;
      l=0;
      for (Int_t p = 0; p < frame_size; p++){
         //signal_vector[i][p] -= average[i];
         h_1D_signalNoCM ->Fill(samples[p]-pedestal_vector[p]-average[i][l]); //PKP
        if(p==(rows-1)+k*rows){l+=1;k+=1;}
      }

      percentage(i,total_frames);
   }

   /* ------------------------------------------------------------------------- */
   /*                                   NOISE                                   */
   /*      The noise is calculated as the square of the standard deviation      */
   /*         assuming that the mean is zero, hence it is only the mean         */
   /*                            square of the signal                           */
   /* ------------------------------------------------------------------------- */
   std::cout<< "calculating remaining noise level" << std::endl;

   for(i=0;i<total_frames;i++) {
      tr->GetEntry(i); // get a frame
      l=0;
      k=0;
      /* Scaning the detector */
      for(Int_t p = 0; p < frame_size; p++){
        signal_vector[p]=samples[p]-pedestal_vector[p]-average[i][l];
        noise_vector[p] += (signal_vector[p]*signal_vector[p]); //PKP
       if(p==(rows-1)+k*rows){l+=1;k+=1;}
      }
      percentage(i,total_frames);
   }
   for(Int_t p = 0; p < frame_size; p++){
      noise_vector[p] = noise_vector[p]/total_frames;
      x_hit = p%cols;
      y_hit = p/cols;
      h_2D_noise->SetBinContent(x_hit + 1, y_hit + 1,noise_vector[p]);
      h_1D_noise->Fill(noise_vector[p]); //PKP

   }

   std::cout<< "common mode and pedestal calculated!" << std::endl;
   /* ------------------------------------------------------------------------- */
   /*                         cleaning the signal                               */
   /*       The pedestal and the common ground are removed from the             */
   /*     original signal. The result is a clean signal, which should           */
   /*         contain a few peaks, if a radiation source was used               */
   /* ------------------------------------------------------------------------- */



   /* Creating the TTree which will store all the information */
   std::cout << "here" << '\n';
   TTree *n_tr = new TTree("pixels","pixels");
   std::vector< Int_t > id(frame_size);
   std::vector< Bool_t > isEdge(frame_size);
   std::vector< std::vector< Int_t > > neighbours(frame_size);
   std::vector< Int_t > n_rows(frame_size);
   std::vector< Int_t > n_cols(frame_size);
   std::vector< Int_t > n_total_frames(frame_size);
   std::vector< Bool_t > hit(frame_size);
   std::vector< Bool_t > hot(frame_size);
   std::vector< Double_t > raw_data(frame_size);
   std::vector< Double_t > common_mode(frame_size);
   std::vector< Double_t > pedestal(frame_size);
   std::vector< Double_t > signal(frame_size);
   std::vector< Double_t > noise(frame_size);
   std::vector< Int_t > x_pos(frame_size);
   std::vector< Int_t > y_pos(frame_size);

   n_tr->Branch("id",&id);
   n_tr->Branch("isEdge",&isEdge);
   n_tr->Branch("neighbours",&neighbours);
   //n_tr->Branch("rows",&n_rows);
  // n_tr->Branch("cols",&n_cols);
   n_tr->Branch("hit",&hit);
   n_tr->Branch("hot",&hot);
   //n_tr->Branch("raw_data",&raw_data);
   //n_tr->Branch("common_mode",&common_mode);
   //n_tr->Branch("pedestal",&pedestal);
   n_tr->Branch("signal",&signal);
  // n_tr->Branch("noise",&noise);
   //n_tr->Branch("x_pos",&x_pos);
   //n_tr->Branch("y_pos",&y_pos);

   std::cout << "Looping through " << total_frames << " frames" << std::endl;
   auto t1 = high_resolution_clock::now();

   for(i=0; i < total_frames; i++) {
      tr->GetEntry(i);
      l=0;
      k=0;
      /* Scanning the detector */
      for(Int_t p = 0; p < frame_size; p++){
        signal_vector[p]=samples[p]-pedestal_vector[p]-average[i][l];
         h_1D_signal->Fill(signal_vector[p]); //PKP
         x_hit = p%cols; /* X position */
         y_hit = p/cols; /* Y position */
        if(p==(rows-1)+k*rows){l+=1;k+=1;}
         if(i == print_pix){
            h_2D_filteredSignal->SetBinContent(x_hit +1, y_hit +1, signal_vector[p]);
            h_1D_filteredSignal->Fill(signal_vector[p]);
         }

         id[p] = p;
         isEdge[p] = (x_hit == 0 || x_hit == cols-1 || y_hit == 0 || y_hit == rows-1);

         hot[p] = (noise_vector[p] > 8.5*h_1D_noise->GetMean());
         hit[p] = (!hot[p] && signal_vector[p]*signal_vector[p] > 9*noise_vector[p]);

         if (hit[p]) {neighbours[p] = get_neighbours(p,rows,cols,y_pos[p],x_pos[p]);}

         signal[p] = signal_vector[p];

         //raw_data[p] = samples[p];
         //common_mode[p] = average[i];
         //pedestal[p] = pedestal_vector[p];
         //noise[p] = noise_vector[p];

      }
      n_tr->Fill();
      percentage(i,total_frames);
   }
   auto t2 = high_resolution_clock::now();
   auto duration = duration_cast<microseconds>( t2 - t1 ).count();
   std::cout << "It has elapsed: " << duration/1000000 <<"seconds, or: "<< duration/60000000<<"minutes"<<'\n';

   /* Plotting stuff on PDF*/
   canvas.SaveAs("hit_map.pdf[");
   canvas.cd();

   // h_2D_hits
   h_2D_hits->SetStats(0);
   h_2D_hits->Draw("colz");
   canvas.SaveAs("hit_map.pdf");
   h_2D_hits->Write();

   // h_2D_pedestal
   canvas.Clear();
   h_2D_pedestal->SetStats(0);
   h_2D_pedestal->Draw("colz");
   canvas.SaveAs("hit_map.pdf");
   h_2D_pedestal->Write(); //PKP

   // h_2D_noise
   canvas.Clear();
   h_2D_noise->SetStats(0);
   h_2D_noise->Draw("colz");
   canvas.SaveAs("hit_map.pdf");
   h_2D_noise->Write();

   // h_2D_filteredSignal
   canvas.Clear();
   h_2D_filteredSignal->SetStats(0);
   h_2D_filteredSignal->Draw("colz");
   canvas.SaveAs("hit_map.pdf");
   h_2D_filteredSignal->Write();

   // h_1D_pedestal
   canvas.cd();
   h_1D_pedestal->Draw();
   canvas.SaveAs("hit_map.pdf");
   h_1D_pedestal->Write();

   // h_1D_commonMode
   canvas.Clear();
   h_1D_commonMode->Draw();
   canvas.SaveAs("hit_map.pdf");
   h_1D_commonMode->Write();

   // h_1D_signal
   canvas.Clear();
   h_1D_signal->Draw();
   canvas.SaveAs("hit_map.pdf");
   h_1D_signal->Write();

   // h_1D_signalNoCM
   canvas.Clear();
   h_1D_signalNoCM->Draw();
   canvas.SaveAs("hit_map.pdf");
   h_1D_signalNoCM->Write();

   // h_1D_noise
   canvas.Clear();
   h_1D_noise->Draw();
   canvas.SaveAs("hit_map.pdf");
   h_1D_noise->Write();

   // h_1D_filteredSignal
   canvas.Clear();
   h_1D_filteredSignal->Draw();
   canvas.SaveAs("hit_map.pdf");
   h_1D_filteredSignal->Write();


   canvas.SaveAs("hit_map.pdf]");

   f.Write();
   f.Close();
   printf("\n");
   pf->Close();
   return 0;
}
