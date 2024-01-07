#include "file_selector.h"
#include <iostream>
#include <sndfile.h> //I had to install an additional package for this to work
#include <fstream>
//the boost library was needed to use gnuplot-iostream.h
#include "gnuplot-iostream/gnuplot-iostream.h"
#include <cstdlib>
#include <string>
#include <cstdio>
#include <cmath>
#include <string>

using namespace std;

class sound_file {
    SNDFILE *sndfile; //the file itself
    SF_INFO sfinfo; //information about the file
    float *data; //array containing the values of the sound
    long length; //length of the array
    double entropy;
    int distinct; //the number of distinct amplitudes
    vector<double> prob; //stores the probability of the amplitudes
    vector<double> ampl; //this stores the distinct amplitudes

    bool not_done_yet(float& value, long j) { 
    //check if value's entropy has already been calculated
        for (long i=0; i<j; i++) { 
        //go upto j since we only care about what came before
            if (data[i] == value) return false;
        }
        return true;
    }

    //idea for huffman coding taken from www.geeksforgeeks.org/huffman-coding-greedy-algo-3/
    struct Node {
        string code;
        Node* parent;
        Node* left;
        Node* right;
        double ampl;
        double prob;

        Node(Node* l, Node* r, double a, double p) : 
        code(""), parent(nullptr), left(l), right(r), ampl(a), prob(p) {}
        Node() : code(""), parent(nullptr), left(nullptr), right(nullptr), ampl(0) {}
        Node(double a, double p) : 
        code(""), parent(nullptr), left(nullptr), right(nullptr), ampl(a), prob(p) {}
    };

    struct minheap {
        int size; //current size of heap
        int capacity; //capacity of minheap
        Node** array;
        minheap(int cap) : size(0), capacity(cap), array(nullptr) {
            array = new Node* [cap];
        }
        ~minheap() {
            delete[] array;
        }
    };

    Node* root;
    minheap* heap;
    double avg_code_len;

    void swap_nodes(Node** a, Node** b) {
        Node* temp = *a;
        *a = *b;
        *b = temp;
    }

    void minheapify(minheap* heap, int idx) {
        int smallest = idx;
        int left = 2 * idx + 1;
        int right = 2 * idx + 2;

        if (left < heap->size && heap->array[left]->prob < heap->array[smallest]->prob) {
            smallest = left;
        }
        if (right < heap->size && heap->array[right]->prob < heap->array[smallest]->prob) {
            smallest = right;
        }
        if (smallest != idx) {
            swap_nodes(&heap->array[smallest], &heap->array[idx]);
            minheapify(heap, smallest); //apply recursion
        }

    }

    Node* get_min(minheap* heap) { //also removes the smallest
        Node* ret = heap->array[0];
        heap->array[0] = heap->array[heap->size - 1];
        heap->size--;
        minheapify(heap, 0);
        return ret;
    }

    void insert_to_heap (minheap* heap, Node* node) {
        heap->size++;
        int i = heap->size - 1;
        while (i && node->prob < heap->array[(i - 1) / 2]->prob) {
            heap->array[i] = heap->array[(i - 1) / 2];
            i = (i - 1) / 2;
        }
        heap->array[i] = node;
    }

    void build_heap(minheap* heap) {
        int n = heap->size - 1;
        for (int i=(n - 1) / 2; i >= 0; i--) {
            minheapify(heap, i);
        }
    }

    void createandbuild() {
        heap = new minheap(distinct);
        for (int i=0; i<distinct; i++) {
            heap->array[i] = new Node(ampl[i], prob[i]);
        }

        heap->size = distinct;
        build_heap(heap);
    }

    void huffman_tree() {
        Node* left;
        Node* right;
        Node* parent;
        createandbuild();
        while(heap->size != 1) {
            //get two smallest
            left = get_min(heap);
            right = get_min(heap);

            parent = new Node(left, right, NAN, left->prob + right->prob);

            insert_to_heap(heap, parent);
        }

        root = get_min(heap);
    }

    void remove(Node* node) {
        if (!node) return;
        remove(node->left);
        remove(node->right);
        delete node;
    }

    bool leaf(Node* node) {
        return !(node->left) && !(node->right); 
    } 

    void assign_values(Node* node, string num) {
        if (!node) return;
        node->code = num;
        if (node->left) {
            assign_values(node->left, num + "0");
        }
        if (node->right){
            assign_values(node->right, num + "1");
        }
        if (leaf(node)) {
            double avg = static_cast<double>
            (node->code.length()) * node->prob;

            avg_code_len += avg;
        }
    }

public:
    sound_file(const char* filename) : 
    sndfile(nullptr), sfinfo(), data(nullptr), length(0) , entropy(0), 
    distinct(0), prob({}), ampl({}), root(nullptr), avg_code_len(0) {
        sndfile = sf_open(filename, SFM_READ, &sfinfo);
        if (!sndfile) {
            cerr << "Error opening audio file" << endl;
            return;
        }
        //sfinfo.frames is the number of sample in the audio channel
        //length is also equal to the total number of samples
        length = sfinfo.frames * sfinfo.channels;
        data = new float[length];

        if (!data) {
            cerr << "Error allocating memory" << endl;
            sf_close(sndfile);
            // Handle the memory allocation error
            return;
        }

        //the next line is a function that reads the audio file
        sf_readf_float(sndfile, data, sfinfo.frames);

        //calculate the entropy
        for (long i=0; i<length; i++) {
            if (not_done_yet(data[i], i)) {
                distinct++;
                long count = 1;
                for (long j=i+1; j<length; j++) { //find # w/ same amplitude
                    if (data[j] == data[i]) count++;
                }
                double probability = static_cast<double>(count) / length;
                entropy += probability * log2(probability);
                //the following line automatically finds where to place the prob
                auto it = std::lower_bound(prob.begin(), prob.end(), probability);
                int k = it - prob.begin();
                prob.insert(it, probability);
                ampl.insert(ampl.begin() + k, data[i]);
            }
        }   
        entropy *= -1; //negate
    }

    double get_avg_code_length() {
        huffman_tree();
        root->code = "0";
        assign_values(root->left, "0");
        assign_values(root->right, "1");
        return avg_code_len;
    }
    
    //the sf_open function only takes const char* so string must be converted
    sound_file(const std::string& filename) : sound_file(filename.c_str()) {}
    
    // Destructor to free allocated memory
    ~sound_file() {
        if (data) {
            delete[] data;
        }
        if (sndfile) {
            sf_close(sndfile);
        }
        remove(root);
        delete heap;
    }

    void print_values() { //used for debugging
        for (int i=0; i < length; i++) {
            std::cout << data[i] << endl;
        }
    }

//gnuplot was causing problems. This was solved by reading
//https://www.cfd-online.com/Forums/openfoam-post-processing/224942-gnuplot-stopped-working-
//unable-find-shared-library.html

    void display_waveform() {
        if (!sndfile) return;
        Gnuplot gp;

        // Set plot title and axis labels
        gp << "set term x11" << endl;

        gp << "set xlabel 'Sample'" << endl;
        gp << "set ylabel 'Amplitude'" << endl;
        
        //multiplot allows two graphs on one window
        //2, 1 means 2 rows and 1 column
        gp << "set multiplot layout 2, 1" << endl;
        
        //length = total number of samples
        gp << "set title 'Entropy: " << entropy << " bits/sample" << "'" << endl; 
        gp << "plot '-' with lines title 'Left channel'" << endl;

        //first handle left channel, then right

        //all even number values in data are for the left channel
        //this is why this for loop starts at 0, the other one starts at 1
        // and both loops increments by 2
        //this part assumes the audio is only stereo like the instructions stated
        for (size_t i=0; i<length; i += 2) {
            gp << i << " " << data[i] << endl;
        }
        gp << "e" << endl; //tells the program to create a new graph

        gp << "set title 'Average code word length: " << avg_code_len<< " bits/sample" << "'" << endl;

        gp << "set xlabel 'Sample'" << endl;
        gp << "set ylabel 'Amplitude'" << endl;
        gp << "plot '-' with lines title 'Right channel'" << endl;

        for (size_t i=1; i<length; i += 2) {
            gp << i << " " << data[i] << endl;
        }
        gp << "e" << endl;

        gp << "unset multiplot" << endl;
    }

    int get_channels() {
        return sfinfo.channels;
    }

    float get_entropy() {
        return entropy;
    }

    void print_ampl() {
        for (int i=0; i<distinct; i++) {
            cout << ampl[i] << endl;
        }
    }

    void print_prob() {
        for (int i=0; i<distinct; i++) {
            cout << prob[i] << endl;
        }
    }
};

int main() {
    char* file = open_file(); //opens the open file box
    sound_file wave(file); //constructs the class / gets the data
    free(file);
    wave.get_avg_code_length();
    wave.display_waveform();
    return 0;
}
