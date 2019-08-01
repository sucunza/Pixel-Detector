Cluster anslysis of data from the Silicon On Insulator detector


  This analysis was made during the project part of the Particle Physics Summer Student Program (PPSSP) at the Institute of Nuclear Sciences.
  
  
  The goal of this project is to analize the data obtained by a Silicon On Insulator detector, implement several algorithms to be able to utilize it, and measure some basic properties of the detector.
  
  CONTENT:
  
  The data conversion folder contains all the code used for the conversion of the data from a binary file into a root file. It contains an implemented algorithm to filter potentially corrupted data as well.
  
  The data analysis folder contains the code in which several algorithms to clean the data ara implemented. We calculate and remove the pedestal, the common mode and noise. The output is a root file with the complete information about each pixel.
  
  The clustering folder contains the code used to cluster the data. It is designed to be run as a ROOT macro. The output is a pdf with histograms about the clusters and a root file with the same information.
