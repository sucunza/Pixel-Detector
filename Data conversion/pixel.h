#ifndef _PIXEL_
#define _PIXEL_

#include <Rtypes.h>
class pixel{
   private:
      //defining the frame, and the position in each frame
      Int_t id;
      Int_t row;
      Int_t col;
      Double_t raw_data;  /*Data as obtained from the detector*/

   public:
      /*Boolean value that states if the pixel is located in a edge of
        the observed part of the detector*/
      Bool_t isedge;

      /*This vector is used to store the id of the neigbouring pixels.The first
        component is the pixel located on the top and to the left, and it continues
        from left to rightand top to bottom. It is important to note that in the case
        of borf¡der pixels, the components of the vector that would be on positions
        outside the detector are left empty.*/

      std::vector<Int_t> neighbours;
      Int_t rows;/*Total number of rows in the frames */
      Int_t cols;/*Total number of columns in the frames */
      Int_t total_frames;

      Bool_t hit;
      Bool_t hot;//constant noise

      Double_t common_mode;/*Value of the calculated common mode*/
      Double_t pedestal;  /*Value of the calculated pedestal*/
      Double_t signal;    /*Value of the final signal after substracting cm & pedestal*/

      pixel (Int_t ID, Int_t signal);
      virtual ~pixel ();
      void position(Int_t pos);
      void set_hit(Int_t signal, Int_t threshold);
      void set_hot(Int_t pedestal, Int_t threshold);
      void set_neighbours(Int_t pos); 


};



#endif
