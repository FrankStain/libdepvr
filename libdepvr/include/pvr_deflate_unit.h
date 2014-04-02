#pragma once

namespace pvr {
	template< compression_t mtd >
	struct deflate_stream_t {
		typedef constants<mtd> c;
		
		color32_t	m_stream[ c::deflate_stream_size ];
		
		color32_t& color( const int32_t x, const int32_t y );
	};

	inline color32_t& deflate_stream_t<ct_2bpp>::color( const int32_t x, const int32_t y ) { return m_stream[ x + y * c::width ]; };
	inline color32_t& deflate_stream_t<ct_4bpp>::color( const int32_t x, const int32_t y ) { return m_stream[ y + x * c::height ]; };

	template< compression_t mtd >
	class deflate_unit_t {
	private:
		typedef constants<mtd> c;
		
		const pk_region_t*		m_regs[4];
		color_unit_t<mtd>		m_cu;
		modulation_unit_t<mtd>	m_mu;
		deflate_stream_t<mtd>	m_stream;

	public:
		deflate_unit_t() {};

		inline void push( const pk_region_t& lt, const pk_region_t& rt, const pk_region_t& lb, const pk_region_t& rb ){
			m_regs[0] = &lt;
			m_regs[1] = &rt;
			m_regs[2] = &lb;
			m_regs[3] = &rb;

			m_mu.push( lt, 0, 0 );
			m_mu.push( rt, c::width, 0 );
			m_mu.push( lb, 0, c::height );
			m_mu.push( rb, c::width, c::height );

			m_cu.push( lt.m_pack, rt.m_pack, lb.m_pack, rb.m_pack );

			for( int32_t y = 0; c::height > y; y++ ){
				const ex_color_state_t* cols = m_cu.memory() + y * c::width;
				for( int32_t x = 0; c::width > x; x++ ){
					m_stream.color( x, y ) = cols[ x ].modulate( m_mu.modulation( x + c::half_width, y + c::half_height ) );
				};
			};
		};

		deflate_stream_t<mtd>& stream() { return m_stream; };
		const pk_region_t* const * regs() const { return m_regs; };
	};
};
