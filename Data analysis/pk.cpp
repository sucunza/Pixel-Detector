void Pixel::set_position() {
   x_pos = id%cols;
   y_pos = id/cols;
}
void Pixel::set_hit(Int_t signal, Int_t threshold){
   hit = signal >= threshold;
}
void Pixel::set_hot(Double_t noise, Double_t threshold){
   hot = noise >= threshold;
   if (hot==true) hit=false;
}
void Pixel::set_neighbours(Int_t pos, Int_t cols) {
   Int_t row = y_pos;
   Int_t col = x_pos;
   if (row==0 && col==0) {                     // Bottom left corner
      isedge=true;
      neighbours[0]=-1;
      neighbours[1]=pos+cols;
      neighbours[2]=pos+cols+1;
      neighbours[3]=-1;
      neighbours[4]=pos+1;
      neighbours[5]=-1;
      neighbours[6]=-1;
      neighbours[7]=-1;
   }else if (row==0 && col==cols-1) {          // Bottom right corner
      isedge=true;
      neighbours[0]=pos+cols-1;
      neighbours[1]=pos+cols;
      neighbours[2]=-1;
      neighbours[3]=pos-1;
      neighbours[4]=-1;
      neighbours[5]=-1;
      neighbours[6]=-1;
      neighbours[7]=-1;
   }else if (row==rows-1 && cols ==0) {        // Top left corner
      isedge=true;
      neighbours[0]=-1;
      neighbours[1]=-1;
      neighbours[2]=-1;
      neighbours[3]=-1;
      neighbours[4]=pos + 1;
      neighbours[5]=-1;
      neighbours[6]=pos-cols;
      neighbours[7]=pos-cols +1;
   }else if (row==rows-1 && col == cols - 1) { // Top right corner
      isedge=true;
      neighbours[0]=-1;
      neighbours[1]=-1;
      neighbours[2]=-1;
      neighbours[3]=pos-1;
      neighbours[4]=-1;
      neighbours[5]=pos-cols-1;
      neighbours[6]=pos-cols;
      neighbours[7]=-1;
   }else if (row==0) {                         // Bottom edge
      isedge=true;
      neighbours[0]=pos+cols-1;
      neighbours[1]=pos+cols;
      neighbours[2]=pos+cols+1;
      neighbours[3]=pos-1;
      neighbours[4]=pos+1;
      neighbours[5]=-1;
      neighbours[6]=-1;
      neighbours[7]=-1;
   }else if (row==rows-1) {                     // top
      isedge=true;
      neighbours[0]=-1;
      neighbours[1]=-1;
      neighbours[2]=-1;
      neighbours[3]=pos-1;
      neighbours[4]=pos+1;
      neighbours[5]=pos-cols-1;
      neighbours[6]=pos-cols;
      neighbours[7]=pos-cols+1;
   }else if (col==0) {                         // left
      isedge=true;
      neighbours[0]=-1;
      neighbours[1]=pos+cols;
      neighbours[2]=pos+cols+1;
      neighbours[3]=-1;
      neighbours[4]=pos+1;
      neighbours[5]=-1;
      neighbours[6]=pos-cols;
      neighbours[7]=pos-cols+1;
   }else if (col==cols-1) {                    // right
      isedge=true;
      neighbours[0]=pos+cols-1;
      neighbours[1]=pos+cols;
      neighbours[2]=-1;
      neighbours[3]=pos-1;
      neighbours[4]=-1;
      neighbours[5]=pos-cols-1;
      neighbours[6]=pos-cols;
      neighbours[7]=-1;
   }else {                                     // a middle Pixel
      isedge=false;
      neighbours[0]=pos+cols-1;
      neighbours[1]=pos+cols;
      neighbours[2]=pos+cols+1;
      neighbours[3]=pos-1;
      neighbours[4]=pos+1;
      neighbours[5]=pos-cols-1;
      neighbours[6]=pos-cols;
      neighbours[7]=pos-cols+1;
   }
}
