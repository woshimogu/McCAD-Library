// McCAD
#include "planeComparator.hpp"

McCAD::Tools::PlaneComparator::PlaneComparator(Standard_Real parameterTolerance,
                                               Standard_Real angularTolerance,
                                               Standard_Real distanceTolerance)
    : parameterTolerance{parameterTolerance},
      angularTolerance{angularTolerance},
      distanceTolerance{distanceTolerance}{
}

std::optional<Standard_Boolean>
McCAD::Tools::PlaneComparator::operator()(const GeomAdaptor_Surface& firstAdaptor,
                                          const GeomAdaptor_Surface& secondAdaptor){
    auto firstPlane = firstAdaptor.Plane();
    auto secondPlane = secondAdaptor.Plane();
    // Scale planes
    firstPlane.Scale(gp_Pnt{0.0, 0.0, 0.0}, 0.1);
    secondPlane.Scale(gp_Pnt{0.0, 0.0, 0.0}, 0.1);
    // Compare distance between planes.
    if (firstPlane.Distance(secondPlane) > distanceTolerance){
        return Standard_False;
    }
    // Compare plane parameters
    return equivalentPlaneParameters(firstPlane, secondPlane);
}

std::array<Standard_Real, 4>
McCAD::Tools::PlaneComparator::planeParameters(const gp_Pln& plane) const{
    std::array<Standard_Real, 4> planeParameters;
    plane.Coefficients(planeParameters[0], planeParameters[1],
            planeParameters[2], planeParameters[3]);
    for(auto& parameter : planeParameters){
        if(std::abs(parameter) < parameterTolerance) parameter = 0.0;
    }
    return planeParameters;
}

Standard_Boolean
McCAD::Tools::PlaneComparator::equivalentPlaneParameters(const gp_Pln& first,
                                                         const gp_Pln& second) const{
    try{
        auto firstPlaneParameters = planeParameters(first);
        gp_Dir firstPlaneDirection{
            firstPlaneParameters[0],
            firstPlaneParameters[1],
            firstPlaneParameters[2]
        };
        auto secondPlaneParameters = planeParameters(second);
        gp_Dir secondPlaneDirection{
            secondPlaneParameters[0],
            secondPlaneParameters[1],
            secondPlaneParameters[2]
        };
        return (firstPlaneDirection.IsEqual(secondPlaneDirection, angularTolerance)
                && std::abs(firstPlaneParameters[3] - secondPlaneParameters[3])
                < distanceTolerance) ||
               (firstPlaneDirection.IsOpposite(secondPlaneDirection, angularTolerance)
                && std::abs(firstPlaneParameters[3] + secondPlaneParameters[3])
                < distanceTolerance);
    } catch(const Standard_ConstructionError&){
        // Plane parameters cannot form a normed direction vector;
        // not distinguishable from zero vector
        return Standard_False;
      }
}
