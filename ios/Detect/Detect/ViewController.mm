//
//  ViewController.m
//  Detect
//
//  Created by jiangxiaotang on 23/11/2017.
//  Copyright © 2017 jiuyan. All rights reserved.
//

#import "ViewController.h"

@interface ViewController ()

@end

#include "InNetApi.h"
#include "InFactoryApi.h"
#include "InImage.h"
#include "InDebug.h"
#include "InDefer.h"
#include <memory>
#include <fstream>
#include <vector>
#include "InClock.h"
#include <stdlib.h>

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    
    auto factory = InFactoryCreate(CPU_INT_ST);
    {
        NSString* nsPath = [NSString stringWithUTF8String:"squeezenet_v1.1.prestissimo"];
        NSString* realPath = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:nsPath];
        std::ifstream modelOs([realPath UTF8String]);
        if (modelOs.fail())
        {
            FUNC_PRINT(1);
            return;
        }
        modelOs.seekg (0, modelOs.end);
        size_t length = modelOs.tellg();
        modelOs.seekg (0, modelOs.beg);
        char * buffer = new char[length];
        InDefer([buffer](){
            delete [] buffer;
        });
        modelOs.read (buffer,length);
        modelOs.close();
        std::shared_ptr<InNet> net = std::shared_ptr<InNet>(InNetCreate((const uint8_t*)buffer, length, factory), [](void* n){
            InNetRelease((InNet*)n);
        });
        auto inputImage = InNetGetInputImage(net.get(), NULL);
        auto outputImage = InNetGetOutputImage(net.get(), NULL);
        
        std::vector<float> inputBuffer(inputImage->getSize());
        std::vector<float> outputBuffer(outputImage->getSize());
        for (int v=0; v<inputBuffer.size(); ++v)
        {
            inputBuffer[v] = (rand() % 255);
        }
        
        {
            INAUTOTIME;
            for (int i=0; i<100; ++i)
            {
                inputImage->vUpload(inputBuffer.data());
                InNetRun(net.get());
                outputImage->vDownload(outputBuffer.data());
            }
        }
        
        

    }
    
    InFactoryDestroy(factory);
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


@end
