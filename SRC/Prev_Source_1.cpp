
/*
#include <osgViewer/Viewer>
#include <iostream>
#include <cmath>

#include <osg/Group>
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/PositionAttitudeTransform>
#include <osg/PolygonMode>


static int index = 0;
static std::map<long, int> middlePointIndexCache;

void _DrawTriangle(osg::ref_ptr<osg::Geometry> geom, int point0, int point1, int point2)
{
	osg::ref_ptr<osg::DrawElementsUInt> triangle = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);
	triangle->push_back(point0);
	triangle->push_back(point1);
	triangle->push_back(point2);
	geom->addPrimitiveSet(triangle.get());
}

void _Normalize(osg::Vec3& pVec)
{
	double length = std::sqrt(pVec.x() * pVec.x() + pVec.y() * pVec.y() + pVec.z() * pVec.z());

	pVec.x() /= length;
	pVec.y() /= length;
	pVec.z() /= length;
}

int _AddVertex(osg::Vec3 value, osg::Vec3Array* vertex)
{
	_Normalize(value);
	vertex->push_back(value);

	return index++;
}

int _GetMiddlePoint(osg::ref_ptr<osg::Geometry> geom, int point1, int point2)
{
	int smallerIndex = point1 < point2 ? point1 : point2;
	int greaterIndex = point1 > point2 ? point1 : point2;
	long key = (smallerIndex << 16) + greaterIndex;

	std::map<long, int>::iterator it = middlePointIndexCache.find(key);
	if (middlePointIndexCache.end() != it) return it->second;

	osg::Vec3Array* pVecPoint = dynamic_cast<osg::Vec3Array*>(geom->getVertexArray());
	osg::Vec3 vecMiddle((pVecPoint->at(point1).x() + pVecPoint->at(point2).x()) / 2,
		(pVecPoint->at(point1).y() + pVecPoint->at(point2).y()) / 2,
		(pVecPoint->at(point1).z() + pVecPoint->at(point2).z()) / 2);

	int i = _AddVertex(vecMiddle, pVecPoint);
	geom->setVertexArray(pVecPoint);
	middlePointIndexCache.insert(std::pair<long, int>(key, i));

	return i;
}


osg::ref_ptr<osg::Node> createSceneGraph(int recursionLevel)
{
	osg::ref_ptr<osg::Group> root = new osg::Group;
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;

	osg::StateSet* pStateSet = geode->getOrCreateStateSet();
	// close the light
	pStateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	// set wire frame mode
	//osg::PolygonMode* polymode = new osg::PolygonMode;
	//polymode->setMode(osg::PolygonMode::FRONT_AND_BACK,osg::PolygonMode::LINE);
	//pStateSet->setAttributeAndModes(polymode,osg::StateAttribute::OVERRIDE|osg::StateAttribute::ON);

	geode->addDrawable(geom.get());
	root->addChild(geode.get());

	double t = (1.0f + std::sqrt(5.0f)) / 2.0f;

	osg::Vec3Array* vertex = new osg::Vec3Array;
	_AddVertex(osg::Vec3(-1, t, 0), vertex);
	_AddVertex(osg::Vec3(1, t, 0), vertex);
	_AddVertex(osg::Vec3(-1, -t, 0), vertex);
	_AddVertex(osg::Vec3(1, -t, 0), vertex);

	_AddVertex(osg::Vec3(0, -1, t), vertex);
	_AddVertex(osg::Vec3(0, 1, t), vertex);
	_AddVertex(osg::Vec3(0, -1, -t), vertex);
	_AddVertex(osg::Vec3(0, 1, -t), vertex);

	_AddVertex(osg::Vec3(t, 0, -1), vertex);
	_AddVertex(osg::Vec3(t, 0, 1), vertex);
	_AddVertex(osg::Vec3(-t, 0, -1), vertex);
	_AddVertex(osg::Vec3(-t, 0, 1), vertex);
	geom->setVertexArray(vertex);

	// create 20 triangles of the icosahedron
	osg::Vec3Array* vecTriangle = new osg::Vec3Array;

	// 5 faces around point 0
	vecTriangle->push_back(osg::Vec3(0, 11, 5));
	vecTriangle->push_back(osg::Vec3(0, 5, 1));
	vecTriangle->push_back(osg::Vec3(0, 1, 7));
	vecTriangle->push_back(osg::Vec3(0, 7, 10));
	vecTriangle->push_back(osg::Vec3(0, 10, 11));

	// 5 adjacent faces
	vecTriangle->push_back(osg::Vec3(1, 5, 9));
	vecTriangle->push_back(osg::Vec3(5, 11, 4));
	vecTriangle->push_back(osg::Vec3(11, 10, 2));
	vecTriangle->push_back(osg::Vec3(10, 7, 6));
	vecTriangle->push_back(osg::Vec3(7, 1, 8));

	// 5 faces around point 3
	vecTriangle->push_back(osg::Vec3(3, 9, 4));
	vecTriangle->push_back(osg::Vec3(3, 4, 2));
	vecTriangle->push_back(osg::Vec3(3, 2, 6));
	vecTriangle->push_back(osg::Vec3(3, 6, 8));
	vecTriangle->push_back(osg::Vec3(3, 8, 9));

	// 5 adjacent faces
	vecTriangle->push_back(osg::Vec3(4, 9, 5));
	vecTriangle->push_back(osg::Vec3(2, 4, 11));
	vecTriangle->push_back(osg::Vec3(6, 2, 10));
	vecTriangle->push_back(osg::Vec3(8, 6, 7));
	vecTriangle->push_back(osg::Vec3(9, 8, 1));

	for (int i = 0; i < recursionLevel; ++i)
	{
		osg::Vec3Array* vecTriangleNew = new osg::Vec3Array;
		for (int j = 0; j < vecTriangle->size(); ++j)
		{
			int a = _GetMiddlePoint(geom, vecTriangle->at(j).x(), vecTriangle->at(j).y());
			int b = _GetMiddlePoint(geom, vecTriangle->at(j).y(), vecTriangle->at(j).z());
			int c = _GetMiddlePoint(geom, vecTriangle->at(j).z(), vecTriangle->at(j).x());

			vecTriangleNew->push_back(osg::Vec3(vecTriangle->at(j).x(), a, c));
			vecTriangleNew->push_back(osg::Vec3(vecTriangle->at(j).y(), b, a));
			vecTriangleNew->push_back(osg::Vec3(vecTriangle->at(j).z(), c, b));
			vecTriangleNew->push_back(osg::Vec3(a, b, c));
		}
		vecTriangle = vecTriangleNew;
	}

	// add triangle to mesh
	for (int i = 0; i < vecTriangle->size(); ++i)
	{
		_DrawTriangle(geom, vecTriangle->at(i).x(), vecTriangle->at(i).y(), vecTriangle->at(i).z());
	}

	// color
	osg::Vec4Array* colors = new osg::Vec4Array;
	for (int i = 0; i < index; ++i)
	{
		if (i % 4 == 0)
			colors->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f)); //index 0 red
		else if (i % 4 == 1)
			colors->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f)); //index 1 green
		else if (i % 4 == 2)
			colors->push_back(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f)); //index 2 blue
		else if (i % 4 == 3)
			colors->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f)); //index 3 white 
	}

	geom->setColorArray(colors);
	geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

	return root.get();
}

int main()
{
	osg::ref_ptr<osg::Node> root = createSceneGraph(3);
	if (!root.valid())
	{
		osg::notify(osg::FATAL) << "Failed in createSceneGraph()." << std::endl;
		return 1;
	}

	osgViewer::Viewer myViewer;
	myViewer.setSceneData(root.get());
	myViewer.realize();
	myViewer.run();

	return 0;
}
*/