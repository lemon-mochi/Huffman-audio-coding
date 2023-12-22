# Huffman-audio-coding
Program that uses entropy coding to perform lossless compression on a stereo audio file. This program does not actually perform any lossless compression, meaning the size of the file will not change. This program only works on stereo audio files. I have only tested this on Ubuntu. I do not think it works on Windows. The GNUPlot-iostream library must be in the same directory for the code to compile.
gnuplot-iostream available at: https://github.com/dstahlke/gnuplot-iostream
To compile: g++ -o audio_huffman audio_huffman.cpp -I/usr/include/boost -L/gnuplot-iostream/gnuplotiostream -lboost_iostreams -lsndfile.
Valgrind reported no errors but Absence of errors is a fallacy according to the experts online.
Shell commands used (most of these were not used in the final version)
sudo apt-get install gnuplot
sudo apt-get install libboost-all-dev
sudo apt-get install libsndfile1
sudo apt install sndfile
sudo apt-get install libsndfile1-dev
sudo apt install qtbase5-dev
sudo apt install libgtk-3-dev
sudo apt-get install zenity
sudo apt-get install libsdl2-dev
git clone https://github.com/opencv/opencv.git
sudo apt-get install libsdl2-image-dev
sudo apt-get install libopencv-dev
https://docs.opencv.org/4.x/d7/d9f/tutorial_linux_install.html
Most of these shells commands are probably not needed but I did not keep track of which ones were actually used.
The program takes in a file input by opening the zenity file selector and extracting the file’s path so that the program knows which file to use. To calculate the entropy, the program finds all the distinct amplitudes that appear in the sample. It then counts the number of times that specific amplitude appears and uses this to calculate the probability. The program summates the probability + log2(probability) each time it finds a new amplitude. To calculate the average code word length, the program creates a binary tree where the leaves are the original amplitudes. I read www.geeksforgeeks.org/huffman-coding-greedy-algo-3/ for help on implementing the Huffman tree. The implementation mainly follows the tree example in the lecture slides. It creates the tree with a bottom-up approach where it looks at the two least likely amplitudes first and combines them. The program assigns the code values to the nodes of the tree by giving all elements in the left subtree an extra 0 and all elements in the right subtree and extra 1. Once a leaf node is reached, the length of the code is multiplied by the probability and is added to the average code length. 
