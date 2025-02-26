#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "imgui.h"

#define VLN_ASSERTIONS_LOG_ENGINE 0
#include "Common/Platform/Assert.h"
#include "Common/Types/Common.h"
#include "Common/Types/String.h"
#include "Common/Math/Vector.h"

namespace Volition
{

namespace EReturnCode
{
    enum Type : i32f
    {
        Success = 0,
        Error   = 1
    };
}

class VLauncher
{
public:
    inline static VLauncher* Instance = nullptr;

    u32 ResizeWidth = 0;
    u32 ResizeHeight = 0;

private:
    struct ID3D11Device*           D3DDevice = nullptr;
    struct ID3D11DeviceContext*    D3DDeviceContext = nullptr;
    struct IDXGISwapChain*         SwapChain = nullptr;
    struct ID3D11RenderTargetView* MainRenderTargetView = nullptr;

    WNDCLASSEXW WindowClass;
    HWND hWnd;

    VWideString WindowTitle;
    VVector2i WindowSize;

    b32 bRunning;

public:
    VLauncher(const VWideString& Title = L"Launcher", const VVector2i& Size = { 1280, 720 })
    {
        VLN_ASSERT(!Instance);
        Instance = this;

        WindowTitle = Title;
        WindowSize = Size;
    }

    virtual ~VLauncher() = default;

    EReturnCode::Type Run();

protected:
    VLN_FINLINE void Stop()
    {
        bRunning = false;
    }

private:
    b32 StartUp();
    void ShutDown();

    void ProcessEvents();
    virtual void Update() {}
    void Render();

    b32 StartUpD3D(HWND hWnd);
    void ShutDownD3D();

    void CreateRenderTarget();
    void DestroyRenderTarget();
};

}
