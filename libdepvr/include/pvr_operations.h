#pragma once

namespace pvr {
	inline color32_t pk_color_t::color() const {
		color32_t res;

		if( m_flags.m_mode ){
			res.m_alpha	= 0x0FU;
			res.m_blue	= ( m_RGB554.m_blue << 1 ) | ( m_RGB554.m_blue >> 3 );
			res.m_green = m_RGB554.m_green;
			res.m_red	= m_RGB554.m_red;
		}else{
			res.m_red	= ( m_RGBA3443.m_red << 1 ) | ( m_RGBA3443.m_red >> 3 );
			res.m_green	= ( m_RGBA3443.m_green << 1 ) | ( m_RGBA3443.m_green >> 3 );
			res.m_blue	= ( m_RGBA3443.m_blue << 2 ) | ( m_RGBA3443.m_blue >> 1 );
			res.m_alpha	= ( m_RGBA3443.m_alpha << 1 );
		};

		return res;
	};

	inline ex_color_t ex_color_state_t::modulate( uint32_t mod ) const {
		const bool flush_alpha = 10 < mod;
		
		mod = ( flush_alpha )? mod - 10 : mod;
		ex_color_t res( ( ( m_lo * ( 8 - mod ) ) + ( m_hi * mod ) ) / 8 );

		if( flush_alpha ){
			res.m_alpha = 0;
		};

		return res;
	};

	inline void color_unit_t<ct_2bpp>::process_stream(){
		for( int32_t x = 0; c::width > x; x++ ){
			m_reg[6][0]	= m_reg[0][0] * 4;
			m_reg[6][1]	= m_reg[0][1] * 4;
			m_reg[7][0]	= m_reg[2][0] - m_reg[0][0];
			m_reg[7][1]	= m_reg[2][1] - m_reg[0][1];
			
			for( int32_t y = 0; c::height > y; y++ ){
				ex_color_state_t* output = m_stream + x + y * c::width;
						
				output->m_lo.m_red		= ( m_reg[6][0].m_red >> 7 ) + ( m_reg[6][0].m_red >> 2 );
				output->m_lo.m_green		= ( m_reg[6][0].m_green >> 7 ) + ( m_reg[6][0].m_green >> 2 );
				output->m_lo.m_blue		= ( m_reg[6][0].m_blue >> 7 ) + ( m_reg[6][0].m_blue >> 2 );
				output->m_lo.m_alpha		= ( m_reg[6][0].m_alpha >> 5 ) + ( m_reg[6][0].m_alpha >> 1 );

				output->m_hi.m_red		= ( m_reg[6][1].m_red >> 7 ) + ( m_reg[6][1].m_red >> 2 );
				output->m_hi.m_green		= ( m_reg[6][1].m_green >> 7 ) + ( m_reg[6][1].m_green >> 2 );
				output->m_hi.m_blue		= ( m_reg[6][1].m_blue >> 7 ) + ( m_reg[6][1].m_blue >> 2 );
				output->m_hi.m_alpha		= ( m_reg[6][1].m_alpha >> 5 ) + ( m_reg[6][1].m_alpha >> 1 );

				m_reg[6][0] += m_reg[7][0];
				m_reg[6][1] += m_reg[7][1];
			};

			m_reg[0][0] += m_reg[4][0];
			m_reg[0][1] += m_reg[4][1];
			m_reg[2][0] += m_reg[5][0];
			m_reg[2][1] += m_reg[5][1];
		};
	};

	inline void color_unit_t<ct_4bpp>::process_stream(){
		for( int32_t y = 0; c::height > y; y++ ){
			m_reg[6][0]	= m_reg[0][0] * 4;
			m_reg[6][1]	= m_reg[0][1] * 4;
			m_reg[7][0]	= m_reg[2][0] - m_reg[0][0];
			m_reg[7][1]	= m_reg[2][1] - m_reg[0][1];
			ex_color_state_t* output = m_stream + y * c::width;

			for( int32_t x = 0; c::width > x; x++ ){
				output->m_lo.m_red		= ( m_reg[6][0].m_red >> 6 ) + ( m_reg[6][0].m_red >> 1 );
				output->m_lo.m_green		= ( m_reg[6][0].m_green >> 6 ) + ( m_reg[6][0].m_green >> 1 );
				output->m_lo.m_blue		= ( m_reg[6][0].m_blue >> 6 ) + ( m_reg[6][0].m_blue >> 1 );
				output->m_lo.m_alpha		= ( m_reg[6][0].m_alpha >> 4 ) + m_reg[6][0].m_alpha;

				output->m_hi.m_red		= ( m_reg[6][1].m_red >> 6 ) + ( m_reg[6][1].m_red >> 1 );
				output->m_hi.m_green		= ( m_reg[6][1].m_green >> 6 ) + ( m_reg[6][1].m_green >> 1 );
				output->m_hi.m_blue		= ( m_reg[6][1].m_blue >> 6 ) + ( m_reg[6][1].m_blue >> 1 );
				output->m_hi.m_alpha		= ( m_reg[6][1].m_alpha >> 4 ) + m_reg[6][1].m_alpha;

				m_reg[6][0] += m_reg[7][0];
				m_reg[6][1] += m_reg[7][1];
				output++;
			};

			m_reg[0][0] += m_reg[4][0];
			m_reg[0][1] += m_reg[4][1];
			m_reg[2][0] += m_reg[5][0];
			m_reg[2][1] += m_reg[5][1];
		};
	};

	inline void modulation_unit_t<ct_2bpp>::push( const pk_region_t& cs, const uint32_t left, const uint32_t top ){
		uint32_t mod_bits = cs.m_mod;
		uint32_t wrd_mode = 0x01U & cs.m_color;

		if( wrd_mode ){
			if( 0x01U & mod_bits ){
				wrd_mode = ( 0x100000U & mod_bits )? 3 : 2;
				mod_bits = ( 0x200000U & mod_bits )? ( mod_bits | 0x100000U ) : ( mod_bits & ~0x100000U );
			};

			mod_bits = ( 0x02U & mod_bits )? ( mod_bits | 0x01U ) : ( mod_bits & ~0x01U );

			for( int32_t y = 0; c::height > y; y++ ){
				for( int32_t x = 0; c::width > x; x++ ){
					m_modes[ left + x ][ top + y ] = wrd_mode;
							
					if( !( 0x01U & ( x ^ y ) ) ){
						m_values[ left + x ][ top + y ] = mod_bits & 0x03U;
						mod_bits >>= 2;
					};
				};
			};
		}else{
			for( int32_t y = 0; c::height > y; y++ ){
				for( int32_t x = 0; c::width > x; x++ ){
					m_modes[ left + x ][ top + y ] = wrd_mode;
					m_values[ left + x ][ top + y ] = ( 0x01U & mod_bits )? 0x03U : 0x00U;
					mod_bits >>= 1;
				};
			};
		};
	};

	inline void modulation_unit_t<ct_4bpp>::push( const pk_region_t& cs, const uint32_t left, const uint32_t top ){
		static const uint32_t rvs[] = { 0, 4, 14, 8 };

		uint32_t mod_bits = cs.m_mod;
		uint32_t wrd_mode = 0x01U & cs.m_color;

		for( int32_t y = 0; c::height > y; y++ ){
			uint32_t* output = &m_values[ top + y ][ left ];
					
			for( int32_t x = 0; c::width > x; x++ ){
				*output = mod_bits & 0x03U;

				if( wrd_mode ){
					switch( *output ){
						case 1:
						case 2:
						case 3:
							*output = rvs[ *output ];
						break;
					};
				}else{
					if( 3 < ( *output *= 3 ) ){
						(*output)--;
					};
				};

				mod_bits >>= 2;
				output++;
			};
		};
	};

	inline const uint32_t modulation_unit_t<ct_2bpp>::modulation( const uint32_t left, const uint32_t top ) const {
		static const uint32_t rvs[] = { 0, 3, 5, 8 };

		const uint32_t& mode	= m_modes[ left ][ top ];
		const uint32_t& value	= m_values[ left ][ top ];

		uint32_t res = 0;

		if( ( 0 == mode ) || ( 0 == ( 0x01U & ( left ^ top ) ) ) ){
			res = rvs[ value ];
		}else{
			switch( mode ){
				case 1: res = (
						2 +
						rvs[ m_values[ left ][ top - 1 ] ] +
						rvs[ m_values[ left ][ top + 1 ] ] +
						rvs[ m_values[ left - 1 ][ top ] ] +
						rvs[ m_values[ left + 1 ][ top ] ]
					) / 4;
				break;
				case 2: res = (
						1 +
						rvs[ m_values[ left - 1 ][ top ] ] +
						rvs[ m_values[ left + 1 ][ top ] ]
					) / 2;
				break;
				default: res = (
						1 +
						rvs[ m_values[ left ][ top - 1 ] ] +
						rvs[ m_values[ left ][ top + 1 ] ]
					) / 2;
				break;
			};
		};

		return res;
	};

	inline const uint32_t modulation_unit_t<ct_4bpp>::modulation( const uint32_t left, const uint32_t top ) const {
		return m_values[ left ][ top ];
	};
};
