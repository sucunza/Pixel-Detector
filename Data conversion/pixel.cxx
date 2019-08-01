#include <vector>
#include <math.h>

#include "pixel.h"
#include <Rtypes.h>
/* CONSTRUCTOR */
pixel::pixel (Int_t ID, Int_t signal){
   position(ID);
   set_neighbours(ID);
}


/* calculates the row and column the pixel is located based on its ID */
void pixel::position(Int_t pos) {
   row = pos%cols;
   col = pos/cols;
}

pixel::~pixel(){}

/* determines if the pixel has been hit provided a certain threshold */
void pixel::set_hit(Int_t signal, Int_t threshold){
   hit = signal >= threshold;
};

/* determines if the pixel has a constant noise, provided a threshold */
// the mean noise should be the pedestal
void pixel::set_hot(Int_t pedestal, Int_t threshold){
   hot = pedestal >= threshold;
}


/* determines the neighbourhood of the pixel */
void pixel::set_neighbours(Int_t pos) {
   if (row==0 && col==0) {                     // Bottom left corner
      isedge=true;
      neighbours[1]=-1;
      neighbours[2]=pos+cols;
      neighbours[3]=pos+cols+1;
      neighbours[4]=-1;
      neighbours[5]=pos+1;
      neighbours[6]=-1;
      neighbours[7]=-1;
      neighbours[8]=-1;
   }else if (row==0 && col==cols-1) {          // Bottom right corner
      isedge=true;
      neighbours[1]=pos+cols-1;
      neighbours[2]=pos+cols;
      neighbours[3]=-1;
      neighbours[4]=pos-1;
      neighbours[5]=-1;
      neighbours[6]=-1;
      neighbours[7]=-1;
      neighbours[8]=-1;
   }else if (row==rows-1 && cols ==0) {        // Top left corner
      isedge=true;
      neighbours[1]=-1;
      neighbours[2]=-1;
      neighbours[3]=-1;
      neighbours[4]=-1;
      neighbours[5]=pos + 1;
      neighbours[6]=-1;
      neighbours[7]=pos-cols;
      neighbours[8]=pos-cols +1;
   }else if (row==rows-1 && col == cols - 1) { // Top right corner
      isedge=true;
      neighbours[1]=-1;
      neighbours[2]=-1;
      neighbours[3]=-1;
      neighbours[4]=pos-1;
      neighbours[5]=-1;
      neighbours[6]=pos-cols-1;
      neighbours[7]=pos-cols;
      neighbours[8]=-1;
   }else if (row==0) {                         // Bottom edge
      isedge=true;
      neighbours[1]=pos+cols-1;
      neighbours[2]=pos+cols;
      neighbours[3]=pos+cols+1;
      neighbours[4]=pos-1;
      neighbours[5]=pos+1;
      neighbours[6]=-1;
      neighbours[7]=-1;
      neighbours[8]=-1;
   }else if (row==rows-1) {                     // top
      isedge=true;
      neighbours[1]=-1;
      neighbours[2]=-1;
      neighbours[3]=-1;
      neighbours[4]=pos-1;
      neighbours[5]=pos+1;
      neighbours[6]=pos-cols-1;
      neighbours[7]=pos-cols;
      neighbours[8]=pos-cols+1;
   }else if (col==0) {                         // left
      isedge=true;
      neighbours[1]=-1;
      neighbours[2]=pos+cols;
      neighbours[3]=pos+cols+1;
      neighbours[4]=-1;
      neighbours[5]=pos+1;
      neighbours[6]=-1;
      neighbours[7]=pos-cols;
      neighbours[8]=pos-cols+1;
   }else if (col==cols-1) {                    // right
      isedge=true;
      neighbours[1]=pos+cols-1;
      neighbours[2]=pos+cols;
      neighbours[3]=-1;
      neighbours[4]=pos-1;
      neighbours[5]=-1;
      neighbours[6]=pos-cols-1;
      neighbours[7]=pos-cols;
      neighbours[8]=-1;
   }else {                                     // a middle pixel
      isedge=false;
      neighbours[1]=pos+cols-1;
      neighbours[2]=pos+cols;
      neighbours[3]=pos+cols+1;
      neighbours[4]=pos-1;
      neighbours[5]=pos+1;
      neighbours[6]=pos-cols-1;
      neighbours[7]=pos-cols;
      neighbours[8]=pos-cols+1;
   }
}

