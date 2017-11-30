#ifndef INIMAGE3D_H
#define INIMAGE3D_H
#include <stdint.h>
#include <stdlib.h>
class InImage
{
public:
    InImage(uint32_t w, uint32_t h, uint32_t depth, uint32_t batch)
    {
        mWidth = w;
        mHeight = h;
        mDepth = depth;
        mBatch = batch;
    }
    virtual ~ InImage()
    {
    }

    inline uint32_t width() const {return mWidth;}
    inline uint32_t height() const {return mHeight;}
    inline uint32_t depth() const {return mDepth;}
    inline uint32_t batch() const {return mBatch;}

    inline uint64_t getSize() const { return mBatch*mWidth*mHeight*((mDepth+3)/4*4);}
    inline uint64_t getImageSize() const {return mWidth*mHeight*((mDepth+3)/4*4);}

    void resize(uint32_t w, uint32_t h, uint32_t d, uint32_t batch)
    {
        mWidth = w;
        mHeight = h;
        mDepth = d;
        mBatch = batch;
        this->vResize();
    }

    virtual void vWait() {}
    
    virtual void vUpload(const float* src) = 0;
    virtual void vDownload(float* dst) = 0;


protected:
    virtual void vResize() = 0;
private:
    uint32_t mWidth;
    uint32_t mHeight;
    uint32_t mDepth;
    uint32_t mBatch;
};
#endif
