#pragma once

#include "pvr_decompress.h"
#include "pixel_walker.h"

class image_desc_t {
private:
	int32_t			m_width;
	int32_t			m_height;
	pvr::color32_t*	m_data;

public:
	image_desc_t();
	image_desc_t( const image_desc_t& origin );
	virtual ~image_desc_t();

	void clear();
	void release();
	const bool set_sizes( const int32_t width, const int32_t height );

	inline pvr::color32_t* row( const int32_t index ) const			{ return ( !( 0 > index ) && ( m_height > index ) )? m_data + m_width * index : NULL; };
	inline pvr::color32_t* memory() const							{ return m_data; };
	inline const int width() const									{ return m_width; };
	inline const int height() const									{ return m_height; };
	inline pixel_walker<pvr::color32_t> pixels()					{ return pixel_walker<pvr::color32_t>( m_data, m_width, m_height ); };
	inline pixel_walker<pvr::color32_t> line( const int32_t index )	{ return pixel_walker<pvr::color32_t>( row( index ), m_width, 1 ); };
};