#include "Texture.h"

Texture::Texture(const std::string& filename, osg::ref_ptr<osg::Geometry>& geom): file_location(filename)
{
	osg::StateSet* state = geom->getOrCreateStateSet(); 
	osg::ref_ptr<osg::Image> image = osgDB::readImageFile(file_location ); 
	// Attach the image in a Texture2D object 
	osg::ref_ptr<osg::Texture2D> tex = new osg::Texture2D;
	tex->setImage(image.get());

	//state->setTextureAttribute(0, tex.get());
	state->setTextureAttributeAndModes(0, tex);
	tex->setUnRefImageDataAfterApply(true);

}

Texture::Texture(const std::string& filename, osg::ref_ptr<osg::Geode>& geom) : file_location(filename)
{
	osg::StateSet* state = geom->getOrCreateStateSet();
	osg::ref_ptr<osg::Image> image = osgDB::readImageFile(file_location);
	// Attach the image in a Texture2D object 
	osg::ref_ptr<osg::Texture2D> tex = new osg::Texture2D;
	tex->setImage(image.get());
	state->setTextureAttributeAndModes(0, tex);

}