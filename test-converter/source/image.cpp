#include "image.h"

image_desc_t::image_desc_t() : m_width(0), m_height(0), m_data(NULL) {

};

image_desc_t::image_desc_t( const image_desc_t& origin ) : m_width(0), m_height(0), m_data(NULL) {
	if( origin.m_data ){
		set_sizes( origin.m_width, origin.m_height );
		memcpy( m_data, origin.m_data, m_width * m_height * sizeof( pvr::color32_t ) );
	};
};

image_desc_t::~image_desc_t(){
	release();
};

void image_desc_t::clear(){
	if( m_data ){
		memset( m_data, 0, m_width * m_height * sizeof( pvr::color32_t ) );
	};
};

void image_desc_t::release(){
	delete[] m_data;

	m_width		= 0;
	m_height	= 0;
	m_data		= NULL;
};

const bool image_desc_t::set_sizes( const int32_t width, const int32_t height ){
	release();

	m_width		= width;
	m_height	= height;
	m_data		= new pvr::color32_t[ m_width * m_height ];

	return true;
};
