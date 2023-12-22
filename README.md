# Huffman-audio-coding
Program that uses entropy coding to perform lossless compression on a stereo audio file. This program does not actually perform any lossless compression, meaning the size of the file will not change. This program only works on stereo audio files. I have only tested this on Ubuntu. I do not think it works on Windows. The GNUPlot-iostream library must be in the same directory for the code to compile.
To compile: g++ -o audio_huffman audio_huffman.cpp -I/usr/include/boost -L/gnuplot-iostream/gnuplotiostream -lboost_iostreams -lsndfile.
Valgrind reported no errors but Absence of errors is a fallacy according to the experts online.
