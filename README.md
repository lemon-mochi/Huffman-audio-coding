# Huffman Audio Coding

This program demonstrates entropy coding to simulate lossless compression on a stereo audio file using Huffman coding. Please note that this version **does not actually perform compression**, meaning the file size remains unchanged. It only works with stereo audio files and has been tested **exclusively on Ubuntu**.

## Dependencies
- **GNUPlot-iostream library** must be present in the same directory to compile the code.
  - Available at: [gnuplot-iostream](https://github.com/dstahlke/gnuplot-iostream)
- **Boost libraries** and **libsndfile** are required for handling input/output streams and audio processing.

## Installation
To set up the required environment, run the following commands (some dependencies may already be installed on your system):

```
sudo apt-get install gnuplot
sudo apt-get install libboost-all-dev
sudo apt-get install libsndfile1 libsndfile1-dev
sudo apt install sndfile
```

### Optional Dependencies:
- **Zenity:** for graphical file selection.
- **SDL2** and **OpenCV:** may be included for future extensions or testing purposes but aren't critical for this version.

```
sudo apt-get install zenity
sudo apt-get install libsdl2-dev libsdl2-image-dev
sudo apt-get install libopencv-dev
```

## Compilation
To compile the program, use the following command:
```
g++ -o audio_huffman audio_huffman.cpp -I/usr/include/boost -L/gnuplot-iostream/gnuplotiostream -lboost_iostreams -lsndfile
```

Ensure you have the **GNUPlot-iostream** and **Boost** libraries in the correct paths. For more details on setting up `gnuplot-iostream`, check the repository [here](https://github.com/dstahlke/gnuplot-iostream)

## How the Program Works
1. **File Selection:** The program uses Zenity to open a file selector and obtain the path to the stereo audio file.
2. **Entropy Calculation:** The program calculates entropy by:
    - Identifying all unique amplitudes in the audio sample.
    - Counting the occurrences of each amplitude to compute its probability.
    - Applying the entropy formula: `Σ(p * log2(p))` for each unique amplitude.
3. **Huffman Tree Creation:**
    - A Huffman tree is built from the amplitude probabilities using a bottom-up approach (starting with the two least frequent amplitudes).
    - Binary codes are assigned by recursively appending 0s to the left subtree and 1s to the right subtree.
4. **Average Code Length Calculation:** The program computes the average code length by multiplying the length of each code with its corresponding probability.

## Implementation Details
- The Huffman tree structure is based on examples from lecture slides and supplemented by information from [GeeksForGeeks' Huffman Coding tutorial.](https://www.geeksforgeeks.org/huffman-coding-greedy-algo-3/)
- The tree is generated by combining the two least probable amplitudes iteratively.
- Code length is adjusted according to the position in the tree, and the final average length is computed by considering both code length and probability.

## Valgrind Testing
Valgrind reported no memory errors during testing. However, as noted by experts, the absence of errors does not guarantee the absence of memory leaks or issues.

## Disclaimer
This program has only been tested on **Ubuntu.** Functionality on other operating systems, particularly Windows, is not guaranteed. Many of the shell commands provided were part of the development process, and not all may be necessary for the final version.

## Future Improvements
    - Actual compression implementation.
    - Cross-platform compatibility (including Windows).
    - Optimization of the Huffman tree and entropy calculations.


