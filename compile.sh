em++ -g4 -o ./assets/appWASM.js ./src/*.cpp \
-std=c++17 \
-I./src -I./third_party/skia/include/core \
-I./third_party/skia/include/gpu \
-I./third_party/skia/ \
./third_party/skia/out/Build-wasm-Release/Release/*.a \
-g4 \
-O3 \
-s LEGACY_GL_EMULATION=0 \
-s ALLOW_MEMORY_GROWTH=1 \
-s USE_WEBGL2=1 \
-s WASM=0 \
-s NO_EXIT_RUNTIME=1 \
-s EXTRA_EXPORTED_RUNTIME_METHODS=['ccall'] \
-s ERROR_ON_UNDEFINED_SYMBOLS=1 \
-s USE_LIBPNG=1 \
-s USE_FREETYPE=1 \
--embed-file ./fonts \
--embed-file ./data.json
