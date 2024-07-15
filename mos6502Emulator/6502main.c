
#define Byte unsigned char //8bits
#define Word unsigned short //16bits

struct CPU {
    
    Word PC; //Program Counter 16bit
    Byte SP; //Stack Pointer 8bit
    Byte ACC; //accumulator 8bit
    Byte X,Y; //registers 8bit
    
    Byte C : 1; //Carry Flag
    Byte Z : 1; //Zero Flag
    Byte I : 1; //Interrupt Disable
    Byte D : 1; //Decimal Mode
    Byte B : 1; //Break Command
    Byte O : 1; //Overflow Flag
    Byte N : 1  //Negative Flag
};


int main() {
    
}