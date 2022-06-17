// C++
#include <array>
// McCAD
#include "senseEvaluator.hpp"
//OCC
#include <BRepAdaptor_Surface.hxx>
#include <GeomAbs_SurfaceType.hxx>
#include <TopAbs_Orientation.hxx>
#include <gp_Ax1.hxx>

std::optional<Standard_Real>
McCAD::Tools::SenseEvaluator::operator()(const TopoDS_Face& face,
                                         const gp_Pnt& point){
    BRepAdaptor_Surface surfaceAdaptor(face);
    if (surfaceAdaptor.GetType() == GeomAbs_Plane){
        gp_Pln plane = surfaceAdaptor.Plane();
        if(face.Orientation() == TopAbs_REVERSED){
            gp_Ax1 planeNormal = plane.Axis();
            plane.SetAxis(planeNormal.Reversed());
        }
        return senseRelativeToPlane(plane, point);
    } else if (surfaceAdaptor.GetType() == GeomAbs_Cylinder){
        gp_Cylinder cylinder = surfaceAdaptor.Cylinder();
        return senseRelativeToCyl(cylinder, point);
    }
    else return std::nullopt;
}

Standard_Real
McCAD::Tools::SenseEvaluator::senseRelativeToPlane(const gp_Pln& plane,
                                                   const gp_Pnt& point){
    std::array<Standard_Real, 4> parameters;
    //A * X + B * Y + C * Z + D = 0
    plane.Coefficients(parameters[0], parameters[1], parameters[2], parameters[3]);
    return parameters[0]*point.X() + parameters[1]*point.Y() +
           parameters[2]*point.Z() + parameters[3];
}

Standard_Real
McCAD::Tools::SenseEvaluator::senseRelativeToCyl(const gp_Cylinder& cylinder,
                                                 const gp_Pnt& point){
    std::array<Standard_Real, 10> parameters;
    // A1.X**2 + A2.Y**2 + A3.Z**2 + 2.(B1.X.Y + B2.X.Z + B3.Y.Z) + 2.(C1.X + C2.Y + C3.Z) + D = 0.0
    cylinder.Coefficients(parameters[0], parameters[1], parameters[2],
                          parameters[3], parameters[4], parameters[5],
                          parameters[6], parameters[7], parameters[8],
                          parameters[9]);
    return parameters[0]*std::pow(point.X(), 2) + parameters[1]*std::pow(point.Y(), 2) +
            parameters[2]*std::pow(point.Z(), 2) + 2*parameters[3]*point.X()*point.Y() +
            2*parameters[4]*point.X()*point.Z() + 2*parameters[5]*point.Y()*point.Z()
            + parameters[3];
}