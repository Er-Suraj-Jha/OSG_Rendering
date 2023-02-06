/*
#include <osg/MatrixTransform>
#include <osg/ShapeDrawable>
#include <osg/PolygonMode>
#include <osgDB/ReadFile>
#include <osgUtil/LineSegmentIntersector>
#include <osgViewer/Viewer>

class PickHandler : public osgGA::GUIEventHandler
{
public:
	osg::Node* getOrCreateSelectionBox();
	virtual bool handle(const osgGA::GUIEventAdapter& ea,
		osgGA::GUIActionAdapter& aa);
protected:
	osg::ref_ptr<osg::MatrixTransform> _selectionBox;
};


osg::Node* PickHandler::getOrCreateSelectionBox()
{
	if (!_selectionBox)
	{
		osg::ref_ptr<osg::Geode> geode = new osg::Geode;
		geode->addDrawable(new osg::ShapeDrawable(new osg::Box(osg::Vec3(), 1.0f)));
		_selectionBox = new osg::MatrixTransform;
		_selectionBox->setNodeMask(0x1);
		_selectionBox->addChild(geode.get());
		osg::StateSet* ss = _selectionBox->getOrCreateStateSet();
		ss->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
		ss->setAttributeAndModes(new osg::PolygonMode(
			osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE));
	}
	return _selectionBox.get();
}



bool PickHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
	if (ea.getEventType() != osgGA::GUIEventAdapter::RELEASE || ea.getButton() != osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON || !(ea.getModKeyMask() & osgGA::GUIEventAdapter::MODKEY_CTRL))
		return false;
	osgViewer::Viewer* viewer =
		dynamic_cast<osgViewer::Viewer*>(&aa);
	if (viewer)
	{
		osg::ref_ptr<osgUtil::LineSegmentIntersector>intersector = new osgUtil::LineSegmentIntersector(osgUtil::Intersector::WINDOW, ea.getX(), ea.getY());
		osgUtil::IntersectionVisitor iv(intersector.get());
		iv.setTraversalMask(~0x1);
		viewer->getCamera()->accept(iv);

		if (intersector->containsIntersections())
		{
			//osgUtil::LineSegmentIntersector::Intersection& result = *(intersector->getIntersections().begin());
			osgUtil::LineSegmentIntersector::Intersection result = *(intersector->getIntersections().begin());
			//osg::BoundingBox bb = result.drawable->getBound(); //for osg 3.0.1

			osg::BoundingBox bb = result.drawable->getBoundingBox(); //for osg 3.6.3
			// for osg 3.6.3 refer this: https://github.com/xarray/osgRecipes/issues/13
			// and this example (line 22 onwards) : https://github.com/xarray/osgRecipes/blob/master/cookbook/chapter6/ch06_09/CloudBlock.cpp

			osg::Vec3 worldCenter = bb.center() * osg::computeLocalToWorld(result.nodePath);
			_selectionBox->setMatrix(osg::Matrix::scale(bb.xMax() - bb.xMin(), bb.yMax() - bb.yMin(), bb.zMax() - bb.zMin()) * osg::Matrix::translate(worldCenter));
		}
	}
	return false;
}



int main(int argc, char* argv[])
{
	(void)argc; (void)argv;

	osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
	traits->x = 50;
	traits->y = 50;
	traits->width = 800;
	traits->height = 600;
	traits->windowName = "OSG application";
	traits->windowDecoration = true;
	traits->doubleBuffer = true;
	traits->samples = 4;

	osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());

	osg::ref_ptr<osg::Camera> camera = new osg::Camera;
	camera->setGraphicsContext(gc);
	camera->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
	camera->setClearMask(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	camera->setClearColor(osg::Vec4(0.2f, 0.2f, 0.4f, 1.0f));
	double aspect = static_cast<double>(traits->width) / static_cast<double>(traits->height);
	camera->setProjectionMatrixAsPerspective(30.0, aspect, 1.0, 1000.0);
	camera->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);

	//osg::ref_ptr<osg::Node> root = osgDB::readNodeFile("cessna.osg");
	osg::ref_ptr<osg::Node> model1 = osgDB::readNodeFile("cessna.osg");
	osg::ref_ptr<osg::Node> model2 = osgDB::readNodeFile("cow.osg");

	osg::ref_ptr<osg::Group> root = new osg::Group;
	root->addChild(model1.get());
	root->addChild(model2.get());

	osg::ref_ptr<PickHandler> picker = new PickHandler;
	root->addChild(picker->getOrCreateSelectionBox());


	// Adding a triangle/line in scene

	// Create the geometry which will define the triangle
	osg::ref_ptr<osg::Geometry> myTriangleGeometry = new osg::Geometry;

	// Define the triangle's 3 vertices
	osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
	vertices->push_back(osg::Vec3(0, 0, 0));
	vertices->push_back(osg::Vec3(150, 100, 0));
	vertices->push_back(osg::Vec3(0, 0, 100));
	myTriangleGeometry->setVertexArray(vertices);

	// You can give each vertex its own color, but let's just make it green for now
	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
	colors->push_back(osg::Vec4(0, 1.0, 0, 1.0)); // RGBA for green
	myTriangleGeometry->setColorArray(colors);
	myTriangleGeometry->setColorBinding(osg::Geometry::BIND_OVERALL);

	// Turn off lighting
	myTriangleGeometry->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

	// Turn on blending
	myTriangleGeometry->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);

	// Define the geometry type as 'triangles'
	//myTriangleGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, vertices->size()));
	myTriangleGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, vertices->size()));

	// Finally, let's add our triangle to a geode
	osg::ref_ptr<osg::Geode> myGeode = new osg::Geode;
	myGeode->addDrawable(myTriangleGeometry);

	root->addChild(myGeode.get());


	osgViewer::Viewer viewer;
	viewer.setCamera(camera.get());
	viewer.setSceneData(root.get());
	viewer.addEventHandler(picker.get());
	return viewer.run();
}

*/
