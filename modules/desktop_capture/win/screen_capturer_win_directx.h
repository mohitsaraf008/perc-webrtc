/*
 *  Copyright (c) 2016 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef WEBRTC_MODULES_DESKTOP_CAPTURE_WIN_SCREEN_CAPTURER_WIN_DIRECTX_H_
#define WEBRTC_MODULES_DESKTOP_CAPTURE_WIN_SCREEN_CAPTURER_WIN_DIRECTX_H_

#include <D3DCommon.h>

#include <memory>
#include <vector>

#include "webrtc/modules/desktop_capture/desktop_capture_options.h"
#include "webrtc/modules/desktop_capture/desktop_capturer.h"
#include "webrtc/modules/desktop_capture/desktop_region.h"
#include "webrtc/modules/desktop_capture/screen_capture_frame_queue.h"
#include "webrtc/modules/desktop_capture/win/dxgi_duplicator_controller.h"
#include "webrtc/modules/desktop_capture/win/dxgi_frame.h"
#include "webrtc/rtc_base/scoped_ref_ptr.h"

namespace webrtc {

// ScreenCapturerWinDirectx captures 32bit RGBA using DirectX.
class ScreenCapturerWinDirectx : public DesktopCapturer {
 public:
  using D3dInfo = DxgiDuplicatorController::D3dInfo;

  // Whether the system supports DirectX based capturing.
  static bool IsSupported();

  // Returns a most recent D3dInfo composed by
  // DxgiDuplicatorController::Initialize() function. This function implicitly
  // calls DxgiDuplicatorController::Initialize() if it has not been
  // initialized. This function returns false and output parameter is kept
  // unchanged if DxgiDuplicatorController::Initialize() failed.
  // The D3dInfo may change based on hardware configuration even without
  // restarting the hardware and software. Refer to https://goo.gl/OOCppq. So
  // consumers should not cache the result returned by this function.
  static bool RetrieveD3dInfo(D3dInfo* info);

  // Whether current process is running in a Windows session which is supported
  // by ScreenCapturerWinDirectx.
  // Usually using ScreenCapturerWinDirectx in unsupported sessions will fail.
  // But this behavior may vary on different Windows version. So consumers can
  // always try IsSupported() function.
  static bool IsCurrentSessionSupported();

  explicit ScreenCapturerWinDirectx();

  ~ScreenCapturerWinDirectx() override;

  // DesktopCapturer implementation.
  void Start(Callback* callback) override;
  void SetSharedMemoryFactory(
      std::unique_ptr<SharedMemoryFactory> shared_memory_factory) override;
  void CaptureFrame() override;
  bool GetSourceList(SourceList* sources) override;
  bool SelectSource(SourceId id) override;

 private:
  const rtc::scoped_refptr<DxgiDuplicatorController> controller_;
  ScreenCaptureFrameQueue<DxgiFrame> frames_;
  std::unique_ptr<SharedMemoryFactory> shared_memory_factory_;
  Callback* callback_ = nullptr;
  SourceId current_screen_id_ = kFullDesktopScreenId;

  RTC_DISALLOW_COPY_AND_ASSIGN(ScreenCapturerWinDirectx);
};

}  // namespace webrtc

#endif  // WEBRTC_MODULES_DESKTOP_CAPTURE_WIN_SCREEN_CAPTURER_WIN_DIRECTX_H_
