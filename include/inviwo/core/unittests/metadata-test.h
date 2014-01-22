#include <inviwo/core/metadata/metadataowner.h>
#include <inviwo/core/metadata/metadata.h>
#include <inviwo/core/io/serialization/ivwserializable.h>
#include <string.h>

#ifndef IVW_METADATA_TEST_H
#define IVW_METADATA_TEST_H


template<typename T, typename M>
void testserialization(T def, T in) {
    T indata = in;
    T outdata1 = def;

    std::string filename =
        InviwoApplication::getPtr()->getPath(InviwoApplication::PATH_MODULES,
        "unittests/tmpfiles/metadatatests.xml");

    MetaDataOwner* mdo1;
    MetaDataOwner* mdo2;
    mdo1 = new MetaDataOwner();
    mdo2 = new MetaDataOwner();


    mdo1->typename setMetaData<M>("data", indata);
    outdata1 = mdo1->typename getMetaData<M>("data", outdata1);
    EXPECT_EQ(indata, outdata1);

    IvwSerializer serializer(filename);
    mdo1->getMetaDataMap()->serialize(serializer);
    serializer.writeFile();

    IvwDeserializer deserializer(filename);
    mdo2->getMetaDataMap()->deserialize(deserializer);

    T outdata2 = def;
    outdata2 = mdo2->typename getMetaData<M>("data", outdata2);
    EXPECT_EQ(indata, outdata2);

    delete mdo1;
    delete mdo2;
}


#define MetaDataMacro(n, t, d, v) \
    TEST(MetaDataTest, n##SerializationTest) { \
        testserialization<t, n##MetaData>(d, v); \
    }
#include <inviwo/core/metadata/metadatadefinefunc.h>

#endif