#include "openGLShading.hpp"

#include <emscripten.h>
extern "C"
{
#include "html5.h"
}
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES3/gl3.h>

#define PIXEL_FORMAT GL_RGBA

#include <math.h>
#include <src/gpu/gl/GrGlDefines.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <GrContext.h>
#include <SkData.h>
#include <SkFont.h>
#include <SkGraphics.h>
#include <SkImage.h>
#include <SkPaint.h>
#include <SkPictureRecorder.h>
#include <SkStream.h>
#include <SkString.h>
#include <SkSurface.h>
#include <SkTextBlob.h>
#include <SkTypeface.h>
#include <gl/GrGLInterface.h>
#include <modules/skottie/include/Skottie.h>
#include <src/core/SkMakeUnique.h>
#include <src/core/SkOSFile.h>
#include <src/utils/SkOSPath.h>

static sk_sp<GrContext> skiaContext;
static sk_sp<SkSurface> lottie_surface;
GLuint lottie_texture;
static sk_sp<skottie::Animation> anim;

GLuint get_texture()
{
  GLuint textureLoc;
  glGenTextures(1, &textureLoc);
  glActiveTexture(GL_TEXTURE0 + textureLoc);

  glBindTexture(GL_TEXTURE_2D, textureLoc);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);
  return textureLoc;
}

static sk_sp<SkSurface> create_surface(int width, int height, GLuint texture_name)
{
  glBindTexture(GL_TEXTURE_2D, texture_name);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
  GrGLTextureInfo texture_info = {
      .fID = texture_name, .fTarget = GL_TEXTURE_2D, .fFormat = GR_GL_RGBA8};
  GrBackendTexture texture(width, height, GrMipMapped::kNo, texture_info);
  auto surface =
      sk_sp(SkSurface::MakeFromBackendTexture(skiaContext.get(), texture, kTopLeft_GrSurfaceOrigin,
                                              0, kRGBA_8888_SkColorType, nullptr, nullptr));
  if (!surface)
  {
    printf("SkSurface::MakeRenderTarget returned null\n");
    exit(1);
  }

  return surface;
}

void setupOpenGL(int width, int height, char *canvasName)
{
  EmscriptenWebGLContextAttributes attrs;
  attrs.explicitSwapControl = 0;
  attrs.depth = 1;
  attrs.stencil = 1;
  attrs.antialias = 1;
  attrs.majorVersion = 1;
  attrs.minorVersion = 0;
  attrs.enableExtensionsByDefault = true;

  int emscripten_context = emscripten_webgl_create_context(canvasName, &attrs);
  emscripten_webgl_make_context_current(emscripten_context);

  glViewport(0, 0, width, height);

  skiaContext = GrContext::MakeGL();
  lottie_texture = get_texture();
  lottie_surface = create_surface(width, height, lottie_texture);

  SkAutoGraphics ag;

  anim = skottie::Animation::Builder().makeFromFile("data.json");
}

uint32_t render_lottie(double time)
{
  double intpart;

  auto fractpart = modf(time, &intpart);

  glBindVertexArray(0);
  skiaContext->resetContext();

  auto canvas = lottie_surface->getCanvas();

  {
    SkAutoCanvasRestore acr(canvas, true);
    anim->seek(fractpart);

    canvas->concat(SkMatrix::MakeRectToRect(
        SkRect::MakeSize(anim->size()), SkRect::MakeIWH(256, 256), SkMatrix::kCenter_ScaleToFit));

    canvas->clear(SK_ColorTRANSPARENT);

    anim->render(canvas);
    canvas->flush();
  }

  return lottie_texture;
}
