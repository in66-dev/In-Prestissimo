#ifndef INNETAPI_H
#define INNETAPI_H
#include "InImage.h"
#include "InFactoryApi.h"

class InIFactory;
class InNet;

/*Create net, a net contain a lot of operators and mid images, which created by factory*/
IN_PUBLIC_API InNet* InNetCreate(const uint8_t* netInfo, size_t length, InIFactory* factory);

/*Destroy net, the net will use the factory created it to release operators and images
 Must release all net before destorying the factory
 */
IN_PUBLIC_API void InNetRelease(InNet* net);

/*Run the net for compute, must be called after the input images' data are all uploaded*/
IN_PUBLIC_API bool InNetRun(InNet* net);

/*Get the input image of net, then you can upload data or resize it
 The name is the layer name in prototxt, if NULL, then return the first input
 Example:
 auto input = InNetGetInputImage(net, "data");
 float* value = new float[input->getSize()];
 //....... Fill the value
 input->vUpload(value);
 
 delete [] value;
 */
IN_PUBLIC_API InImage* InNetGetInputImage(InNet* net, const char* name);


/*Get the output image of net, then you can download data from it
 The name is the layer name in prototxt, if NULL, then return the first output
 Don't resize or upload data to output */
/* Example:
 auto output = InNetGetOutputImage(net, "conv1");
 float* value = new float[output->getSize()];
 output->vDownload(value);
 
 //....... (Use the value do something)
 
 delete [] value; */
IN_PUBLIC_API InImage* InNetGetOutputImage(InNet* net, const char* name);

/*After setting all size of input images, call this function to resize total net*/
/*
 Example:
 auto input1 = InNetGetInputImage(net, "data1");
 auto input2 = InNetGetInputImage(net, "data2");
 input1->resize(281, 281, 3, 1);
 input2->resize(114, 222, 18, 1);
 InNetResize(net);
 */
IN_PUBLIC_API void InNetResize(InNet* net);


/*API For Debug Begin*/
/*The Debug API Don't Support Multit-Thread and GPU mode*/

/*Get the pos' output image of the layer, the name can't be null.
 Mainly used for get the value of mid image*/
IN_PUBLIC_API InImage* InNetGetImage(InNet* net, const char* name, int pos);

/*Compare all the images in net1 and net2, print the incorrect layer*/
/*Example:
 auto net1 = InNetCreate(buffer, bufferSize, factory1);
 auto net2 = InNetCreate(buffer, bufferSize, factory2);

 auto input1 = InNetGetInputImage(net1, "conv1");
 auto input2 = InNetGetInputImage(net2, "conv1");
 float* value = new float[input1->getSize()];
 //....... Fill the value
 input1->vUpload(value);
 input2->vUpload(value);
 delete [] value;
 
 InNetRun(net1);
 InNetRun(net2);
 
 InNetCompare(net1, net2);
 */
IN_PUBLIC_API bool InNetCompare(InNet* net1, InNet* net2);

/*Use the count the time for all layers
 * InNetDumpTrace will print all layer's cost time
 * */
/*Example:
 InNetOpenTrace(net);
 for (int i=0; i<100; ++i)
 {
     InNetRun(net);
 }
 InNetDumpTrace(net);
 */
IN_PUBLIC_API void InNetOpenTrace(InNet* net);
IN_PUBLIC_API void InNetDumpTrace(InNet* net);

/*Dump All Image Info to One Directory
 If the Layer's name has '/', it will be replaced by '_'*/
IN_PUBLIC_API void InNetDumpAllImages(InNet* net, const char* dir);

/*Load All Image Data from the Directory, if file not exist will skip,
 the file name is the same as InNetDumpAllImages*/
IN_PUBLIC_API void InNetReadAllImages(InNet* net, const char* dir);

/*Only Compute One Layer*/
IN_PUBLIC_API bool InNetRunLayer(InNet* net, const char* layerName);

/*API For Debug End*/


#endif
