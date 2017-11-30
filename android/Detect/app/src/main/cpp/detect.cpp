#include <jni.h>
#include <string>
#include "InNetApi.h"
#include "InFactoryApi.h"
#include "InClock.h"
#include "InDefer.h"
#include <memory>
#include <fstream>
#include <vector>
#include <sstream>
#include <android/bitmap.h>

extern "C" {
JNIEXPORT jstring JNICALL Java_com_jiuyan_detect_PrestissimoTestInstance_test(JNIEnv *env, jclass type, int factoryType);
JNIEXPORT jlong JNICALL Java_com_jiuyan_detect_PrestissimoTestInstance_createNetInstance(JNIEnv *env, jclass type_,
                                                                 jint type, jstring modelFileName_);
JNIEXPORT void JNICALL Java_com_jiuyan_detect_PrestissimoTestInstance_destroyNetInstance(JNIEnv *env, jclass type,
        jlong instance);
JNIEXPORT jfloatArray JNICALL Java_com_jiuyan_detect_PrestissimoTestInstance_predict(JNIEnv *env, jclass type, jlong instance, jobject inputImage);

}
JNIEXPORT jstring JNICALL Java_com_jiuyan_detect_PrestissimoTestInstance_test(JNIEnv *env, jclass type, int factoryType)
{
    auto factory = std::shared_ptr<InIFactory>(InFactoryCreate( (FACTORYTYPE)factoryType), [](void *f) {
        InFactoryDestroy((InIFactory *) f);
    });
    std::string testModelPath = "/sdcard/prestissimo/test.prestissimo";
    std::ifstream modelOs(testModelPath);
    if (modelOs.fail())
    {
        FUNC_PRINT(1);
        return env->NewStringUTF("Can't find model");
    }
    modelOs.seekg (0, modelOs.end);
    auto length = modelOs.tellg();
    modelOs.seekg (0, modelOs.beg);
    char * buffer = new char[length];
    InDefer([buffer](){
        delete [] buffer;
    });
    modelOs.read (buffer,length);
    modelOs.close();
    auto net = InNetCreate((const uint8_t *) buffer, length, factory.get());
    if (NULL == net)
    {
        INPRINT("Invalid moel file %s!!!\n", testModelPath.c_str());
    }
    InDefer([net] {
        InNetRelease(net);
    });

    auto input = InNetGetInputImage(net, NULL);
    auto output = InNetGetOutputImage(net, NULL);
    float* inputBuffer = new float[input->getSize()];
    float* outputBuffer = new float[output->getSize()];
    float* outputBufferCheck = new float[output->getSize()];
    InDefer([inputBuffer, outputBuffer, outputBufferCheck]() {
        delete [] inputBuffer;
        delete [] outputBuffer;
        delete [] outputBufferCheck;
    });

    {
        std::ifstream inputOs("/sdcard/prestissimo/input.txt");
        auto inputSize = input->getSize();
        for (int i=0; i<inputSize; ++i)
        {
            inputOs >> inputBuffer[i];
        }
        std::ifstream outputOs("/sdcard/prestissimo/output.txt");
        auto outputSize = output->getSize();
        for (int i=0; i<outputSize; ++i)
        {
            outputOs >> outputBufferCheck[i];
        }
    }

    std::ostringstream report;
    //Check Time
    int times = 10;
    {
        InFuncClock clock([&report, times](float time) {
            report << "Mean Cost time: " << (time/(float)times) << " ms\n";
        });
        for (int t=0; t< times; ++t)
        {
            input->vUpload(inputBuffer);
            InNetRun(net);
            output->vDownload(outputBuffer);
        }
    }
    return env->NewStringUTF(report.str().c_str());
}

/*This kind of instance only support one input and one output*/
class PrestissimoNetInstance
{
public:
    PrestissimoNetInstance(InNet* net, InIFactory* factory)
    {
        INASSERT(NULL!=net);
        INASSERT(NULL!=factory);
        mNet = net;
        mFactory = factory;
        mInputImage = InNetGetInputImage(net, NULL);
        mOutputImage = InNetGetOutputImage(net, NULL);
        mInputBuffer.resize(mInputImage->getSize());
        mOutputBuffer.resize(mOutputImage->getSize());
    }

    const float* getInput () const
    {
        return mInputBuffer.data();
    }

    const float* getOutput() const
    {
        return mOutputBuffer.data();
    }

    size_t inputSize() const
    {
        return mInputBuffer.size();
    }

    size_t outputSize() const
    {
        return mOutputBuffer.size();
    }


    void run()
    {
        mInputImage->vUpload(mInputBuffer.data());
        InNetRun(mNet);
        mOutputImage->vDownload(mOutputBuffer.data());
    }

    ~ PrestissimoNetInstance()
    {
        InNetRelease(mNet);
        InFactoryDestroy(mFactory);
    }

private:
    InNet* mNet;
    InIFactory* mFactory;
    InImage* mInputImage;
    InImage* mOutputImage;

    std::vector<float> mInputBuffer;
    std::vector<float> mOutputBuffer;
};


JNIEXPORT jlong JNICALL
Java_com_jiuyan_detect_PrestissimoTestInstance_createNetInstance(JNIEnv *env, jclass type_,
                                                                 jint factorytype, jstring modelFileName_)
{
    const char *modelFileName = env->GetStringUTFChars(modelFileName_, 0);
    InDefer([&]{
        env->ReleaseStringUTFChars(modelFileName_, modelFileName);
    });
    std::ifstream modelOs(modelFileName);
    if (modelOs.fail())
    {
        INPRINT("Can't open model : %s\n", modelFileName);
        return 0;
    }
    modelOs.seekg (0, modelOs.end);
    auto length = modelOs.tellg();
    modelOs.seekg (0, modelOs.beg);
    auto factory = InFactoryCreate( (FACTORYTYPE)factorytype);
    if (NULL == factory)
    {
        INPRINT("Invalid factory type : %d", factorytype);
        return 0;
    }
    char * buffer = new char[length];
    InDefer([buffer](){
        delete [] buffer;
    });
    modelOs.read (buffer,length);
    modelOs.close();
    auto net = InNetCreate((const uint8_t *) buffer, length, factory);
    if (NULL == net)
    {
        INPRINT("Invalid model file %s!!!\n", modelFileName);
        InFactoryDestroy(factory);
        return 0;
    }
    PrestissimoNetInstance* instance = new PrestissimoNetInstance(net, factory);
    return (long)instance;
}

JNIEXPORT void JNICALL
Java_com_jiuyan_detect_PrestissimoTestInstance_destroyNetInstance(JNIEnv *env, jclass type,
                                                                  jlong instance)
{
    if (0 == instance)
    {
        INPRINT("instance is NULL!!!\n");
        return;
    }
    auto prestissimoNet = (PrestissimoNetInstance*)instance;
    delete prestissimoNet;
}

JNIEXPORT jfloatArray JNICALL Java_com_jiuyan_detect_PrestissimoTestInstance_predict(JNIEnv *env, jclass type, jlong instance, jobject inputImage)
{
    INAUTOTIME;
    auto netInstance = (PrestissimoNetInstance*)instance;
    AndroidBitmapInfo bitmapInfo;
    auto ret = AndroidBitmap_getInfo(env, inputImage, &bitmapInfo);
    INASSERT(227 == bitmapInfo.width && 227==bitmapInfo.height && ANDROID_BITMAP_FORMAT_RGBA_8888 == bitmapInfo.format);
    void* pixels = NULL;
    {
        //INAUTOTIME;
        ret = AndroidBitmap_lockPixels(env, inputImage, &pixels);
        INASSERT(NULL != pixels);
        auto inputData = netInstance->getInput();
        auto bitmapSize = 227 * 227;
        for (int i = 0; i < bitmapSize; ++i)
        {
            auto dst = (float *) inputData + 4 * i;
            auto src = (unsigned char *) pixels + 4 * i;

            //RGBA -> BGRA
            dst[0] = (float) src[2] - 123.f;
            dst[1] = (float) src[1] - 117.f;
            dst[2] = (float) src[0] - 104.f;
            dst[3] = 0.0;
        }

        AndroidBitmap_unlockPixels(env, inputImage);
    }
    {
        //INAUTOTIME;
        netInstance->run();
    }
    jfloatArray outputArray = NULL;
    {
        //INAUTOTIME;
        outputArray = env->NewFloatArray(netInstance->outputSize());
        auto outputJava = env->GetFloatArrayElements(outputArray, NULL);
        ::memcpy(outputJava, netInstance->getOutput(), sizeof(float) * netInstance->outputSize());
        env->ReleaseFloatArrayElements(outputArray, outputJava, 0);
    }
    return outputArray;
}