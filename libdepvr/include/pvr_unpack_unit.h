#pragma once

namespace pvr {
	class basic_unpack_unit_t {
	protected:
		int32_t			m_pos[2];
		int32_t			m_words[2];
		int32_t			m_size[2];
		int32_t			m_margin[2];
		int32_t			m_twiddle[2];

		tex_region_t	m_rect;
		
		inline void next_step(){
			if( *this ){
				m_rect.set( m_pos[0], m_pos[1], m_margin[0], m_margin[1] );

				if( !( m_margin[0] > ++m_pos[0] ) ){
					m_pos[1]++;
					m_pos[0] = -1;
				};
			};
		};

		inline const int32_t twiddle( const int32_t x, const int32_t y ){
			int32_t res	= 0;
			int32_t db	= 1;
			int32_t sc	= 0;

			for( int32_t sb = 1; m_twiddle[0] > sb; sb <<= 1, db <<= 2, sc++ ){
				if( y & sb ){
					res |= db;
				};

				if( x & sb ){
					res |= ( db << 1 );
				};
			};

			db = ( ( m_twiddle[1] )? y : x ) >> sc;
			return res | ( db << ( sc << 1 ) );
		};

	public:
		basic_unpack_unit_t( const int32_t dim_x, const int32_t dim_y, const int32_t ww, const int32_t wh ){
			m_size[0]	= dim_x;
			m_size[1]	= dim_y;
			m_words[0]	= dim_x / ww;
			m_words[1]	= dim_y / wh;
			m_margin[0]	= m_words[0] - 1;
			m_margin[1]	= m_words[1] - 1;
			m_pos[0]	= -1;
			m_pos[1]	= -1;

			if( m_words[0] > m_words[1] ){
				m_twiddle[0]	= m_words[1];
				m_twiddle[1]	= 0;
			}else{
				m_twiddle[0]	= m_words[0];
				m_twiddle[1]	= 1;
			};
		};

		virtual const uint32_t processed() const = 0;
		virtual void push( const pk_region_t* regs ) = 0;
		virtual void pop( color32_t* pixels ) = 0;

		inline operator const bool () const { return m_margin[1] > m_pos[1]; };
		inline void operator ++ ( const int variant ){ if( !variant ){ next_step(); }; };
	};

	template< compression_t mtd >
	class unpack_unit_t : public basic_unpack_unit_t {
	protected:
		typedef constants<mtd> c;
		
		deflate_unit_t<mtd>	m_du;

	public:
		unpack_unit_t( const int32_t dim_x, const int32_t dim_y ) : basic_unpack_unit_t( dim_x, dim_y, c::width, c::height ) {};
		
		virtual inline void push( const pk_region_t* regs ){
			const bool use_cache = m_rect.right > m_rect.left;

			m_du.push(
				( use_cache )? *m_du.regs()[1] : regs[ twiddle( m_rect.left, m_rect.top ) ],
				regs[ twiddle( m_rect.right, m_rect.top ) ],
				( use_cache )? *m_du.regs()[3] : regs[ twiddle( m_rect.left, m_rect.bottom ) ],
				regs[ twiddle( m_rect.right, m_rect.bottom ) ]
			);
		};

		virtual inline void pop( color32_t* pixels ){
			const tex_region_t wr(
				m_rect.left * c::width,
				m_rect.top * c::height,
				m_rect.right * c::width,
				m_rect.bottom * c::height
			);

			for( int32_t y = 0; c::half_height > y; y++ ){
				color32_t* tr	= pixels + ( m_size[0] * ( wr.top + y + c::half_height ) );
				color32_t* br	= pixels + ( m_size[0] * ( wr.bottom + y ) );
				color32_t* ts	= m_du.stream().m_stream + y * c::width;
				color32_t* bs	= m_du.stream().m_stream + ( y + c::half_height ) * c::width;
			
				for( int32_t x = 0; c::half_width > x; x++ ){
					register int32_t row_off	= wr.left + x + c::half_width;
					register int32_t stk_off	= x + c::half_width;
					tr[ row_off ]				= ts[ x ];
					br[ row_off ]				= bs[ x ];
					row_off						= wr.right + x;
					tr[ row_off ]				= ts[ stk_off ];
					br[ row_off ]				= bs[ stk_off ];
				};
			};
		};

		virtual const uint32_t processed() const		{ return ( m_size[0] * m_size[1] ) / c::half_width; };
	};
};
