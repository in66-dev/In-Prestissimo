#ifndef UTILS_InCLOCK_H
#define UTILS_InCLOCK_H
/******************************************************************
 Copyright 2013, Jiang Xiao-tang
 
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 
 http://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 ******************************************************************/
#include <time.h>
#include <string>
#include <sys/time.h>
#include <functional>
#include "InDebug.h"

class InClock
{
public:
    InClock(const char* name, int line)
    {
        gettimeofday(&mSta, NULL);
        mName = name;
        mLine = line;
    }
    ~InClock()
    {
        gettimeofday(&mFin, NULL);
        float inter_ms = (mFin.tv_usec-mSta.tv_usec)/1000.0f + (mFin.tv_sec-mSta.tv_sec)*1000.0f;
        INPRINT("For %s, %d: Time is %.3fms\n", mName.c_str(), mLine, inter_ms);
    }
protected:
    timeval mSta;
    timeval mFin;
    std::string mName;
    int mLine;
};

class InFuncClock
{
public:
    InFuncClock(std::function<void(float)> output)
    {
        gettimeofday(&mSta, NULL);
        mFunc = std::move(output);
    }
    ~ InFuncClock()
    {
        gettimeofday(&mFin, NULL);
        float inter_ms = (mFin.tv_usec-mSta.tv_usec)/1000.0f + (mFin.tv_sec-mSta.tv_sec)*1000.0f;
        mFunc(inter_ms);
    }

private:
    timeval mSta;
    timeval mFin;
    std::function<void(float)> mFunc;
};


#define INAUTOTIME InClock __clock(__func__,__LINE__)
#endif
