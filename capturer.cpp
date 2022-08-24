#include "capturer.h"
#include "XTools.h"
#include <iostream>

#include <QImage>
extern"C"
{
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
}
#pragma comment(lib,"avcodec.lib")
#pragma comment(lib,"swscale.lib")
#pragma comment(lib,"avutil.lib")

using namespace std;


D3D_DRIVER_TYPE DriverTypes[] =
{
    D3D_DRIVER_TYPE_HARDWARE,
    D3D_DRIVER_TYPE_WARP,
    D3D_DRIVER_TYPE_REFERENCE
};

D3D_FEATURE_LEVEL FeatureLevels[] =
{
    D3D_FEATURE_LEVEL_11_0,
    D3D_FEATURE_LEVEL_10_1,
    D3D_FEATURE_LEVEL_10_0,
    D3D_FEATURE_LEVEL_9_1
};

ID3D11Device* g_hD3dDevice = nullptr;
ID3D11DeviceContext* g_hD3dDeviceContext = nullptr;
IDXGIOutputDuplication* g_hDeskDupl = nullptr;

static void PrintError(int errcode)
{
    char buffer[1024] = { 0 };
    av_strerror(errcode, buffer, sizeof(buffer) - 1);
    cerr << buffer << endl;
}

bool Capture::CaptureInit()
{
    HRESULT hr = S_OK;
    D3D_FEATURE_LEVEL featureLevel;
    
    // Create D3D device
    for (int i = 0; i < ARRAYSIZE(DriverTypes); i++)
    {
        hr = D3D11CreateDevice(nullptr, DriverTypes[i], nullptr, 0,
            FeatureLevels, ARRAYSIZE(FeatureLevels), D3D11_SDK_VERSION,
            &g_hD3dDevice, &featureLevel, &g_hD3dDeviceContext);
        if (SUCCEEDED(hr))
            break;
    }
    if (FAILED(hr)) return false;

    // Get DXGI device
    IDXGIDevice* dxgiDevice = nullptr;
    hr = g_hD3dDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
    if (FAILED(hr)) return false;

    // Get DXGI adapter
    IDXGIAdapter* dxgiAdapter = nullptr;
    hr = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&dxgiAdapter));
    RESET_OBJECT(dxgiDevice);
    if (FAILED(hr)) return false;

    // Get output
    int output = 0;
    IDXGIOutput* dxgiOutput = nullptr;
    hr = dxgiAdapter->EnumOutputs(output, &dxgiOutput);
    RESET_OBJECT(dxgiAdapter);
    if (FAILED(hr)) return false;

    // get output description struct
    DXGI_OUTPUT_DESC dxgiOutDesc;
    dxgiOutput->GetDesc(&dxgiOutDesc);
    g_Width_ = dxgiOutDesc.DesktopCoordinates.right - dxgiOutDesc.DesktopCoordinates.left;
    g_Height_ = dxgiOutDesc.DesktopCoordinates.bottom - dxgiOutDesc.DesktopCoordinates.top;

    // QI for Output 1
    IDXGIOutput1* dxgiOutput1 = nullptr;
    hr = dxgiOutput->QueryInterface(__uuidof(dxgiOutput1), reinterpret_cast<void**>(&dxgiOutput1));
    RESET_OBJECT(dxgiOutput);
    if (FAILED(hr)) return false;

    // Create desktop duplication
    hr = dxgiOutput1->DuplicateOutput(g_hD3dDevice, &g_hDeskDupl);
    RESET_OBJECT(dxgiOutput1);
    if (FAILED(hr)) return false;

    return true;
}

void Capture::captureUnInit()
{
    RESET_OBJECT(g_hDeskDupl);
    RESET_OBJECT(g_hD3dDeviceContext);
    RESET_OBJECT(g_hD3dDevice);
}

bool attatchToThread()
{
    HDESK hold = GetThreadDesktop(GetCurrentThreadId());
    HDESK currentDesktop = OpenInputDesktop(0, FALSE, GENERIC_ALL);
    if (!currentDesktop)
        return FALSE;

    // Attach desktop to this thread
    BOOL bDesktopAttached = SetThreadDesktop(currentDesktop);
    CloseDesktop(hold);
    CloseDesktop(currentDesktop);
    currentDesktop = nullptr;

    return bDesktopAttached;
}

bool Capture::CaptureImgData(void* pImgData, INT& nImgSize)
{
    if (!attatchToThread())
        return false;

    nImgSize = 0;

    IDXGIResource* hDesktopResource = nullptr;
    DXGI_OUTDUPL_FRAME_INFO FrameInfo;
    HRESULT hr = g_hDeskDupl->AcquireNextFrame(0, &FrameInfo, &hDesktopResource);
    if (FAILED(hr))
        return true;

    // query next frame staging buffer
    ID3D11Texture2D* hAcquiredDesktopImage = nullptr;
    hr = hDesktopResource->QueryInterface(__uuidof(ID3D11Texture2D),
        reinterpret_cast<void**>(&hAcquiredDesktopImage));
    RESET_OBJECT(hDesktopResource);
    if (FAILED(hr))
        return FALSE;

    // copy old description
    D3D11_TEXTURE2D_DESC frameDescriptor;
    hAcquiredDesktopImage->GetDesc(&frameDescriptor);

    // create a new staging buffer for fill frame image
    ID3D11Texture2D* hNewDesktopImage = nullptr;
    frameDescriptor.Usage = D3D11_USAGE_STAGING;
    frameDescriptor.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    frameDescriptor.BindFlags = 0;
    frameDescriptor.MiscFlags = 0;
    frameDescriptor.MipLevels = 1;
    frameDescriptor.ArraySize = 1;
    frameDescriptor.SampleDesc.Count = 1;
    hr = g_hD3dDevice->CreateTexture2D(&frameDescriptor, nullptr, &hNewDesktopImage);
    if (FAILED(hr))
    {
        RESET_OBJECT(hAcquiredDesktopImage);
        g_hDeskDupl->ReleaseFrame();
        return false;
    }

    // copy next staging buffer to new staging buffer
    g_hD3dDeviceContext->CopyResource(hNewDesktopImage, hAcquiredDesktopImage);
    RESET_OBJECT(hAcquiredDesktopImage);
    g_hDeskDupl->ReleaseFrame();

    // create staging buffer for map bits
    IDXGISurface* hStagingSurf = nullptr;
    hr = hNewDesktopImage->QueryInterface(__uuidof(IDXGISurface), (void**)(&hStagingSurf));
    RESET_OBJECT(hNewDesktopImage);
    if (FAILED(hr))
        return false;

    DXGI_SURFACE_DESC hStagingSurfDesc;
    hStagingSurf->GetDesc(&hStagingSurfDesc);

    // copy bits to user space
    DXGI_MAPPED_RECT mappedRect;
    hr = hStagingSurf->Map(&mappedRect, DXGI_MAP_READ);
    int imgSize = g_Width_ * g_Height_ * 4;
    if (SUCCEEDED(hr))
    {
        nImgSize = imgSize;
        memcpy(pImgData, mappedRect.pBits, imgSize);
        hStagingSurf->Unmap();
    }

    RESET_OBJECT(hStagingSurf);
    return SUCCEEDED(hr);
}

void Capture::CaptureRotatelImg(unsigned char* pImgData)
{
    unsigned int* pImg32 = (unsigned int*)pImgData;
    int iCount = g_Width_ * g_Height_;
    unsigned int* pRotateImg32 = new unsigned int[iCount];
    for (int i = 0; i < iCount; ++i) {
        pRotateImg32[i] = pImg32[iCount - i - 1];
    }

    unsigned int* pRotateImg32_r = (unsigned int*)pImgData;
    for (int i = 0; i < g_Height_; ++i) {
        for (int j = 0; j < g_Width_; ++j) {
            pRotateImg32_r[j + i * g_Width_] = pRotateImg32[g_Width_ - j - 1 + i * g_Width_];
        }
    }
    delete[] pRotateImg32;
}

void Capture::CaptureSaveBmpFile(const char* fileName, unsigned char* pImgData, int imgLength)
{
    BITMAPFILEHEADER bmheader;
    memset(&bmheader, 0, sizeof(bmheader));
    bmheader.bfType = 0x4d42;     //图像格式。必须为'BM'格式。  
    bmheader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER); //从文件开头到数据的偏移量  
    bmheader.bfSize = imgLength + bmheader.bfOffBits;//文件大小  

    BITMAPINFOHEADER bmInfo;
    memset(&bmInfo, 0, sizeof(bmInfo));
    bmInfo.biSize = sizeof(bmInfo);
    bmInfo.biWidth = g_Width_;
    bmInfo.biHeight = g_Height_;
    bmInfo.biPlanes = 1;
    bmInfo.biBitCount = 32;
    bmInfo.biCompression = BI_RGB;

    HANDLE hFile = CreateFileA(fileName, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
    if (hFile != INVALID_HANDLE_VALUE) {
        DWORD dwWritten;
        BOOL bRet = WriteFile(hFile, &bmheader, sizeof(BITMAPFILEHEADER), &dwWritten, nullptr);
        //assert(TRUE == bRet);
        bRet = WriteFile(hFile, &bmInfo, sizeof(BITMAPINFOHEADER), &dwWritten, nullptr);
        //assert(TRUE == bRet);
        bRet = WriteFile(hFile, pImgData, imgLength, &dwWritten, nullptr);
        //assert(TRUE == bRet);
        CloseHandle(hFile);
    }
}


bool Capture::ScaleInit()
{
    unique_lock<mutex> lock(mtx_);
    if (out_Width_ < 0 || out_Height_ < 0 || g_Width_ < 0 || g_Height_ < 0)
    {
        LOGERROR("(out_Width_ < 0 || out_Height_ < 0 || g_Width_ < 0 || g_Height_ < 0)")
            return false;
    }
    plinesize_ = new int [AV_NUM_DATA_POINTERS];
    memset(plinesize_, 0, sizeof(plinesize_));
    pImgData_ = new unsigned char[g_Width_ * g_Height_ * 4];
    swsCtx_ = sws_getCachedContext(swsCtx_, g_Width_, g_Height_, AV_PIX_FMT_BGRA,
        out_Width_, out_Height_, AV_PIX_FMT_YUV420P,
        SWS_BILINEAR,
        0, 0, 0);
    if (!swsCtx_)
    {
        LOGERROR("sws_getCachedContext failed")
            return false;
    }
    return true;
}

int Capture::getInputWidth()
{
    return g_Width_;
}
int Capture::getInputHeight()
{
    return g_Height_;
}
void Capture::setOutputSize(int width, int height)
{
    unique_lock<mutex> lock(mtx_);
    out_Width_ = width;
    out_Height_ = height;
}

////////////////////////////////////////////////////////////////////////////
//调用者释放空间
AVFrame* Capture::ImgDataScale(void* data, int* linesize)
{
    unique_lock<mutex> lock(mtx_);
    if (!swsCtx_ || !data || linesize[0] < 0)return nullptr;

    uint8_t* inData[AV_NUM_DATA_POINTERS] = { 0 };
    inData[0] = (uint8_t*)data;
    //uint8_t* inData = (uint8_t*)data;

    AVFrame* outFrame = av_frame_alloc();
    outFrame->width = out_Width_;
    outFrame->height = out_Height_;
    outFrame->format = AV_PIX_FMT_YUV420P;
    outFrame->pts = 0;

    int ret = av_frame_get_buffer(outFrame, 0);
    if (ret != 0)
    {
        av_frame_free(&outFrame);
        PrintError(ret);
        return nullptr;
    }

    int lsz = (g_Width_ * 4);
    ret = sws_scale(swsCtx_, inData, &lsz, 0, g_Height_, outFrame->data, outFrame->linesize);
    if (ret <= 0)
    {
        av_frame_free(&outFrame);
        PrintError(ret);
        return nullptr;
    }
    return outFrame;
}

////////////////////////////////////////////////////////////////////////////
//调用者释放空间
AVFrame* Capture::GetCapturedFrame()
{
    int linesize[AV_NUM_DATA_POINTERS] = { 0 };
    linesize[0] = g_Width_ * 4;
    bool success = CaptureImgData(pImgData_, plinesize_[0]);
    if (!success)return nullptr;
    return ImgDataScale(pImgData_, linesize);
}

unsigned char* Capture::getImageData()
{
    return pImgData_;
}

int Capture::getImageSize()
{
    return g_Width_ * g_Height_ * 4;
}