#include "apilib.h"

// 可恶!!!!! 虚拟机无法发声!!!!
void HariMain(void)
{
    int i, timer;
    timer = api_alloctimer();
    api_inittimer(timer, 128);
    for (i = 200000000; i >= 20000; i -= i / 100)
    {
        // 20KHz ~ 20Hz 人类可以听到的范围
        // i 以 1% 的速度递减
        api_beep(i);
        api_settimer(timer, 1);
        if (api_getkey(1) != 128)
        {
            break;
        }
    }
    api_beep(0);
    api_end();
}
