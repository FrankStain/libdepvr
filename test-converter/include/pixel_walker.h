#pragma once

template< typename pix_t >
class pixel_walker
{
private:
	pix_t*		m_base;
	pix_t*		m_row;
	pix_t*		m_current;

	uint32_t	m_width;
	uint32_t	m_height;

	pix_t*		m_eol;
	pix_t*		m_eom;

	void go_next(){
		if( *this ){
			m_current++;
			if( m_eol > m_current ){
				m_row		+= m_width;
				m_current	= m_row;
			};
		};
	};

public:
	pixel_walker( const uint8_t* vbase, const uint32_t vwidth, const uint32_t vheight ) :
			m_base((pix_t*)vbase), m_row((pix_t*)vbase), m_current((pix_t*)vbase),
			m_width(vwidth), m_height(vheight)
	{
		m_eol	= m_row + m_width;
		m_eom	= m_base + m_width * m_height;
	};

	pixel_walker( const pix_t* vbase, const uint32_t vwidth, const uint32_t vheight ) :
			m_base((pix_t*)vbase), m_row((pix_t*)vbase), m_current((pix_t*)vbase),
			m_width(vwidth), m_height(vheight)
	{
		m_eol	= m_row + m_width;
		m_eom	= m_base + m_width * m_height;
	};

	inline void rewind() {
		m_current	= m_base;
		m_row		= m_base;
		m_eol		= m_row + m_width;
		m_eom		= m_base + m_width * m_height;
	};

	inline operator pix_t& () { return *m_current; };
	inline operator const pix_t& () const { return *m_current; };
	inline operator const bool () const { return m_eom > m_current; };

	inline pix_t* operator -> () const { return m_current; };

	inline pix_t& operator ++ ( const int op ) { pix_t* p = m_current; go_next(); if( op ){ p = m_current; }; return *p; };
};
