#include "Common.h"
#include <iostream>
#include "States.h"


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

	osg::ref_ptr<osg::Node> root = createSceneGraph2();
	if (!root.valid())
		osg::notify(osg::FATAL) <<
		"Failed in createSceneGraph()." << std::endl;

	osgViewer::Viewer viewer;
	//viewer.addEventHandler(new RemoveShapeHandler);
	viewer.setSceneData(root.get());
	return viewer.run();
	
}