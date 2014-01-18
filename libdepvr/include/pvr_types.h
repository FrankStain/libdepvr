#pragma once

#pragma pack( push, 1 )
namespace pvr {
	typedef unsigned char		channel_t;
	typedef unsigned int		pixel_t;
	typedef unsigned short int	pixel16_t;

	enum compression_t {
		ct_undefined	= 0,
		ct_2bpp,
		ct_4bpp
	};

	static int32_t w2b_width	= 8;
	static int32_t w2b_height	= 4;
	static int32_t w4b_width	= 4;
	static int32_t w4b_height	= 4;
	static int32_t w2b_hw		= 4;
	static int32_t w2b_hh		= 2;
	static int32_t w4b_hw		= 2;
	static int32_t w4b_hh		= 2;

	union color32_t {
		pixel_t		m_pixel;
		channel_t	m_rgba[4];
		struct {
			channel_t	m_red;
			channel_t	m_green;
			channel_t	m_blue;
			channel_t	m_alpha;
		};

		color32_t() : m_pixel(0) {};
		color32_t( const pixel_t& value ) : m_pixel(value) {};
		color32_t( channel_t r, channel_t g, channel_t b, channel_t a ) : m_red(r), m_green(g), m_blue(b), m_alpha(a) {};

		inline operator pixel_t& () { return m_pixel; };
		inline operator const pixel_t& () const { return m_pixel; };
		inline channel_t& operator [] ( const int index ) { return m_rgba[ index ]; };
		inline const channel_t& operator [] ( const int index ) const { return m_rgba[ index ]; };
	};

	union color16_t {
		pixel16_t		m_pixel;
		struct {
			channel_t	m_red : 4;
			channel_t	m_green : 4;
			channel_t	m_blue : 4;
			channel_t	m_alpha : 4;
		};

		struct {
			channel_t	m_alpha : 4;
			channel_t	m_red : 4;
			channel_t	m_green : 4;
			channel_t	m_blue : 4;

			inline operator const pixel_t () const { return color32_t( m_red * 17, m_green * 17, m_blue * 17, m_alpha * 17 ); };
		}				m_argb_4444;

		struct {
			pixel16_t	m_red : 5;
			pixel16_t	m_green : 6;
			pixel16_t	m_blue : 5;

			inline operator const pixel_t () const { return color32_t( ( (channel_t)m_red << 3 ) | ( m_red & 0x03U ), ( (channel_t)m_green << 2 ) | ( m_green & 0x02U ), ( (channel_t)m_blue << 3 ) | ( m_blue & 0x03U ), 0 ); };
		}				m_565;

		struct {
			pixel16_t	m_red : 5;
			pixel16_t	m_green : 5;
			pixel16_t	m_blue : 5;
			pixel16_t	m_alpha : 1;

			inline operator const pixel_t () const { color32_t( ( (channel_t)m_red << 3 ) | ( m_red & 0x03U ), ( (channel_t)m_green << 3 ) | ( m_green & 0x03U ), ( (channel_t)m_blue << 3 ) | ( m_blue & 0x03U ), ( m_alpha )? 0x00U : 0xFFU ); };
		}				m_5551;

		struct {
			pixel16_t	m_alpha : 1;
			pixel16_t	m_red : 5;
			pixel16_t	m_green : 5;
			pixel16_t	m_blue : 5;
			
			inline operator const pixel_t () const { color32_t( ( (channel_t)m_red << 3 ) | ( m_red & 0x03U ), ( (channel_t)m_green << 3 ) | ( m_green & 0x03U ), ( (channel_t)m_blue << 3 ) | ( m_blue & 0x03U ), ( m_alpha )? 0x00U : 0xFFU ); };
		}				m_argb_5551;

		color16_t() : m_pixel(0) {};
		color16_t( const pixel16_t& value ) : m_pixel(value) {};
		color16_t( channel_t r, channel_t g, channel_t b, channel_t a ){ m_red = r; m_green = g; m_blue = b; m_alpha = a; };

		inline operator pixel16_t& () { return m_pixel; };
		inline operator const pixel16_t& () const { return m_pixel; };
		inline operator const pixel_t () const { return color32_t( m_red * 17, m_green * 17, m_blue * 17, m_alpha * 17 ); };
		inline const channel_t operator [] ( const int index ) const { return 0x0FU & ( m_pixel >> ( 4 * index ) ); };
	};

	struct color24_t {
		channel_t	m_red;
		channel_t	m_green;
		channel_t	m_blue;

		color24_t() : m_red(0), m_green(0), m_blue(0) {};
		color24_t( const pixel_t& value ) { color32_t c( value ); m_red = c.m_red; m_green = c.m_green; m_blue = c.m_blue; };
		color24_t( channel_t r, channel_t g, channel_t b ){ m_red = r; m_green = g; m_blue = b; };

		inline const color24_t& operator = ( const pixel_t& op ) { color32_t c( op ); m_red = c.m_red; m_green = c.m_green; m_blue = c.m_blue; return *this; };

		inline operator pixel_t () { color32_t c( m_red, m_green, m_blue, 0 ); return c; };
		inline operator const pixel_t& () const { color32_t c( m_red, m_green, m_blue, 0 ); return c; };
	};
	
	union pk_color_t {
		uint16_t		m_color;

		struct {
			uint16_t	m_data : 15;
			uint16_t	m_mode : 1;
		}				m_flags;

		struct {
			uint16_t	m_reserved : 1;
			uint16_t	m_blue : 4;
			uint16_t	m_green: 5;
			uint16_t	m_red : 5;
		}				m_RGB554;

		struct {
			uint16_t	m_reserved : 1;
			uint16_t	m_blue : 3;
			uint16_t	m_green : 4;
			uint16_t	m_red : 4;
			uint16_t	m_alpha : 3;
		}				m_RGBA3443;

		inline color32_t color() const;
		inline operator const color32_t () const { return color(); };
	};

	union ex_color_t {
		struct {
			uint32_t	m_red;
			uint32_t	m_green;
			uint32_t	m_blue;
			uint32_t	m_alpha;
		};

		uint32_t		m_rgba[4];

		ex_color_t() {};
		ex_color_t( const uint32_t r, const uint32_t g, const uint32_t b, const uint32_t a ) : m_red(r), m_green(g), m_blue(b), m_alpha(a) {};

		inline operator const color32_t () const								{ return color32_t( (channel_t&)m_red, (channel_t&)m_green, (channel_t&)m_blue, (channel_t&)m_alpha ); };

		inline const ex_color_t operator - ( const ex_color_t& op ) const		{ return ex_color_t( m_red - op.m_red, m_green - op.m_green, m_blue - op.m_blue, m_alpha - op.m_alpha ); };
		inline const ex_color_t operator + ( const ex_color_t& op ) const		{ return ex_color_t( m_red + op.m_red, m_green + op.m_green, m_blue + op.m_blue, m_alpha + op.m_alpha ); };
		inline const ex_color_t operator * ( const uint32_t op ) const			{ return ex_color_t( m_red * op, m_green * op, m_blue * op, m_alpha * op ); };
		inline const ex_color_t operator / ( const uint32_t op ) const			{ return ex_color_t( m_red / op, m_green / op, m_blue / op, m_alpha / op ); };
		inline const ex_color_t& operator *= ( const uint32_t op )				{ m_red *= op; m_green *= op; m_blue *= op; m_alpha *= op; return *this; };
		inline const ex_color_t& operator += ( const uint32_t op )				{ m_red += op; m_green += op; m_blue += op; m_alpha += op; return *this; };
		inline const ex_color_t& operator += ( const ex_color_t& op )			{ m_red += op.m_red; m_green += op.m_green; m_blue += op.m_blue; m_alpha += op.m_alpha; return *this; };

		inline const ex_color_t& operator = ( const color32_t& op )				{ m_red = op.m_red; m_green = op.m_green; m_blue = op.m_blue; m_alpha = op.m_alpha; return *this; };
	};

	union pk_color_set_t {
		uint32_t		m_value;
		pk_color_t		m_colors[2];
	};

	struct ex_color_state_t {
		ex_color_t		m_lo;
		ex_color_t		m_hi;

		inline ex_color_t modulate( uint32_t mod ) const;
	};

	union pk_region_t {
		uint64_t		m_value;
		struct {
			uint32_t	m_mod;
			
			union {
				uint32_t		m_color;
				pk_color_set_t	m_pack;
			};
		};
	};

	#define wrap(V,M) ( ( V ) & ( M ) )
	struct tex_region_t {
		int32_t		left;
		int32_t		top;
		int32_t		right;
		int32_t		bottom;

		tex_region_t() {};
		tex_region_t( const int32_t l, const int32_t t, const int32_t r, const int32_t b ) : left(l), top(t), right(r), bottom(b) {};

		inline void set( const int32_t& x, const int32_t& y, const int32_t& xmargin, const int32_t& ymargin ){
			left	= wrap( x, xmargin );
			top		= wrap( y, ymargin );
			right	= wrap( x + 1, xmargin );
			bottom	= wrap( y + 1, ymargin );
		};

		inline const tex_region_t operator * ( const int32_t op ) const { return tex_region_t( left * op, top * op, right * op, bottom * op ); };
	};
	#undef wrap
};
#pragma pack( pop )
