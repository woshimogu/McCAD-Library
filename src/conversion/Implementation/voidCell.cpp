// McCAD
#include "voidCell.hpp"
// OCC
#include <BRepBndLib.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <STEPControl_Writer.hxx>
#include <gp_Pnt.hxx>

McCAD::Conversion::VoidCell::VoidCell()
    : depth{0}, width{0} {
}

McCAD::Conversion::VoidCell::VoidCell(Standard_Integer& depth,
                                      Standard_Integer& width)
    : depth{depth}, width{width}{
}

McCAD::Conversion::VoidCell::~VoidCell(){}

void
McCAD::Conversion::VoidCell::addSolidIDs(const std::vector<std::shared_ptr<
                                         Geometry::Solid>>& solidObjList){
    for(auto& solid : solidObjList){
        solidIDList.push_back(solid->accessSImpl()->solidID);
    }
}

void
McCAD::Conversion::VoidCell::addSolids(const std::vector<std::shared_ptr<
                                       Geometry::Solid>>& solidObjList){
    for(auto& solid : solidObjList){
        BRepBndLib::AddOptimal(solid->accessSImpl()->solid, aabb);
    }
    aabb.SetGap(0);
    aabb.Get(minX, minY, minZ, maxX, maxY, maxZ);
    gp_Pnt minPoint(minX, minY, minZ);
    gp_Pnt maxPoint(maxX, maxY, maxZ);
    aabbSolid = BRepPrimAPI_MakeBox(minPoint, maxPoint).Solid();
    ///*//debug
    std::cout << "VoidCell contains: " << solidIDList.size() << std::endl;
    STEPControl_Writer writer0;
    writer0.Transfer(aabbSolid, STEPControl_StepModelType::STEPControl_AsIs);
    Standard_Integer kk = 0;
    std::string filename = "./aabb";
    std::string suffix = ".stp";
    while (std::filesystem::exists(filename + std::to_string(kk) + suffix)){
        ++kk;
    }
    filename += std::to_string(kk);
    filename += suffix;
    writer0.Write(filename.c_str());
    //*///debug
}
