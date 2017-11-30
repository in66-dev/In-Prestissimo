#ifndef INCLUDE_INFACTORYAPI_H
#define INCLUDE_INFACTORYAPI_H
#ifndef IN_PUBLIC_API
#define IN_PUBLIC_API __attribute__((visibility("default")))
#endif

typedef enum {
    //ST: single thread, MT: multi thread
    //FLOAT: Use Float as Inter Format, Slow but accurate
    //INT: Fast but sometimes may be not accurate
    //GPU_FLOAT: If use gpu, must use set_gpu_info.out to add gpu shader to model firstly
    
    CPU_FLOAT_ST=0,
    CPU_FLOAT_MT,
    CPU_INT_ST,
    CPU_INT_MT,
    GPU_FLOAT
} FACTORYTYPE;

class InIFactory;
IN_PUBLIC_API InIFactory* InFactoryCreate(FACTORYTYPE type);

IN_PUBLIC_API void InFactoryDestroy(InIFactory* factory);

#endif
