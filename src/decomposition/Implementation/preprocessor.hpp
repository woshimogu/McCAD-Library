#ifndef PREPROCESSOR_HPP
#define PREPROCESSOR_HPP

// C++
#include <variant>
#include <memory>
// McCAD
#include "SolidType.hpp"
#include "compound.hpp"
#include "planarSolid_impl.hpp"
#include "cylSolid_impl.hpp"
#include "torSolid_impl.hpp"
// OCC
#include <Standard.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_Shape.hxx>

namespace McCAD::Decomposition{
  class Preprocessor{
  public:
      Preprocessor();
      Preprocessor(const Standard_Real& minSolidVolume);
      ~Preprocessor();
  private:
    Tools::SolidType solidType;
    // WARNING!!
    // The order of solid object types should be synched with Tools::SolidType;
    using VariantType = std::variant<std::monostate,
                                     std::shared_ptr<McCAD::Geometry::PLSolid>,
                                     std::shared_ptr<McCAD::Geometry::CYLSolid>,
                                     std::shared_ptr<McCAD::Geometry::TORSolid>>;
  public:
    Standard_Real minSolidVolume;
    void operator()(const std::shared_ptr<Geometry::Impl::Compound>& compound);
    VariantType perform(const TopoDS_Shape& shape);
    Standard_Boolean checkBndSurfaces(const TopoDS_Shape& shape);
    Standard_Boolean checkVolume(const TopoDS_Shape& shape);
    Standard_Integer determineSolidType(const TopoDS_Solid& solid);
  };
}

#endif //PREPROCESSOR_HPP
