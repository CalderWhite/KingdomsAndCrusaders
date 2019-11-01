// This code is an object oriented port of: http://sdl.beuc.net/sdl.wiki/SDL_Average_FPS_Measurement

#include <SDL.h>

class FrameCounter {
public:
    void init();
    void update();

    inline float getFps() const {
        return m_framespersecond;
    }

private:
    static const int m_avg_period = 10;

    // An array to store frame times:
    Uint32 m_frametimes[m_avg_period];

    // Last calculated SDL_GetTicks
    Uint32 m_frametimelast;

    // total frames rendered
    Uint32 m_framecount;

    // the value you want
    float m_framespersecond;
};