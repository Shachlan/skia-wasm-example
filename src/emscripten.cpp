#include <emscripten.h>
#include <iostream>
#include <string>
extern "C"
{
#include "html5.h"
}
#include "openGLShading.hpp"

int main(int argc, char const *argv[])
{
    printf("[WASM] Loaded\n");

    EM_ASM(
        if (typeof window != "undefined") {
            window.dispatchEvent(new CustomEvent("wasmLoaded"))
        } else {
            global.onWASMLoaded && global.onWASMLoaded()});

    return 0;
}

extern "C"
{

    EMSCRIPTEN_KEEPALIVE
    void clearContexts(void)
    {
    }

    EMSCRIPTEN_KEEPALIVE
    void createContext(int width, int height, char *id)
    {
        printf("creating %s context\n", id);
        try
        {
            setupOpenGL(width, height, id);
        }
        catch (std::exception ex)
        {
            std::cout << ex.what() << '\n';
        }
        free(id);
    }

    EMSCRIPTEN_KEEPALIVE
    uint32_t render_lottieRun(double time)
    {
        return render_lottie(time);
    }
}
