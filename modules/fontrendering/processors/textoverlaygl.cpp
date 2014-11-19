/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2013-2014 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * Contact: Erik Sundén
 *
 *********************************************************************************/

#include "textoverlaygl.h"
#include <inviwo/core/common/inviwoapplication.h>
#include <modules/opengl/glwrap/shader.h>
#include <modules/opengl/textureutils.h>
#include <modules/fontrendering/fontrenderingmodule.h>

namespace inviwo {

ProcessorClassIdentifier(TextOverlayGL, "org.inviwo.TextOverlayGL");
ProcessorDisplayName(TextOverlayGL, "Text Overlay");
ProcessorTags(TextOverlayGL, Tags::GL);
ProcessorCategory(TextOverlayGL, "Drawing");
ProcessorCodeState(TextOverlayGL, CODE_STATE_STABLE);

TextOverlayGL::TextOverlayGL()
    : Processor()
    , inport_("inport")
    , outport_("outport", &inport_)
    , text_("Text", "Text", "Lorem ipsum etc.", INVALID_OUTPUT,
            PropertySemantics::TextEditor)
    , font_size_(20)
    , xpos_(0)
    , ypos_(0)
    , color_("color_", "Color", vec4(1.0f), vec4(0.0f), vec4(1.0f), vec4(0.01f),
                  INVALID_OUTPUT, PropertySemantics::Color)
    , fontSize_("Font size", "Font size")
    , fontPos_("Position", "Position", vec2(0.0f), vec2(0.0f), vec2(1.0f), vec2(0.01f))
    , refPos_("Reference", "Reference", vec2(-1.0f), vec2(-1.0f), vec2(1.0f), vec2(0.01f))
    , copyShader_(NULL)
    , textShader_(NULL) {

    addPort(inport_);
    addPort(outport_);
    addProperty(text_);
    addProperty(color_);
    addProperty(fontPos_);
    addProperty(refPos_);
    addProperty(fontSize_);
    fontSize_.addOption("10", "10", 10);
    fontSize_.addOption("12", "12", 12);
    fontSize_.addOption("18", "18", 18);
    fontSize_.addOption("24", "24", 24);
    fontSize_.addOption("36", "36", 36);
    fontSize_.addOption("48", "48", 48);
    fontSize_.addOption("60", "60", 60);
    fontSize_.addOption("72", "72", 72);
    fontSize_.setSelectedIndex(3);
    fontSize_.setCurrentStateAsDefault();
}

TextOverlayGL::~TextOverlayGL() {}

void TextOverlayGL::initialize() {
    Processor::initialize();
    copyShader_ = new Shader("img_copy.frag", true);
    textShader_ = new Shader("fontrendering_freetype.vert", "fontrendering_freetype.frag", true);
    int error = 0;

    if (FT_Init_FreeType(&fontlib_)) LogWarn("FreeType: Major error.");

    std::string arialfont = InviwoApplication::getPtr()
                                ->getModuleByType<FontrenderingModule>()
                                ->getPath() + "/fonts/arial.ttf";

    error = FT_New_Face(fontlib_, arialfont.c_str(), 0, &fontface_);
    if (error == FT_Err_Unknown_File_Format) {
        LogWarn("FreeType: File opened and read, format unsupported.");
    } else if (error) {
        LogWarn("FreeType:  Could not read/open the font file.");
    }

    glGenTextures(1, &texCharacter_);
    glGenBuffers(1, &vboCharacter_);
}

void TextOverlayGL::deinitialize() {
    delete copyShader_;
    delete textShader_;
    copyShader_ = NULL;
    textShader_ = NULL;
    glDeleteTextures(1, &texCharacter_);
    glDeleteBuffers(1, &vboCharacter_);
    Processor::deinitialize();
}


vec2 TextOverlayGL::measure_text(const char* text, float sx, float sy) {
    const char* p;
    FT_Set_Pixel_Sizes(fontface_, 0, fontSize_.get());

    float x = 0.0f;
    float y = 0.0f;

    float maxx = 0.0f;
    float maxy = 0.0f;

    float offset = 0.0f;

    char lf = (char)0xA;  // Line Feed Ascii for std::endl, \n
    char tab = (char)0x9;  // Tab Ascii

    for (p = text; *p; p++) {
        if (FT_Load_Char(fontface_, *p, FT_LOAD_RENDER)) {
            LogWarn("FreeType: could not render char");
            continue;
        }

        float w = fontface_->glyph->bitmap.width * sx;
        float h = fontface_->glyph->bitmap.rows * sy;

        if(y == 0.0f) y+=2*h;

        if (*p == lf) {
            y += (2 * h);
            x += (fontface_->glyph->advance.x >> 6) * sx;
            y += (fontface_->glyph->advance.y >> 6) * sy;
            maxx = std::max(maxx, x);
            x = 0.0f;
            continue;
        } else if (*p == tab) {
            x += (fontface_->glyph->advance.x >> 6) * sx;
            y += (fontface_->glyph->advance.y >> 6) * sy;
            x += (4 * w);  // 4 times glyph character width
            maxx = std::max(maxx, x);
            continue;
        }
        x += (fontface_->glyph->advance.x >> 6) * sx;
        maxx = std::max(maxx, x);

        y += (fontface_->glyph->advance.y >> 6) * sy;
        maxy = std::max(maxy, y);
    }
    
    
    return vec2(maxx, maxy);
}


void TextOverlayGL::render_text(const char* text, float x, float y, float sx, float sy,
                                unsigned int unitNumber) {
    const char* p;
    FT_Set_Pixel_Sizes(fontface_, 0, fontSize_.get());

    float offset = 0;
    float inputX = x;

    // TODO: To make things more reliable ask the system for proper ascii
    char lf = (char)0xA;  // Line Feed Ascii for std::endl, \n
    char tab = (char)0x9;  // Tab Ascii

    for (p = text; *p; p++) {
        if (FT_Load_Char(fontface_, *p, FT_LOAD_RENDER)) {
            LogWarn("FreeType: could not render char");
            continue;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, fontface_->glyph->bitmap.width,
                     fontface_->glyph->bitmap.rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE,
                     fontface_->glyph->bitmap.buffer);
        float x2 = x + fontface_->glyph->bitmap_left * sx;
        float y2 = -y - fontface_->glyph->bitmap_top * sy;
        float w = fontface_->glyph->bitmap.width * sx;
        float h = fontface_->glyph->bitmap.rows * sy;

        if (*p == lf) {
            offset += (2 * h);
            x += (fontface_->glyph->advance.x >> 6) * sx;
            y += (fontface_->glyph->advance.y >> 6) * sy;
            x = inputX;
            continue;
        } else if (*p == tab) {
            x += (fontface_->glyph->advance.x >> 6) * sx;
            y += (fontface_->glyph->advance.y >> 6) * sy;
            x += (4 * w);  // 4 times glyph character width
            continue;
        }

        y2 += offset;

        GLfloat box[4][4] = {
            {x2, -y2, 0, 0}, {x2 + w, -y2, 1, 0}, {x2, -y2 - h, 0, 1}, {x2 + w, -y2 - h, 1, 1},
        };
        textShader_->setUniform("tex", (GLint)unitNumber);
        textShader_->setUniform("color", color_.get());
        glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_STREAM_DRAW);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        x += (fontface_->glyph->advance.x >> 6) * sx;
        y += (fontface_->glyph->advance.y >> 6) * sy;
    }
}

void TextOverlayGL::process() {
    TextureUnit colorUnit, depthUnit, pickingUnit;
    utilgl::bindTextures(inport_, colorUnit, depthUnit, pickingUnit);

    utilgl::activateAndClearTarget(outport_);
    copyShader_->activate();
    copyShader_->setUniform("color_", colorUnit.getUnitNumber());
    copyShader_->setUniform("depth_", depthUnit.getUnitNumber());
    copyShader_->setUniform("picking_", pickingUnit.getUnitNumber());
    utilgl::singleDrawImagePlaneRect();
    copyShader_->deactivate();
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    TextureUnit texUnit;
    texUnit.activate();
    glBindTexture(GL_TEXTURE_2D, texCharacter_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    GLuint attribute_location = 0;
    glEnableVertexAttribArray(attribute_location);
    glBindBuffer(GL_ARRAY_BUFFER, vboCharacter_);
    glVertexAttribPointer(attribute_location, 4, GL_FLOAT, GL_FALSE, 0, 0);
    float sx = 2.f / outport_.getData()->getDimension().x;
    float sy = 2.f / outport_.getData()->getDimension().y;
    font_size_ = fontSize_.getSelectedValue();
    xpos_ = fontPos_.get().x * outport_.getData()->getDimension().x;
    ypos_ = fontPos_.get().y * outport_.getData()->getDimension().y + float(font_size_);
    textShader_->activate();

    vec2 size = measure_text(text_.get().c_str(), sx, sy);
    vec2 shift = 0.5f * size * (refPos_.get() + vec2(1.0f,1.0f));
    render_text(text_.get().c_str(), -1 + xpos_*sx - shift.x, 1 - ypos_*sy + shift.y, sx, sy,
                texUnit.getUnitNumber());

    textShader_->deactivate();
    glDisableVertexAttribArray(attribute_location);
    glDisable(GL_BLEND);
    utilgl::deactivateCurrentTarget();
}

}  // namespace
