#ifndef WORDCLOCK_H_INCLUDED
#define WORDCLOCK_H_INCLUDED

#include <Arduino.h>
#include <stdint.h>
#include <stdarg.h>

template <int X, int Y>
class TLedMatrix
{
public:
    static const int SIZE_X = X;
    static const int SIZE_Y = Y;
    static const int SIZE = SIZE_X * SIZE_Y;

    TLedMatrix()
    {
        clear();
    }

    TLedMatrix(const TLedMatrix<X,Y>& f_other)
    {
        for (int n=0; n<SIZE; ++n)
        {
            m_ledState[n] = f_other.m_ledState[n];
        }
    }

    void clear()
    {
        for (int n=0; n<SIZE; ++n)
        {
            m_ledState[n]=0u;
        }
    }

    void setLeds(int numActiveLeds, ...)
    {
        va_list leds;
        va_start(leds, numActiveLeds);
        for (int n=0; n<numActiveLeds; ++n)
        {
            int idx = va_arg(leds, int);
            if ((idx >= 0) && (idx < SIZE))
            {
                m_ledState[idx] = 1u;
            }
        } 
        va_end(leds);     
    }

    bool isLedActive(int idx)
    {
        if ((idx >=0) && (idx < SIZE))
        {
            return (m_ledState[idx] > 0);
        }
        return false;
    }

    TLedMatrix<X,Y>& operator= (const TLedMatrix<X,Y>& f_other)
    {
        for (int n=0; n<SIZE; ++n)
        {
            m_ledState[n] = f_other.m_ledState[n];
        }
        return *this;
    }

    TLedMatrix<X,Y>& operator+= (const TLedMatrix<X,Y>& f_other)
    {
        for (int n=0; n<SIZE; ++n)
        {
            m_ledState[n] += f_other.m_ledState[n];
        }
        return *this;
    }

protected:
    uint8_t m_ledState[SIZE];
};

template <class LED_MATRIX_CONFIG>
class Wordclock
{
public:
    typedef LED_MATRIX_CONFIG led_matrix_config_t;
    typedef typename led_matrix_config_t::led_matrix_t led_matrix_t;

    Wordclock()
    {
    }

    const led_matrix_t& getLedMatrix() const;

    template <class LedMatrixCtrl, class Color>
    void setTime(uint8_t hour, uint8_t minutes, LedMatrixCtrl& matrixCtrl, Color color)
    {
        led_matrix_config_t::setTime(hour, minutes, m_ledMatrix);
        for (int n=0; n<led_matrix_t::SIZE; ++n)
        {
            if (m_ledMatrix.isLedActive(n))
            {
                matrixCtrl.setLed(n, color);
            }
        }
        // set the four corner minute LEDs
    }

protected:
    led_matrix_t m_ledMatrix;
};

class LedMatrixConfig_DE_11x10
{
public:
    typedef TLedMatrix<11,10> led_matrix_t;

    static void setTime(uint8_t hour, uint8_t minutes, led_matrix_t& matrix) 
    {
        matrix.clear();
        matrix.setLeds(5,0,1,3,4,5); // es ist
        
        // set minutes and increment the hour if needed
        const uint8_t minutesBy5 = (minutes / 5) * 5;
        bool fullHour = false;
        switch (minutesBy5)
        {
            case 0 : matrix.setLeds(3,107,108,109); fullHour=true; break;
            case 5 : matrix.setLeds(8,7,8,9,10,40,41,42,43); break;
            case 10: matrix.setLeds(8,11,12,13,14,40,41,42,43); break;
            case 15: matrix.setLeds(11,26,27,28,29,30,31,32,40,41,42,43); break;
            case 20: matrix.setLeds(11,15,16,17,18,19,20,21,40,41,42,43); break;
            case 25: matrix.setLeds(11,7,8,9,10,33,34,35,44,45,46,47); hour++; break;
            case 30: matrix.setLeds(4,44,45,46,47); hour++; break;
            case 35: matrix.setLeds(12,7,8,9,10,40,41,42,43,44,45,46,47); hour++; break;
            case 40: matrix.setLeds(10,15,16,17,18,19,20,21,33,34,35); hour++; break;
            case 45: matrix.setLeds(10,26,27,28,29,30,31,32,33,34,35); hour++; break;
            case 50: matrix.setLeds(7,11,12,13,14,33,34,35); hour++; break;
            case 55: matrix.setLeds(7,7,8,9,10,33,34,35); hour++; break;
        }

        const uint8_t hour12 = hour % 12;
        switch (hour12)
        {
            case 0: matrix.setLeds(5,94,95,96,97,98); break;
            case 1: fullHour ? matrix.setLeds(3,55,56,57) : matrix.setLeds(4,55,56,57,58); break;
            case 2: matrix.setLeds(4,62,63,64,65); break;
            case 3: matrix.setLeds(4,66,67,68,69); break;
            case 4: matrix.setLeds(4,73,74,75,76); break;
            case 5: matrix.setLeds(4,51,52,53,54); break;
            case 6: matrix.setLeds(5,77,78,79,80,81); break;
            case 7: matrix.setLeds(6,88,89,90,91,92,93); break;
            case 8: matrix.setLeds(4,84,85,86,87); break;
            case 9: matrix.setLeds(4,102,103,104,105); break;
            case 10: matrix.setLeds(4,99,100,101,102); break;
            case 11: matrix.setLeds(3,49,50,51); break;
        }
    }
};

#endif // WORDCLOCK_H_INCLUDED