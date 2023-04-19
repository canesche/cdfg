#define NUM TAPS 4

void fir(int input, int *output, int taps[NUM TAPS]) {
    
    static int delay line[NUM TAPS] = {};
    int result = 0;
    
    for (int i = NUM TAPS - 1; i > 0; i--) {
        delay line[i] = delay line[i - 1];
    }

    delay line[0] = input;
    
    for (int i = 0; i < NUM TAPS; i++) {
        delay line[i] = delay line[i - 1];
    }
    output = result;
}