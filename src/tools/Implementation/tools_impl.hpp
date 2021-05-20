#ifndef TOOLSIMPL_HPP
#define TOOLSIMPL_HPP

// McCAD
#include "tools.hpp"
// OCC
#include <Standard.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Face.hxx>

namespace McCAD::Tools{
  class Preprocessor::Impl{
  public:
      Impl();
      Impl(const Standard_Real& precision, const Standard_Real& faceTolerance);
      ~Impl();

      Standard_Real precision{1.0e-7}, maxTolerance{1.0e-3};
      void removeSmallFaces(TopoDS_Shape& solidShape);
      void repairSolid(TopoDS_Solid& solid);
      Standard_Boolean checkFace(const TopoDS_Face& face);
      void fixFace(TopoDS_Face& face);
  };
}

#endif //TOOLSIMPL_HPP
