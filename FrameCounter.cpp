// This code is an object oriented port of: http://sdl.beuc.net/sdl.wiki/SDL_Average_FPS_Measurement

#include <SDL.h>
#include "FrameCounter.h"

// This function gets called once on startup.
void FrameCounter::init() {
    // Set all frame times to 0ms.
    memset(m_frametimes, 0, sizeof(m_frametimes));
    m_framecount = 0;
    m_framespersecond = 0;
    m_frametimelast = SDL_GetTicks();
}

void FrameCounter::update() {
    Uint32 frametimesindex;
    Uint32 getticks;
    Uint32 count;
    Uint32 i;

    // frametimesindex is the position in the array. It ranges from 0 to FRAME_VALUES.
    // This value rotates back to 0 after it hits FRAME_VALUES.
    frametimesindex = m_framecount % m_avg_period;

    // store the current time
    getticks = SDL_GetTicks();

    // save the frame time value
    m_frametimes[frametimesindex] = getticks - m_frametimelast;

    // save the last frame time for the next fpsthink
    m_frametimelast = getticks;

    // increment the frame count
    m_framecount++;

    // Work out the current framerate

    // The code below could be moved into another function if you don't need the value every frame.

    // I've included a test to see if the whole array has been written to or not. This will stop
    // strange values on the first few (FRAME_VALUES) frames.
    if (m_framecount < m_avg_period) {
        count = m_framecount;
    } else {
        count = m_avg_period;
    }

    // add up all the values and divide to get the average frame time.
    m_framespersecond = 0;
    for (i = 0; i < count; i++) {
        m_framespersecond += m_frametimes[i];
    }

    m_framespersecond /= count;

    // now to make it an actual frames per second value...
    m_framespersecond = 1000.f / m_framespersecond;
}