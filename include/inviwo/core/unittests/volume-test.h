#include <inviwo/core/datastructures/volume/volume.h>
#include <inviwo/core/datastructures/volume/volumeram.h>
#include <inviwo/core/resources/resourcemanager.h>
#include <inviwo/core/io/datareaderfactory.h>
#include <inviwo/core/resources/templateresource.h>
#include <inviwo/core/datastructures/volume/volumeramprecision.h>
#include <math.h>

#ifndef IVW_VOLUME_TEST_H
#define IVW_VOLUME_TEST_H

template<typename T>
void testDatReader(std::string filename) {
    std::string file = InviwoApplication::getPtr()->getPath(InviwoApplication::PATH_MODULES)
                       + "unittests/testdata/" + filename;
    std::string fileExtension = URLParser::getFileExtension(file);
    DataReaderType<Volume>* reader =
        DataReaderFactory::getRef().getReaderForTypeAndExtension<Volume>(fileExtension);
    ASSERT_TRUE(reader != NULL);
    Volume* volume = reader->readMetaData(file);
    const VolumeRAMPrecision<T>* volumeRAM =
        static_cast<const VolumeRAMPrecision<T>*>(volume->getRepresentation<VolumeRAM>());
    const T* data = static_cast<const T*>(volumeRAM->getData());
    uvec3 dim = volume->getDimension();
    long long ref0;
    double ref1;
    double ref2;
    double ref3;
    double val;
    long long ty;
    long long tz;
    double mod = static_cast<double>(std::numeric_limits<T>::max())
                 - static_cast<double>(std::numeric_limits<T>::min());
    double min = static_cast<double>(std::numeric_limits<T>::min());

    for (long long z = 0; z < dim.z; z++) {
        tz = z*z*z*z*z*z*z*z;

        for (long long y = 0; y < dim.y; y++) {
            ty = y*y*y*y;

            for (long long x = 0; x < dim.x; x++) {
                ref0 = x + x*ty + x*ty*tz;
                ref1 = static_cast<double>(ref0);
                ref2 = fmod(ref1, mod);
                ref3 = ref2 + min;
                val = static_cast<double>(data[x + (y*dim.x) + (z*dim.x *dim.y)]);
                EXPECT_EQ(ref3, val);
            }
        }
    }

    delete volume;
}


// Test the .dat reader

TEST(VolumeTest, DatReaderLoadTypeUINT8) {
    testDatReader<unsigned char>("testdata.UINT8.LittleEndian.dat");
}
TEST(VolumeTest, DatReaderLoadTypeUINT16) {
    testDatReader<unsigned short>("testdata.UINT16.LittleEndian.dat");
}
TEST(VolumeTest, DatReaderLoadTypeUINT32) {
    testDatReader<unsigned int>("testdata.UINT32.LittleEndian.dat");
}
TEST(VolumeTest, DatReaderLoadTypeUINT64) {
    testDatReader<unsigned long long>("testdata.UINT64.LittleEndian.dat");
}

TEST(VolumeTest, DatReaderLoadTypeINT8) {
    testDatReader<char>("testdata.INT8.LittleEndian.dat");
}
TEST(VolumeTest, DatReaderLoadTypeINT16) {
    testDatReader<short>("testdata.INT16.LittleEndian.dat");
}
TEST(VolumeTest, DatReaderLoadTypeINT32) {
    testDatReader<int>("testdata.INT32.LittleEndian.dat");
}
TEST(VolumeTest, DatReaderLoadTypeINT64) {
    testDatReader<long long>("testdata.INT64.LittleEndian.dat");
}

TEST(VolumeTest, DatReaderLoadTypeUINT8BigEndian) {
    testDatReader<unsigned char>("testdata.UINT8.BigEndian.dat");
}
TEST(VolumeTest, DatReaderLoadTypeUINT16BigEndian) {
    testDatReader<unsigned short>("testdata.UINT16.BigEndian.dat");
}
TEST(VolumeTest, DatReaderLoadTypeUINT32BigEndian) {
    testDatReader<unsigned int>("testdata.UINT32.BigEndian.dat");
}
TEST(VolumeTest, DatReaderLoadTypeUINT64BigEndian) {
    testDatReader<unsigned long long>("testdata.UINT64.BigEndian.dat");
}

TEST(VolumeTest, DatReaderLoadTypeINT8BigEndian) {
    testDatReader<char>("testdata.INT8.BigEndian.dat");
}
TEST(VolumeTest, DatReaderLoadTypeINT16BigEndian) {
    testDatReader<short>("testdata.INT16.BigEndian.dat");
}
TEST(VolumeTest, DatReaderLoadTypeINT32BigEndian) {
    testDatReader<int>("testdata.INT32.BigEndian.dat");
}
TEST(VolumeTest, DatReaderLoadTypeINT64BigEndian) {
    testDatReader<long long>("testdata.INT64.BigEndian.dat");
}


// Test the .ivf reader

TEST(VolumeTest, IvfReaderLoadTypeUINT8) {
    testDatReader<unsigned char>("testdata.UINT8.LittleEndian.ivf");
}
TEST(VolumeTest, IvfReaderLoadTypeUINT16) {
    testDatReader<unsigned short>("testdata.UINT16.LittleEndian.ivf");
}
TEST(VolumeTest, IvfReaderLoadTypeUINT32) {
    testDatReader<unsigned int>("testdata.UINT32.LittleEndian.ivf");
}
TEST(VolumeTest, IvfReaderLoadTypeUINT64) {
    testDatReader<unsigned long long>("testdata.UINT64.LittleEndian.ivf");
}

TEST(VolumeTest, IvfReaderLoadTypeINT8) {
    testDatReader<char>("testdata.INT8.LittleEndian.ivf");
}
TEST(VolumeTest, IvfReaderLoadTypeINT16) {
    testDatReader<short>("testdata.INT16.LittleEndian.ivf");
}
TEST(VolumeTest, IvfReaderLoadTypeINT32) {
    testDatReader<int>("testdata.INT32.LittleEndian.ivf");
}
TEST(VolumeTest, IvfReaderLoadTypeINT64) {
    testDatReader<long long>("testdata.INT64.LittleEndian.ivf");
}

TEST(VolumeTest, IvfReaderLoadTypeUINT8BigEndian) {
    testDatReader<unsigned char>("testdata.UINT8.BigEndian.ivf");
}
TEST(VolumeTest, IvfReaderLoadTypeUINT16BigEndian) {
    testDatReader<unsigned short>("testdata.UINT16.BigEndian.ivf");
}
TEST(VolumeTest, IvfReaderLoadTypeUINT32BigEndian) {
    testDatReader<unsigned int>("testdata.UINT32.BigEndian.ivf");
}
TEST(VolumeTest, IvfReaderLoadTypeUINT64BigEndian) {
    testDatReader<unsigned long long>("testdata.UINT64.BigEndian.ivf");
}

TEST(VolumeTest, IvfReaderLoadTypeINT8BigEndian) {
    testDatReader<char>("testdata.INT8.BigEndian.ivf");
}
TEST(VolumeTest, IvfReaderLoadTypeINT16BigEndian) {
    testDatReader<short>("testdata.INT16.BigEndian.ivf");
}
TEST(VolumeTest, IvfReaderLoadTypeINT32BigEndian) {
    testDatReader<int>("testdata.INT32.BigEndian.ivf");
}
TEST(VolumeTest, IvfReaderLoadTypeINT64BigEndian) {
    testDatReader<long long>("testdata.INT64.BigEndian.ivf");
}
#endif