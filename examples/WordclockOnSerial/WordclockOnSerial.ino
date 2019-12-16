#include <Wordclock.h>

class MatrixEmulator
{
public:
    MatrixEmulator()
    {

    }
    typedef int color;

    static const int SIZE = 10*11;
    static const char *MatrixLetters;
    static const color StdColor = 0;

    void setLed(int idx, color c)
    {
        if ((idx >=0) && (idx < SIZE))
        {
            m_ledText[idx] = MatrixLetters[idx];
        }
    }

    void print()
    {
        for (int n=0; n<SIZE; ++n)
        {
            Serial.print(m_ledText[n]);
        }
        Serial.println("");
        Serial.flush();
    }

    void clear()
    {
        for (int n=0; n<SIZE; ++n)
        {
            m_ledText[n]=' ';
        }
    }

private:
    char m_ledText[10*11];
};

const char* MatrixEmulator::MatrixLetters = "ESKISTAFUNFZEHNZWANZIGDREIVIERTELVORMCLONACHHALBDELFUNFEINSOAMZWEIDREILARVIERSECHSNLACHTSIEBENZWOLFZEHNEUNKUHR";

MatrixEmulator matrixEmu;
Wordclock<LedMatrixConfig_DE_11x10> wordclock;

void setup()
{
    Serial.begin(115200);
    while (!Serial); // wait for serial attach

    Serial.println();
    Serial.println("Initializing...");
    Serial.flush();

}

void loop()
{
    for (uint8_t h=0; h<23; ++h)
    {
        for (uint8_t m=0; m<60; m+=5)
        {
            Serial.print("Current Hours ");
            Serial.print(h);
            Serial.print(":");
            Serial.print(m);
            Serial.println("");
            Serial.flush();
            matrixEmu.clear();
            wordclock.setTime(h, m, matrixEmu, MatrixEmulator::StdColor);
            matrixEmu.print();
            delay(100);
        }
    }

}