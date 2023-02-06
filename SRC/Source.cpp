#include "Common.h"
#include <iostream>

osg::ref_ptr<osg::Node> createSceneGraph()
{
	// Create an object to store geometry in.
	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
		// Create an array of four vertices.
		osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array;
	geom->setVertexArray(v.get());
	v->push_back(osg::Vec3(-1.f, 0.f, -1.f));
	v->push_back(osg::Vec3(1.f, 0.f, -1.f));
	v->push_back(osg::Vec3(1.f, 0.f, 1.f));
	v->push_back(osg::Vec3(-1.f, 0.f, 1.f));
	// Create an array of four colors.
	osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array;
	geom->setColorArray(c.get());
	geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
	c->push_back(osg::Vec4(1.f, 0.f, 0.f, 1.f));
	c->push_back(osg::Vec4(0.f, 1.f, 0.f, 1.f));
	c->push_back(osg::Vec4(0.f, 0.f, 1.f, 1.f));
	c->push_back(osg::Vec4(1.f, 1.f, 1.f, 1.f));
	// Create an array for the single normal.
	osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array;
	geom->setNormalArray(n.get());
	geom->setNormalBinding(osg::Geometry::BIND_OVERALL);
	n->push_back(osg::Vec3(0.f, -1.f, 0.f));
	// Draw a four-vertex quad from the stored data.
	geom->addPrimitiveSet(
		new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4));
	// Add the Geometry (Drawable) to a Geode and
	// return the Geode.
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->addDrawable(geom.get());
	return geode.get();
}

int main()
{
	
	//osgText::Text* text = osgCookBook::createText(osg::Vec3(50.0f, 50.0f, 0.0f), "", 10.0f);
	//osgText::Text* text2 = osgCookBook::createText(osg::Vec3(50.0f, 50.0f, 0.0f), "", 20.0f);

	//osg::ref_ptr<osg::Geode> textGeode = new osg::Geode;
	//textGeode->addDrawable(text);
	////textGeode->addDrawable(text2);

	//osg::ref_ptr<osg::Camera> hudCamera =osgCookBook::createHUDCamera(0, 800, 0, 600);
	//hudCamera->addChild(textGeode.get());

	//osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	//geode->addDrawable(new osg::ShapeDrawable(new osg::Box(osg::Vec3(-2.0f, 0.0f, 0.0f), 1.0f)));
	//geode->addDrawable(new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(2.0f, 0.0f, 0.0f), 1.0f)));

	////Testing
	//osg::ref_ptr<osg::Geode> geode2 = new osg::Geode;
	//geode2->addDrawable(new osg::ShapeDrawable(new osg::Box(osg::Vec3(-5.0f, 5.0f, 0.0f), 5.0f)));
	//geode2->addDrawable(new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(5.0f, 5.0f, 0.0f), 5.0f)));

	//osg::ref_ptr<osg::Group> root = new osg::Group;
	//root->addChild(hudCamera.get());
	//root->addChild(geode.get());

	////Added
	//root->addChild(geode2.get());


	//osg::ref_ptr<ObserveShapeCallback> observerCB = new ObserveShapeCallback;
	//observerCB->_text = text;
	//observerCB->_drawable1 = geode->getDrawable(0);
	//observerCB->_drawable2 = geode->getDrawable(1);
	//root->addUpdateCallback(observerCB.get());

	////Added
	//osg::ref_ptr<ObserveShapeCallback> observerCB2 = new ObserveShapeCallback;
	//observerCB2->_text = text2;
	//observerCB2->_drawable1 = geode2->getDrawable(0);
	//observerCB2->_drawable2 = geode2->getDrawable(1);
	//root->addUpdateCallback(observerCB2.get());

	osg::ref_ptr<osg::Node> root = createSceneGraph();
	if (!root.valid())
		osg::notify(osg::FATAL) <<
		"Failed in createSceneGraph()." << std::endl;

	osgViewer::Viewer viewer;
	//viewer.addEventHandler(new RemoveShapeHandler);
	viewer.setSceneData(root.get());
	return viewer.run();
	
}