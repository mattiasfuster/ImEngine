#ifndef IMENGINE_ENGINE_RENDER_RHI_H
#define IMENGINE_ENGINE_RENDER_RHI_H

#include "EngineExport.h"

namespace ime::engine::platform::rendering {

class IWindow;

class IMENGINE_ENGINE_API IRHI {
 public:
  virtual ~IRHI() = default;

  virtual void Initialize(const std::shared_ptr<IWindow>& window) = 0;

 protected:
  IRHI() = default;
  IRHI(const IRHI&) = default;
  IRHI& operator=(const IRHI&) = default;
  IRHI(IRHI&&) = default;
  IRHI& operator=(IRHI&&) = default;
};

}  // namespace ime::engine::platform::rendering
#endif