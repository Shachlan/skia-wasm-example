run ./setup.sh in order to prepare all of the resources necessary for building.

run ./build-skia.sh in order to build the Skia library.

run ./compile.sh in order to compile my code with Skia and see the undefined symbols.

alternatively, run ./e2e.sh in order to run all 3 scripts together.



Expected output:
error: undefined symbol: FT*Palette_Select
error: undefined symbol: \_Z14Sk3PerspectiveP10SkMatrix44fff
error: undefined symbol: \_Z9Sk3LookAtP10SkMatrix44RK8SkPoint3S3_S3*
error: undefined symbol: _ZN10SkCubicMapC1E7SkPointS0_
error: undefined symbol: \_ZN11SkOpBuilder3addERK6SkPath8SkPathOp
error: undefined symbol: \_ZN11SkOpBuilder7resolveEP6SkPath
error: undefined symbol: \_ZN6skjson3DOMC1EPKcm
error: undefined symbol: \_ZN7SkParse7FindHexEPKcPj
error: undefined symbol: \_ZNK10SkCubicMap13computeYFromXEf
error: undefined symbol: \_ZNK6skjson11ObjectValueixEPKc
error: undefined symbol: \_ZNK6skjson5Value8toStringEv
error: undefined symbol: SK_EMBEDDED_FONTS
Error: Aborting compilation due to previous errors

notice that Skia might require some packages to be installed on the machine, including
freetype2, libpng and the OpenGL framework.
