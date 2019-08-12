// McCAD
#include "tools_impl.hpp"
#include <vector>

void
McCAD::Tools::Preprocessor::Impl::removeSmallFaces(
        TopoDS_Shape& solidShape,
        Standard_Real precision,
        Standard_Real maxTolerance){
  TopoDS_Shape fixedSolidShape;
  Handle_ShapeFix_FixSmallFace smallFaceFix = new ShapeFix_FixSmallFace;
  smallFaceFix->Init(solidShape);
  smallFaceFix->SetPrecision(precision);
  smallFaceFix->SetMaxTolerance(maxTolerance);
  smallFaceFix->Perform();
  solidShape = smallFaceFix->FixShape();
}

void
McCAD::Tools::Preprocessor::Impl::repairSolid(TopoDS_Solid& solid){
  Handle_ShapeFix_Solid solidFix = new ShapeFix_Solid;
  solidFix->Init(solid);
  solidFix->Perform();
  solid = TopoDS::Solid(solidFix->Solid());
}

Standard_Boolean
McCAD::Tools::Preprocessor::Impl::checkFace(const TopoDS_Face& face, Standard_Real tolerance){
  ShapeAnalysis_CheckSmallFace shapeAnalysis;
  TopoDS_Edge edge1, edge2;
  if( shapeAnalysis.CheckSpotFace(face, tolerance) || shapeAnalysis.CheckStripFace(face, edge1, edge2, tolerance))
    {
      return Standard_True;
    }
  return Standard_False;
}

void
McCAD::Tools::Preprocessor::Impl::fixFace(TopoDS_Face& face, Standard_Real precision, Standard_Real maxTolerance){
  Handle_ShapeFix_Shape shapeFixer = new ShapeFix_Shape(face);
  shapeFixer->SetPrecision(precision);
  shapeFixer->SetMaxTolerance(maxTolerance);
  shapeFixer->FixWireTool()->FixRemovePCurveMode() = 1;
  shapeFixer->FixWireTool()->FixConnected();
  shapeFixer->Perform();
  face = TopoDS::Face(shapeFixer->Shape());
}

<<<<<<< HEAD
gp_Dir
McCAD::Tools::Preprocessor::Impl::normalOnFace(const TopoDS_Face& aFace, const gp_Pnt& aPoint){
  Standard_Real uParameter, vParameter;
  BRepAdaptor_Surface surfaceAdaptor(aFace, Standard_True);

  switch (surfaceAdaptor.GetType())
    {
    case GeomAbs_Plane:
      ElSLib::Parameters(surfaceAdaptor.Plane(), aPoint, uParameter, vParameter);
      break;

    case GeomAbs_Cylinder:
      ElSLib::Parameters(surfaceAdaptor.Cylinder(), aPoint, uParameter, vParameter);
      break;

    case GeomAbs_Cone:
      ElSLib::Parameters(surfaceAdaptor.Cone(), aPoint, uParameter, vParameter);
      break;

    case GeomAbs_Torus:
      ElSLib::Parameters(surfaceAdaptor.Torus(), aPoint, uParameter, vParameter);
      break;

    default:
      {
	return gp_Dir(1., 0., 0.);
      }
    }

  gp_Pnt point;
  gp_Vec D1U, D1V;
  surfaceAdaptor.D1(uParameter, vParameter, point, D1U, D1V);
  gp_Dir normal;
  CSLib_DerivativeStatus derivativeStatus;
  CSLib::Normal(D1U, D1V, Precision::Confusion(), derivativeStatus, normal);
  if (aFace.Orientation() == TopAbs_FORWARD)
    normal.Reverse();
  return normal;
}


=======
>>>>>>> refactoring parametrized faces with uv-coordinates
Standard_Boolean
McCAD::Tools::Preprocessor::Impl::isSameEdge(
        const TopoDS_Edge& firstEdge,
        const TopoDS_Edge& secondEdge,
        Standard_Real distanceTolerance){
  // Compare first the start and end points of the curves.
  Standard_Real firstEdgeStart, firstEdgeEnd;
  Handle_Geom_Curve firstCurve = BRep_Tool::Curve(firstEdge, firstEdgeStart, firstEdgeEnd);
  GeomAdaptor_Curve firstCurveAdaptor(firstCurve);

  Standard_Real secondEdgeStart, secondEdgeEnd;
  Handle_Geom_Curve secondCurve = BRep_Tool::Curve(secondEdge, secondEdgeStart, secondEdgeEnd);
  GeomAdaptor_Curve secondCurveAdaptor(secondCurve);

  if (firstCurveAdaptor.GetType() != secondCurveAdaptor.GetType())
    {
      return Standard_False;
    }

  gp_Pnt firstStartPoint, firstEndPoint;
  firstCurve->D0(firstEdgeStart, firstStartPoint);
  firstCurve->D0(firstEdgeEnd, firstEndPoint);

  gp_Pnt secondStartPoint, secondEndPoint;
  secondCurve->D0(secondEdgeStart, secondStartPoint);
  secondCurve->D0(secondEdgeEnd, secondEndPoint);

  if ((firstStartPoint.IsEqual(secondStartPoint, distanceTolerance) && firstEndPoint.IsEqual(secondEndPoint, distanceTolerance)) || (firstStartPoint.IsEqual(secondEndPoint, distanceTolerance) && firstEndPoint.IsEqual(secondStartPoint, distanceTolerance)))
    {
      return Standard_True;
    }
  else
    {
      if ((firstStartPoint.Distance(secondStartPoint) <= distanceTolerance && firstEndPoint.Distance(secondEndPoint) <= distanceTolerance) || ((firstStartPoint.Distance(secondEndPoint) <= distanceTolerance && firstEndPoint.Distance(secondStartPoint) <= distanceTolerance)))
	{
	  return Standard_True;
	}
      else
	{
	  return Standard_False;
	}
    }
}
<<<<<<< HEAD

TopoDS_Face
<<<<<<< HEAD
McCAD::Tools::Preprocessor::Impl::fusePlanes(const TopoDS_Face& firstFace, const TopoDS_Face& secondFace, Standard_Real zeroTolerance, Standard_Real tolerance){
  std::array<Standard_Real, 4> firstUV;
=======
McCAD::Tools::Preprocessor::Impl::fusePlanes(
        const TopoDS_Face& firstFace,
        const TopoDS_Face& secondFace,
        Standard_Real zeroTolerance,
        Standard_Real tolerance){

  std::vector<Standard_Real> firstUV(4);
>>>>>>> refactoring of plane comparison
  BRepTools::UVBounds(firstFace, firstUV[0], firstUV[1], firstUV[2], firstUV[3]);
  std::array<Standard_Real, 4> secondUV;
  BRepTools::UVBounds(secondFace, secondUV[0], secondUV[1], secondUV[2], secondUV[3]);
  for (Standard_Integer i = 0; i <= firstUV.size() - 1; ++i)
    {
      if (firstUV[i] <= zeroTolerance)
	{
	  firstUV[i] = 0.0;
	}
    }
  for (Standard_Integer j = 0; j <= secondUV.size() - 1; ++j)
    {
      if (secondUV[j] <= zeroTolerance)
        {
          secondUV[j] = 0.0;
	}
    }

<<<<<<< HEAD
  std::array<Standard_Real, 4> newUV;
  newUV[0] = (firstUV[0] <= secondUV[0]) ? firstUV[0] : secondUV[0];
  newUV[1] = (firstUV[1] >= secondUV[1]) ? firstUV[1] :	secondUV[1];
  newUV[2] = (firstUV[2] <= secondUV[2]) ? firstUV[2] :	secondUV[2];
  newUV[3] = (firstUV[3] >= secondUV[3]) ? firstUV[3] : secondUV[3];

  Handle_Geom_Surface newSurface = BRep_Tool::Surface(firstFace);
  TopoDS_Face newFace = BRepBuilderAPI_MakeFace(newSurface, newUV[0], newUV[1], newUV[2], newUV[3], tolerance).Face();
=======
  std::vector<Standard_Real> newUV(4);
  newUV[0] = (firstUV[0] <= secondUV[0]) ? firstUV[0] : secondUV[0];
  newUV[1] = (firstUV[1] >= secondUV[1]) ? firstUV[1] : secondUV[1];
  newUV[2] = (firstUV[2] <= secondUV[2]) ? firstUV[2] : secondUV[2];
  newUV[3] = (firstUV[3] >= secondUV[3]) ? firstUV[3] :	secondUV[3];

  TopLoc_Location location;
  Handle_Geom_Surface newSurface = BRep_Tool::Surface(firstFace, location);
  TopoDS_Face newFace
          = BRepBuilderAPI_MakeFace(
              newSurface,
              newUV[0], newUV[1], newUV[2], newUV[3],
                tolerance).Face();
>>>>>>> refactoring of plane comparison

  if (newFace.Orientation() != firstFace.Orientation())
    {
      newFace.Orientation(firstFace.Orientation());
    }
  return newFace;
}
=======
>>>>>>> refactoring fusion of planes
