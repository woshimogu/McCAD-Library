#ifndef BOUNDSURFACE_HPP
#define BOUNDSURFACE_HPP

// C++
#include <memory>
// McCAD
#include "surface.hpp"

namespace McCAD::Geometry{
  class BoundSurface : public Surface {
  private:
    class Impl;

  public:
    BoundSurface();
    ~BoundSurface();
    Standard_Boolean operator == (const BoundSurface& that);
    Standard_Boolean operator << (const BoundSurface& that);
      
    Impl* accessBSImpl() const;

  private:
    std::unique_ptr<Impl> pBSImpl;
  };
}
#endif //BOUNDSURFACE_HPP
