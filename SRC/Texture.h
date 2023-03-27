#pragma once
#include <osg/Texture2D> 
#include <osg/Image> 

#include <osg/Geode> 
#include <osg/Group> 
#include <osg/StateAttribute> 
#include <osgDB/ReadFile> 
#include <osgDB/WriteFile> 


class Texture {
private:
	const std::string file_location;
public:
	Texture(const std::string& filename, osg::ref_ptr<osg::Geometry>& geom);
	Texture(const std::string& filename, osg::ref_ptr<osg::Geode>& geom);

};