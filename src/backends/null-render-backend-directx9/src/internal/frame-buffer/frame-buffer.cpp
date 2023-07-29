#include <null-render.h>

#include <internal/frame-buffer/frame-buffer.h>

/* 
* @note:
*  � ������� ��� ��� �� ���� � ���� ��������� directx9.
*  �� ����� ����� �� ��������, ��� �������� ������ ������������ �������� ��������� ����� ��������� rt � ��������� � �� ����-����.
*  ����� ���� ��� ������ �������� � ������������� �������� ��� ����� ������, ��������� � ��������� ������ ����� �� ��������,
*  ��� ������� ������, �� � SetTexture, �� � DrawIndexedPrimitive.
*  ���� ������ msaa � msaa_buffer �� �� ����� ���������.
*  � ��� ����� � ��� ��������, ����� blend ��� �����, ����� ��� ���-��, �� ������� ��� ������ ������� �� ��� ������ � �� ���������,
*  ������������ �� ����� ��������, 2 �������� � �� �������� � rt, ��������� �������� ����� ������������, ��������� �������� ����� ���������� rt, ����� �� ��������.
*
*  ���� ���-�� ����� ��������� ��� ����� � ���� � ���� � ��� ��� ��������, ����� � � ��������� ����� ��� ���� ������� ����� � ����� �� ��� �������.
*/

namespace null::render::backend::directx9 {
    void c_frame_buffer::on_create() {
        if(!empty()) return;

        //@note: creating texture for render target
        if(type == e_frame_buffer_type::postprocessing) {
            IDirect3DSurface9* main_surface{ };
            shared.device->GetRenderTarget(0, &main_surface);

            D3DSURFACE_DESC main_surface_desc{ };
            main_surface->GetDesc(&main_surface_desc);

            if(auto result{ shared.device->CreateTexture(size.x, size.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &texture, nullptr) }; FAILED(result)) {
                utils::logger(utils::e_log_type::error, "CreateTexture2D failed, return code {}.", result);
                return;
            }

			if(flags & e_frame_buffer_flags::msaa ) {
                D3DMULTISAMPLE_TYPE multisample_type{ D3DMULTISAMPLE_NONE };
                if(flags & e_frame_buffer_flags::msaa && shared::msaa_quality != 1) {
                    multisample_type = (D3DMULTISAMPLE_TYPE)shared::msaa_quality;
                }

                //@note: creating render target
                if(auto result{ shared.device->CreateRenderTarget(size.x, size.y, D3DFMT_A8R8G8B8, multisample_type, main_surface_desc.MultiSampleQuality, false, &surface, nullptr) }; FAILED(result)) {
                    utils::logger(utils::e_log_type::error, "CreateRenderTarget failed, return code {}.", result);
                    return;
                }
            } else {
                if(texture) {
                    IDirect3DSurface9* surf{ };
                    if(auto result{ texture->GetSurfaceLevel(0, &surface) }; FAILED(result)) {
                        utils::logger(utils::e_log_type::error, "GetSurfaceLevel failed, return code {}.", result);
                        return;
                    }
                }
            }
        } else {
            if(auto result{ shared.device->GetRenderTarget(0, &surface) }; FAILED(result)) {
                utils::logger(utils::e_log_type::error, "GetBuffer failed, return code {}.", result);
                return;
            }
        }
    }

    void c_frame_buffer::on_destroy() {
        if(texture) { texture->Release(); texture = nullptr; }
        if(surface) { surface->Release(); surface = nullptr; }
    }

    void c_frame_buffer::clear() {
        shared.device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
    }

    void c_frame_buffer::set() {
        shared.device->SetRenderTarget(0, surface);
    }

    void c_frame_buffer::copy_from(const std::unique_ptr<i_frame_buffer>& another_frame_buffer) {
        shared.device->StretchRect((IDirect3DSurface9*)another_frame_buffer->get_surface(), nullptr, surface, nullptr, D3DTEXF_LINEAR);
    }
}