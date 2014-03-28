#pragma once

namespace pvr {
	template< compression_t mtd >
	struct deflate_stream_t {
		color32_t	m_stream[ constants<mtd>::deflate_stream_size ];
		
		color32_t& color( const int32_t x, const int32_t y );
	};

	inline color32_t& deflate_stream_t<ct_2bpp>::color( const int32_t x, const int32_t y ) { return m_stream[ x + y * constants<ct_2bpp>::width ]; };
	inline color32_t& deflate_stream_t<ct_4bpp>::color( const int32_t x, const int32_t y ) { return m_stream[ y + x * constants<ct_4bpp>::height ]; };

	template< compression_t mtd >
	class deflate_unit_t {
	private:
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
			m_mu.push( rt, constants<mtd>::width, 0 );
			m_mu.push( lb, 0, constants<mtd>::height );
			m_mu.push( rb, constants<mtd>::width, constants<mtd>::height );

			m_cu.push( lt.m_pack, rt.m_pack, lb.m_pack, rb.m_pack );

			for( int32_t y = 0; constants<mtd>::height > y; y++ ){
				const ex_color_state_t* cols = m_cu.memory() + y * constants<mtd>::width;
				for( int32_t x = 0; constants<mtd>::width > x; x++ ){
					m_stream.color( x, y ) = cols[ x ].modulate( m_mu.modulation( x + constants<mtd>::half_width, y + constants<mtd>::half_height ) );
				};
			};
		};

		deflate_stream_t<mtd>& stream() { return m_stream; };
		const pk_region_t* const * regs() const { return m_regs; };
	};
};
