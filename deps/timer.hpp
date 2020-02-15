/*
 MIT License

 Copyright (c) 2022 https://github.com/arlogy

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
*/

#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <cmath>
#include <string>

namespace lib
{
class timer
{
private:
    typedef std::chrono::high_resolution_clock hrclock_t;

public:
    explicit timer() { reset(); }

    /// Sets the start time of this timer to the current time. Called in the
    /// constructor for initialization purpose.
    void reset() { m_begin = hrclock_t::now(); }

    /// Returns the elapsed time in milliseconds since the last reset().
    double elapsed_time() const
    {
        std::chrono::time_point<hrclock_t> end = hrclock_t::now();
        return std::chrono::duration<double, std::milli>(end - m_begin).count();
    }

    /// Returns elapsed_time() without floating point precision. std::ceil() is
    /// first used to not completely ignore the floating point precision. The
    /// resulting duration is then casted to long long which might lead to loss
    /// of data for a huge floating point number (should be rare though, so
    /// casting should not be a problem).
    long long elapsed_time_rounded() const
    {
        return static_cast<long long>(std::ceil(elapsed_time()));
    }

    /// Returns a stringified version of the elapsed time which is rounded by
    /// default.
    std::string elapsed_time_str(bool rounded = true) const
    {
        if(rounded) {
            return "(" + std::to_string(elapsed_time_rounded()) + " ms)";
        }
        return "(" + std::to_string(elapsed_time()) + " ms)";
    }

private:
    std::chrono::time_point<hrclock_t> m_begin;
};
}

#endif // TIMER_H
