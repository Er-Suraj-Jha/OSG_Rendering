#pragma once
#include <osg/Geode> 
#include <osg/Group> 
#include <osg/MatrixTransform> 
#include <osg/Geode> 
#include <osg/Geometry> 
#include <osg/StateSet> 
#include <osg/StateAttribute> 
#include <osg/ShadeModel> 
#include <osg/CullFace> 
#include <osg/PolygonMode> 
#include <osg/LineWidth>
#include <osg/ShapeDrawable>


#include"Texture.h"

osg::ref_ptr<osg::Geometry> createSceneGraph()
{
	// Create an object to store geometry in.
	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
	// Create an array of four vertices.
	osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array;
	v->push_back(osg::Vec3(-1.f, 0.f, -1.f));
	v->push_back(osg::Vec3(1.f, 0.f, -1.f));
	v->push_back(osg::Vec3(1.f, 0.f, 1.f));
	v->push_back(osg::Vec3(-1.f, 0.f, 1.f));
	geom->setVertexArray(v.get());


	//Texture Testing
	std::string file_location = "C:\\Users\\SURAJ JHA\\Desktop\\Suraj_Temp\\OSG_Upload\\Test.jpg";
	Texture texture{ file_location,geom };

	//Texture coordinates
	osg::ref_ptr<osg::Vec2Array> tc = new osg::Vec2Array;
	geom->setTexCoordArray(0, tc.get());
	tc->push_back(osg::Vec2(0.5f, 0.5f));
	tc->push_back(osg::Vec2(1.f, 0.5f));
	tc->push_back(osg::Vec2(1.f, 1.f));
	tc->push_back(osg::Vec2(0.5f, 1.f));

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
	return geom;

	// Add the Geometry (Drawable) to a Geode and return the Geode.
	/*osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->addDrawable(geom.get());
	return geode.get();*/
}

osg::ref_ptr<osg::Node> createSceneGraph2()
{
	// Create the root node Group. 
	osg::ref_ptr<osg::Group> root = new osg::Group;
	{
		// Disable lighting in the root node's StateSet. Make 
		// it PROTECTED to prevent osgviewer from enabling it. 
		osg::StateSet* state = root->getOrCreateStateSet();
		state->setMode(GL_LIGHTING,
			osg::StateAttribute::OFF |
			osg::StateAttribute::PROTECTED);
	}


	// Create the leaf node Geode and attach the Drawable. 
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->addDrawable(createSceneGraph().get());
	osg::Matrix m;
	{
		// Upper-left: Render the drawable with default state. 
		osg::ref_ptr<osg::MatrixTransform> mt =
			new osg::MatrixTransform;
		m.makeTranslate(-2.f, 0.f, 2.f);
		mt->setMatrix(m);

		/*std::string file_location = "C:\\Users\\SURAJ JHA\\Desktop\\Suraj_Temp\\OSG_Upload\\Test.jpg";
		Texture texture{ file_location,geode };*/

		root->addChild(mt.get());
		mt->addChild(geode.get());
	}

	{
		// Upper-right Set shade model to FLAT. 
		osg::ref_ptr<osg::MatrixTransform> mt =
			new osg::MatrixTransform;
		m.makeTranslate(2.f, 0.f, 2.f);
		mt->setMatrix(m);
		root->addChild(mt.get());
		mt->addChild(geode.get());
		osg::StateSet* state = mt->getOrCreateStateSet();
		osg::ShadeModel* sm = new osg::ShadeModel();
		sm->setMode(osg::ShadeModel::FLAT);
		state->setAttribute(sm);
	}

	{
		// Lower-left: Enable back face culling. 
		osg::ref_ptr<osg::MatrixTransform> mt =
			new osg::MatrixTransform;
		m.makeTranslate(-2.f, 0.f, -2.f);
		mt->setMatrix(m);
		root->addChild(mt.get());
		mt->addChild(geode.get());
		osg::StateSet* state = mt->getOrCreateStateSet();
		osg::CullFace* cf = new osg::CullFace(); // Default: BACK 
		state->setAttributeAndModes(cf);
	}

	{
		// Lower-right: Set polygon mode to LINE in 
		// draw3's StateSet. 
		osg::ref_ptr<osg::MatrixTransform> mt =
			new osg::MatrixTransform;
		m.makeTranslate(2.f, 0.f, -2.f);
		mt->setMatrix(m);
		root->addChild(mt.get());
		mt->addChild(geode.get());
		osg::StateSet* state = mt->getOrCreateStateSet();
		osg::PolygonMode* pm = new osg::PolygonMode(
			osg::PolygonMode::FRONT_AND_BACK,
			osg::PolygonMode::LINE);
		state->setAttributeAndModes(pm);
		// Also set the line width to 3. 
		osg::LineWidth* lw = new osg::LineWidth(30.f);
		state->setAttribute(lw);
	}

	return root.get();
}
