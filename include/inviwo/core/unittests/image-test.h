#ifndef IVW_IMAGE_TEST_H
#define IVW_IMAGE_TEST_H

#include <inviwo/core/datastructures/image/image.h>
#include <inviwo/core/datastructures/image/layerdisk.h>
#include "../datastructures/image/layerramprecision.h"


#define IMG_RGB InviwoApplication::getPtr()->getPath(InviwoApplication::PATH_MODULES,"unittests/testdata/images/2x2.bmp")
#define IMG_WHITE InviwoApplication::getPtr()->getPath(InviwoApplication::PATH_MODULES,"unittests/testdata/images/white.bmp")
#define IMG_RANGE InviwoApplication::getPtr()->getPath(InviwoApplication::PATH_MODULES,"unittests/testdata/images/range.bmp")

TEST(ImageTests,ImageLoadWhite){
    Image img;
    img.getColorLayer()->addRepresentation(new LayerDisk(IMG_WHITE));
    const LayerRAM *layer = img.getColorLayer()->getRepresentation<LayerRAM>();

    ASSERT_TRUE(layer!=0);

    uvec2 dim = layer->getDimension();

    EXPECT_EQ(dim.x,2);
    EXPECT_EQ(dim.y,2);

    vec4 a = layer->getValueAsVec4Float(uvec2(0,0));
    vec4 b = layer->getValueAsVec4Float(uvec2(0,1));
    vec4 c = layer->getValueAsVec4Float(uvec2(1,0));
    vec4 d = layer->getValueAsVec4Float(uvec2(1,1));

    EXPECT_FLOAT_EQ(a.r, 1.0f);
    EXPECT_FLOAT_EQ(a.g, 1.0f);
    EXPECT_FLOAT_EQ(a.b, 1.0f);
    EXPECT_FLOAT_EQ(a.a, 1.0f);

    EXPECT_FLOAT_EQ(b.r, 1.0f);
    EXPECT_FLOAT_EQ(b.g, 1.0f);
    EXPECT_FLOAT_EQ(b.b, 1.0f);
    EXPECT_FLOAT_EQ(b.a, 1.0f);

    EXPECT_FLOAT_EQ(c.r, 1.0f);
    EXPECT_FLOAT_EQ(c.g, 1.0f);
    EXPECT_FLOAT_EQ(c.b, 1.0f);
    EXPECT_FLOAT_EQ(c.a, 1.0f);


    EXPECT_FLOAT_EQ(d.r, 1.0f);
    EXPECT_FLOAT_EQ(d.g, 1.0f);
    EXPECT_FLOAT_EQ(d.b, 1.0f);
    EXPECT_FLOAT_EQ(d.a, 1.0f);
}


TEST(ImageTests,ImageLoadRGB){
    Image img;
    img.getColorLayer()->addRepresentation(new LayerDisk(IMG_RGB));
    const LayerRAM *layer = img.getColorLayer()->getRepresentation<LayerRAM>();

    ASSERT_TRUE(layer!=0);

    uvec2 dim = layer->getDimension();

    EXPECT_EQ(dim.x,2);
    EXPECT_EQ(dim.y,2);

    vec4 a = layer->getValueAsVec4Float(uvec2(0,0));
    vec4 b = layer->getValueAsVec4Float(uvec2(1,0));
    vec4 c = layer->getValueAsVec4Float(uvec2(0,1));
    vec4 d = layer->getValueAsVec4Float(uvec2(1,1));

    EXPECT_FLOAT_EQ(a.r, 1.0f);
    EXPECT_FLOAT_EQ(a.g, 0.0f);
    EXPECT_FLOAT_EQ(a.b, 0.0f);
    EXPECT_FLOAT_EQ(a.a, 1.0f);

    EXPECT_FLOAT_EQ(b.r, 0.0f);
    EXPECT_FLOAT_EQ(b.g, 1.0f);
    EXPECT_FLOAT_EQ(b.b, 0.0f);
    EXPECT_FLOAT_EQ(b.a, 1.0f);

    EXPECT_FLOAT_EQ(c.r, 0.0f);
    EXPECT_FLOAT_EQ(c.g, 0.0f);
    EXPECT_FLOAT_EQ(c.b, 1.0f);
    EXPECT_FLOAT_EQ(c.a, 1.0f);


    EXPECT_FLOAT_EQ(d.r, 50/255.0f);
    EXPECT_FLOAT_EQ(d.g, 100/255.0f);
    EXPECT_FLOAT_EQ(d.b, 150/255.0f);
    EXPECT_FLOAT_EQ(d.a, 1.0f);
}



TEST(ImageTests,ImageLoadRange){
    Image img;
    img.getColorLayer()->addRepresentation(new LayerDisk(IMG_RANGE));
    const LayerRAM *layer = img.getColorLayer()->getRepresentation<LayerRAM>();

    ASSERT_TRUE(layer!=0);

    uvec2 dim = layer->getDimension();

    EXPECT_EQ(dim.x,256);
    EXPECT_EQ(dim.y,1);

    for(int i = 0;i<255;i++){
        vec4 a = layer->getValueAsVec4Float(uvec2(i,0));
        EXPECT_FLOAT_EQ(a.r, i/255.0f);
        EXPECT_FLOAT_EQ(a.g, i/255.0f);
        EXPECT_FLOAT_EQ(a.b, i/255.0f);

        EXPECT_FLOAT_EQ(a.a, 1.0f);
    }
}




#endif