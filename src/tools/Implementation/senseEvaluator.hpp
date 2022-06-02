#ifndef SENSEEVALUATOR_HPP
#define SENSEEVALUATOR_HPP

// C++
#include <optional>
// OCC
#include <Standard.hxx>
#include <TopoDS_Face.hxx>
#include <gp_Pnt.hxx>
#include <gp_Pln.hxx>
#include <gp_Cylinder.hxx>

namespace McCAD::Tools{
  class SenseEvaluator{
  public:
      SenseEvaluator() = default;

      std::optional<Standard_Real> operator()(const TopoDS_Face& face,
                                              const gp_Pnt& point);
      Standard_Real senseRelativeToPlane(const gp_Pln& plane, const gp_Pnt& point);
      Standard_Real senseRelativeToCyl(const gp_Cylinder& cylinder, const gp_Pnt& point);
  };
}

#endif //SENSEEVALUATOR_HPP
