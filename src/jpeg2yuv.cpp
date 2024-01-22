/*
 * Copyright (C) 2018 Microchip Technology Inc.  All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <egt/ui>
#include <iostream>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <planes/plane.h>
#include "egt/detail/screen/kmsoverlay.h"
#include "egt/detail/screen/kmsscreen.h"
#include "djpeg.h"


#define MAX_WIDTH	800
#define MAX_HEIGHT	360


struct ImageFrame {
    char* buffer;
    size_t bufferSize;
};

bool readJPEG(const char* filename, ImageFrame& frame) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return false;
    }

    frame.bufferSize = static_cast<size_t>(file.tellg());
    file.seekg(0, std::ios::beg);

    frame.buffer = static_cast<char*>(malloc(frame.bufferSize));
    if (!frame.buffer) {
        std::cerr << "Error allocating memory for buffer." << std::endl;
        return false;
    }

    file.read(reinterpret_cast<char*>(frame.buffer), frame.bufferSize);
    file.close();

    return true;
}

class EGT_API OverlayWindow : public egt::Window
{
public:
    OverlayWindow(const egt::Rect& rect, 
              egt::PixelFormat format_hint = egt::PixelFormat::yuv420,
              egt::WindowHint hint = egt::WindowHint::overlay)
        : egt::Window(rect, format_hint, hint)
    {
        allocate_screen();
        m_overlay = reinterpret_cast<egt::detail::KMSOverlay*>(screen());
        assert(m_overlay);
        plane_set_pos(m_overlay->s(), 0, 60);
        plane_apply(m_overlay->s());
    }

    egt::detail::KMSOverlay* GetOverlay()
    {
        return m_overlay;
    }

private:
    egt::detail::KMSOverlay* m_overlay;
};

int main(int argc, char** argv)
{
    egt::Application app(argc, argv);

    egt::add_search_path("/usr/share/libegt");

    egt::TopWindow window;

    ///-- Read the JPEG file to local buffer
    std::vector<ImageFrame> frames;
    std::vector<std::string> filenames;

    // Since this code is tested on sam9x60-curiosity D1G, the memory is limited.
    // So we only decode 641 images to ram.
    for (auto i=3000; i<3641; i++) {
        filenames.push_back("images/" + std::to_string(i+1) + ".jpg");
    }

    for (std::string filename : filenames) {
        ImageFrame frame;
        if (readJPEG(filename.c_str(), frame)) {
            frames.push_back(frame);
        } else {
            std::cerr << "Failed to read JPEG file: " << filename << std::endl;
        }
    }

    auto label = std::make_shared<egt::ImageLabel>(
                     egt::Image("icon:egt_logo_black.png;128"));
    label->fill_flags().clear();
    label->margin(7);
    label->align(egt::AlignFlag::left);
    label->image_align(egt::AlignFlag::center);
    window.add(label);

    auto txt = std::make_shared<egt::TextBox>("EGT JPEG decoding YUV demo");
    txt->align(egt::AlignFlag::top | egt::AlignFlag::center_horizontal);
    txt->font(egt::Font(25, egt::Font::Weight::bold));
    txt->text_align(egt::AlignFlag::center);
    txt->margin(5);
    txt->border_flags({egt::Theme::BorderFlag::bottom, egt::Theme::BorderFlag::top});
    txt->disable();
    window.add(txt);

    auto descriptions = std::make_shared<egt::Label>(window, "Decode JPEG to YUV on HEO layer        800x360");
    descriptions->align(egt::AlignFlag::bottom | egt::AlignFlag::left);
    descriptions->font(egt::Font(30));
    descriptions->margin(7);

    egt::Label lfps("FPS: ---");
    lfps.align(egt::AlignFlag::bottom | egt::AlignFlag::right);
    lfps.color(egt::Palette::ColorId::label_text, egt::Palette::black);
    lfps.color(egt::Palette::ColorId::label_bg, egt::Palette::transparent);
    lfps.font(egt::Font(30));
    lfps.margin(7);
    window.add(lfps);

    OverlayWindow heoWin(egt::Rect(0, 60, MAX_WIDTH, MAX_HEIGHT));
    window.add(heoWin);
    
    egt::experimental::FramesPerSecond fps;
    fps.start();

    egt::PeriodicTimer timer(std::chrono::milliseconds(33));
    timer.on_timeout([&heoWin, &frames, &fps, &lfps]() {
        static int idx = 0;
        char* yuv_ptr = (char*)heoWin.GetOverlay()->raw();

        int width = MAX_WIDTH;
        int height = MAX_HEIGHT;
        djpeg_yuv((char *)frames[idx].buffer, frames[idx].bufferSize, yuv_ptr, &width, &height);

        if (fps.ready())
        {
            std::ostringstream ss;
            ss << "FPS: " << std::round(fps.fps());
            lfps.text(ss.str());
        }

        heoWin.GetOverlay()->schedule_flip();

        fps.end_frame();

        if (++idx >= 641)
            idx = 0;
    });
    timer.start();

    window.show();

    auto ret = app.run();

    for (ImageFrame& frame : frames) {
        free(frame.buffer);
    }

    return ret;
}

