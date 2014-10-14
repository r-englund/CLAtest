/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2012-2014 Inviwo Foundation
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

namespace inviwo {

ProcessorClassIdentifier(TextOverlayGL, "org.inviwo.TextOverlayGL");
ProcessorDisplayName(TextOverlayGL, "Text Overlay");
ProcessorTags(TextOverlayGL, Tags::GL);
ProcessorCategory(TextOverlayGL, "Drawing");
ProcessorCodeState(TextOverlayGL, CODE_STATE_EXPERIMENTAL);

TextOverlayGL::TextOverlayGL()
    : Processor()
    , inport_("inport")
    , outport_("outport")
    , textStringProperty_("Text", "Text", "Lorem ipsum etc.", PropertyOwner::INVALID_OUTPUT,
                          PropertySemantics::TextEditor)
    , font_size_(20)
    , xpos_(0)
    , ypos_(0)
    , floatColor_("FloatColor", "FloatColor", vec4(0.2f), vec4(0.0f), vec4(0.1f), vec4(1.0f),
                  PropertyOwner::INVALID_OUTPUT, PropertySemantics::Color)
    , optionPropertyIntFontSize_("Font size", "Font size")
    , floatVec2FontPos_("Position", "Position", vec2(0.0f)) {
    addPort(inport_);
    addPort(outport_);
    addProperty(textStringProperty_);
    addProperty(floatColor_);
    addProperty(floatVec2FontPos_);
    floatVec2FontPos_.setMinValue(vec2(0.0f));
    floatVec2FontPos_.setMaxValue(vec2(1.0f));
    addProperty(optionPropertyIntFontSize_);
    optionPropertyIntFontSize_.addOption("10", "10", 10);
    optionPropertyIntFontSize_.addOption("12", "12", 12);
    optionPropertyIntFontSize_.addOption("18", "18", 18);
    optionPropertyIntFontSize_.addOption("24", "24", 24);
    optionPropertyIntFontSize_.addOption("36", "36", 36);
    optionPropertyIntFontSize_.addOption("48", "48", 48);
    optionPropertyIntFontSize_.addOption("60", "60", 60);
    optionPropertyIntFontSize_.addOption("72", "72", 72);
    optionPropertyIntFontSize_.setSelectedIndex(3);
    optionPropertyIntFontSize_.setCurrentStateAsDefault();
}

TextOverlayGL::~TextOverlayGL() {}

void TextOverlayGL::initialize() {
    Processor::initialize();
    copyShader_ = new Shader("img_copy.frag", true);
    textShader_ = new Shader("fontrendering_freetype.vert", "fontrendering_freetype.frag", true);
    int error = 0;

    if (FT_Init_FreeType(&fontlib_)) std::cout << "Major FT error.\n";

    std::string arialfont = InviwoApplication::getPtr()->getPath(InviwoApplication::PATH_MODULES) +
                            "fontrendering/fonts/arial.ttf";
    error = FT_New_Face(fontlib_, arialfont.c_str(), 0, &fontface_);

    if (error == FT_Err_Unknown_File_Format)
        std::cout << "FT2 File opened and read, format unsupported.\n";
    else if (error)
        std::cout << "FT2 Could not read/open the font file.\n";

    glGenTextures(1, &texCharacter_);
    glGenBuffers(1, &vboCharacter_);
}
void TextOverlayGL::deinitialize() {
    delete copyShader_;
    delete textShader_;
    glDeleteTextures(1, &texCharacter_);
    glDeleteBuffers(1, &vboCharacter_);
    Processor::deinitialize();
}

void TextOverlayGL::render_text(const char* text, float x, float y, float sx, float sy,
                                unsigned int unitNumber) {
    const char* p;
    FT_Set_Pixel_Sizes(fontface_, 0, optionPropertyIntFontSize_.get());

    float offset = 0;
    float inputX = x;

    // TODO: To make things more reliable ask the system for proper ascii
    char lf = (char)0xA;  // Line Feed Ascii for std::endl, \n
    char tab = (char)0x9;  // Tab Ascii

    for (p = text; *p; p++) {
        if (FT_Load_Char(fontface_, *p, FT_LOAD_RENDER)) {
            std::cout << "FT could not render char\n";
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
            // p++;
            continue;
        } else if (*p == tab) {
            x += (fontface_->glyph->advance.x >> 6) * sx;
            y += (fontface_->glyph->advance.y >> 6) * sy;
            x += (4 * w);  // 4 times glyph character width
            // p++;
            continue;
        }

        y2 += offset;

        GLfloat box[4][4] = {
            {x2, -y2, 0, 0}, {x2 + w, -y2, 1, 0}, {x2, -y2 - h, 0, 1}, {x2 + w, -y2 - h, 1, 1},
        };
        textShader_->setUniform("tex", (GLint)unitNumber);
        textShader_->setUniform("color", floatColor_.get());
        glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_STREAM_DRAW);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        x += (fontface_->glyph->advance.x >> 6) * sx;
        y += (fontface_->glyph->advance.y >> 6) * sy;
    }
}

void TextOverlayGL::process() {
    utilgl::activateAndClearTarget(outport_);
    TextureUnit colorUnit, depthUnit, pickingUnit;
    utilgl::bindTextures(inport_, colorUnit.getEnum(), depthUnit.getEnum(), pickingUnit.getEnum());

    copyShader_->activate();

    vec2 dim = static_cast<vec2>(outport_.getDimension());
    copyShader_->setUniform("screenDim_", dim);
    copyShader_->setUniform("screenDimRCP_", vec2(1.0f, 1.0f) / dim);

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
    font_size_ = optionPropertyIntFontSize_.getSelectedValue();
    xpos_ = floatVec2FontPos_.get().x * outport_.getData()->getDimension().x;
    ypos_ = floatVec2FontPos_.get().y * outport_.getData()->getDimension().y + float(font_size_);
    textShader_->activate();
    render_text(textStringProperty_.get().c_str(), -1 + xpos_ * sx, 1 - (ypos_)*sy, sx, sy,
                texUnit.getUnitNumber());
    textShader_->deactivate();
    glDisableVertexAttribArray(attribute_location);
    glDisable(GL_BLEND);
    utilgl::deactivateCurrentTarget();
}

}  // namespace
