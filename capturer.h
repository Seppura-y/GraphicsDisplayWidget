#pragma once
#include <d3d11.h>
#include <dxgi1_2.h>
#include <mutex>

#pragma comment(lib, "D3D11.lib")
#pragma comment(lib, "DXGI.lib")

#define RESET_OBJECT(x) {if (x) x->Release(); x = nullptr;}

struct AVFrame;
struct SwsContext;
class Capture
{
public:

	bool CaptureInit();
	bool ScaleInit();

	bool CaptureImgData(void* pImgData, INT& nImgSize);
	AVFrame* ImgDataScale(void* data, int* linesize);
	AVFrame* GetCapturedFrame();

	void CaptureRotatelImg(unsigned char* pImgData);
	void CaptureSaveBmpFile(const char* fileName, unsigned char* pImgData, int imgLength);

	int getInputWidth();
	int getInputHeight();
	void setOutputSize(int width,int height);

private:
	int g_Width_ = -1;
	int g_Height_ = -1;
	int out_Width_ = -1;
	int out_Height_ = -1;
	SwsContext* swsCtx_ = nullptr;
	unsigned char* pImgData_ = nullptr;
	int* plinesize_ = nullptr;
	std::mutex mtx_;
};
