#define pixels_cxx
#include "pixels.h"
#include <TH2.h>
#include <TStyle.h>
#include <TH1D.h>
#include <TCanvas.h>

void percentage(UInt_t now, UInt_t max){
   for (long int i = 0; i <= 100; i++){
      if (i*max/100 == now){
         std::cout << i <<"%";
         fflush(stdout);
         std::cout << "  \r";
      }
   }
}
typedef struct{
   int id;
   bool isEdge;
   bool hit;
   bool hot;
   vector<int> neighbours;
   double signal;
}Pix;

bool is_neighbor(Pix pixel, Pix neigh){
   for(int i = 0; i < 8; i++){
      if(pixel.neighbours[i] == neigh.id)
         // std::cout <<  "Is neighbour" << std::endl;
         return true;
   }
   // std::cout <<  "Not neighbour" << std::endl;
   return false;
}

void pixels::Loop(){
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();


   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
   }
}

/*
 *                                Clustering:
 *   Clustering is done by creating a vector contaning only the IDs of the
 *   pixels which were hit. Then we select the first one (removing it from
 *     the original vector) and compare with the remaining hit pixels, if
 *     they are a neighbour of this first. if so, we add them to the same
 *    cluster, and remove it from the original vector. We do this until we
 *   get all the neighbour members of this initial pixel, and then move to
 *    the next pixel on the cluster: check its neighbours on the original
 *   vector... once we have no more neighbours, the cluster is closed, and
 *    we move to the remaining elements on the initial vector, until it is
 *                                   empty.
 */

void pixels::Cluster(){
  TFile *f = new TFile("clusters.root","RECREATE");

  double clustSpectra[21] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  TNtuple *clusterSpectra = new TNtuple("clusterSpectra","Spectrum","clustSpectra:clustSpectra_1:clustSpectra_2:clustSpectra_3:clustSpectra_4:clustSpectra_5:clustSpectra_6:clustSpectra_7:clustSpectra_8:clustSpectra_9:clustSpectra_10:clustSpectra_11:clustSpectra_12:clustSpectra_13:clustSpectra_14:clustSpectra_15:clustSpectra_16:clustSpectra_17:clustSpectra_18:clustSpectra_19:clustSpectra_20");
   Long64_t nentries = fChain->GetEntriesFast();
   std::cout << "Calculating clusters over " << nentries << " entries" << std::endl;
   for (Long64_t jentry = 10; jentry<nentries; jentry++) {
      Long64_t ientry = LoadTree(jentry);
      fChain->GetEntry(jentry);
      std::vector<Pix> hit_pixel;

      for(int i = 0; i < id->size();i++){
         if(hit->at(i) && signal->at(i)>0){
            Pix temp = {id->at(i), isEdge->at(i),hit->at(i),hot->at(i),neighbours->at(i),signal->at(i)};
            hit_pixel.push_back(temp);
         }
      }
      std::vector<std::vector<Pix>> clusters(hit_pixel.size(),std::vector<Pix>(1));

      int i = 0;
      int j = 0;
      int k = 0;
      while(hit_pixel.size() != 0){
         clusters[i][j] = hit_pixel[0];
         hit_pixel.erase(hit_pixel.begin());
         while(j < clusters[i].size()){
            while(k < hit_pixel.size()){
               if(is_neighbor(clusters[i][j],hit_pixel[k])){
                  clusters[i].push_back(hit_pixel[k]);
                  hit_pixel.erase(hit_pixel.begin() + k);
               }else{
                  k++;
               }
            }
            j++;
            k = 0;
         }
         i++;
         j = 0;
         k = 0;
      }

      double sumClusters;
      for(i = 0; i < clusters.size(); i++){
         sumClusters = 0;
         for(j = 0; j < clusters[i].size();j++){
            sumClusters += clusters[i][j].signal;
         }
         if(sumClusters != 0)
            clustSpectra[0] = (sumClusters);
         if(clusters[i].size() == 1 && sumClusters != 0)
            clustSpectra[1] = (sumClusters);
         if(clusters[i].size() == 2) clustSpectra[2] = (sumClusters);
         if(clusters[i].size() == 3) clustSpectra[3] = (sumClusters);
         if(clusters[i].size() == 4) clustSpectra[4] = (sumClusters);
         if(clusters[i].size() == 5) clustSpectra[5] = (sumClusters);
         if(clusters[i].size() == 6) clustSpectra[6] = (sumClusters);
         if(clusters[i].size() == 7) clustSpectra[7] = (sumClusters);
         if(clusters[i].size() == 8) clustSpectra[8] = (sumClusters);
         if(clusters[i].size() == 9) clustSpectra[9] = (sumClusters);
         if(clusters[i].size() == 10) clustSpectra[10] = (sumClusters);
         if(clusters[i].size() == 11) clustSpectra[11] = (sumClusters);
         if(clusters[i].size() == 12) clustSpectra[12] = (sumClusters);
         if(clusters[i].size() == 13) clustSpectra[13] = (sumClusters);
         if(clusters[i].size() == 14) clustSpectra[14] = (sumClusters);
         if(clusters[i].size() == 15) clustSpectra[15] = (sumClusters);
         if(clusters[i].size() == 16) clustSpectra[16] = (sumClusters);
         if(clusters[i].size() == 17) clustSpectra[17] = (sumClusters);
         if(clusters[i].size() == 18) clustSpectra[18] = (sumClusters);
         if(clusters[i].size() == 19) clustSpectra[19] = (sumClusters);
         if(clusters[i].size() == 20) clustSpectra[20] = (sumClusters);
         clusterSpectra->Fill((const float*)clustSpectra);
      }
      percentage(jentry,nentries);
   }



   clusterSpectra->Write();
}

/*
 *       Demonstration of the algorithm
 *              FGH
 *              BCD           E
 *               A
 *
 *
 *                Vi = BCDEFGH
 *                   C1 = A
 *         is Vi[0]=B neigh C1[0] = A
 *                     y
 *                Vi = CDEFGH
 *                  C1 = AB
 *         is Vi[0]=C neigh C1[0] = A
 *                     y
 *                 Vi = DEFGH
 *                  C1 = ABC
 *         is Vi[0]=D neigh C1[0] = A
 *                     y
 *                 Vi = EFGH
 *                 C1 = ABCD
 *         is Vi[0]=E neigh C1[0] = A
 *                     n
 *         is Vi[1]=F neigh C1[0] = A
 *                     n
 *         is Vi[2]=G neigh C1[0] = A
 *                     n
 *         is Vi[3]=H neigh C1[0] = A
 *                     n
 *         is Vi[0]=E neigh C1[1] = B
 *                     n
 *         is Vi[1]=F neigh C1[1] = B
 *                     y
 *                  Vi = EGH
 *                 C1 = ABCDF
 *         is Vi[1]=G neigh C1[1] = B
 *                     y
 *                  Vi = EH
 *                C1 = ABCDFG
 *         is Vi[1]=H neigh C1[1] = B
 *                     n
 *         is Vi[0]=E neigh C1[2] = C
 *                     n
 *         is Vi[1]=H neigh C1[2] = C
 *                     y
 *                   Vi = E
 *                C1 = ABCDFGH
 *
 *     Now the index of C1 is larger than
 *     the size of Vi, so C1 is done, C2
 *       comes next, with the remaining
 *               elements of Vi
 *
 *                   Vi = E
 *                   C2 = .
 *                   Vi = .
 *                   C2 = E
 *
 *         C2 done. All clusters done
 *
 */
