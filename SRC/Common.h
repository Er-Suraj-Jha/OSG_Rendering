
//#include <iostream>
//#include <cmath>

//#include <osg/Group>
//#include <osg/Geometry>
//#include <osg/PositionAttitudeTransform>
//#include <osg/PolygonMode>

#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osgText/Text>
#include <osgText/Font>
#include <osgViewer/Viewer>



namespace osgCookBook {
	 osg::Camera* createHUDCamera(double left, double right, double
		bottom, double top)
	{
		osg::ref_ptr<osg::Camera> camera = new osg::Camera;
		camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
		camera->setClearMask(GL_DEPTH_BUFFER_BIT);
		camera->setRenderOrder(osg::Camera::POST_RENDER);
		camera->setAllowEventFocus(false);
		camera->setProjectionMatrix(
			osg::Matrix::ortho2D(left, right, bottom, top));
		camera->getOrCreateStateSet()->setMode(
			GL_LIGHTING, osg::StateAttribute::OFF);
		return camera.release();
	}

	//osg::ref_ptr<osgText::Font> g_font = osgText::readFontFile("fonts/arial.ttf");

	 osgText::Text* createText(const osg::Vec3 & pos, const std::string &content, float size)
	{
		osg::ref_ptr<osgText::Text> text = new osgText::Text;
		text->setDataVariance(osg::Object::DYNAMIC);
		osg::ref_ptr<osgText::Font> g_font = osgText::readFontFile("fonts/arial.ttf");

		text->setFont(g_font.get());
		text->setCharacterSize(size);
		text->setAxisAlignment(osgText::TextBase::XY_PLANE);
		text->setPosition(pos);
		text->setText(content);
		return text.release();
	}

	class PickHandler : public osgGA::GUIEventHandler
	{
	public:
		virtual void doUserOperations(
			osgUtil::LineSegmentIntersector::Intersection&) = 0;
		virtual bool handle(const osgGA::GUIEventAdapter& ea,
			osgGA::GUIActionAdapter& aa)
		{
			if (ea.getEventType() != osgGA::GUIEventAdapter::RELEASE
				|| ea.getButton() != osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON
				|| !(ea.getModKeyMask() & osgGA::GUIEventAdapter::MODKEY_CTRL))
				return false;
			osgViewer::View* viewer = dynamic_cast<osgViewer::View*>(&aa);
			if (viewer)
			{
				osg::ref_ptr<osgUtil::LineSegmentIntersector>
					intersector = new osgUtil::LineSegmentIntersector
					(osgUtil::Intersector::WINDOW, ea.getX(), ea.getY());
				osgUtil::IntersectionVisitor iv(intersector.get());
				viewer->getCamera()->accept(iv);
				if (intersector->containsIntersections())
				{
				    osgUtil::LineSegmentIntersector::Intersection result = *(intersector->getIntersections().begin());
					doUserOperations(result);
				}
			}
			return false;
		}
	};
}

class RemoveShapeHandler : public osgCookBook::PickHandler
{
	virtual void doUserOperations(osgUtil::LineSegmentIntersector::
		Intersection& result)
	{
		if (result.nodePath.size() > 0)
		{
			osg::Geode* geode = dynamic_cast<osg::Geode*>(
				result.nodePath.back());
			if (geode) geode->removeDrawable(
				result.drawable.get());
		}
	}
};

class ObserveShapeCallback : public osg::NodeCallback
{
public:
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
	{
		std::string content;
		if (_drawable1.valid()) content += "Drawable 1; ";
		if (_drawable2.valid()) content += "Drawable 2; ";
		if (_text.valid()) _text->setText(content);
	}
	osg::ref_ptr<osgText::Text> _text;
	osg::observer_ptr<osg::Drawable> _drawable1;
	osg::observer_ptr<osg::Drawable> _drawable2;
};
