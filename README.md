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
