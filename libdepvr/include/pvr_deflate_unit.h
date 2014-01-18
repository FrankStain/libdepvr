#pragma once

namespace pvr {
	template< compression_t mtd >
	struct deflate_stream_t {
		color32_t& color( const int32_t x, const int32_t y );
	};

	template<>
	struct deflate_stream_t<ct_2bpp> {
		color32_t	m_stream[32];

		color32_t& color( const int32_t x, const int32_t y ){ return m_stream[ x + y * w2b_width ]; };
	};

	template<>
	struct deflate_stream_t<ct_4bpp> {
		color32_t	m_stream[16];

		color32_t& color( const int32_t x, const int32_t y ){ return m_stream[ y + x * w4b_height ]; };
	};

	
	template< compression_t mtd >
	class deflate_unit_t {
	private:
		const pk_region_t*		m_regs[4];
		color_unit_t<mtd>		m_cu;
		modulation_unit_t<mtd>	m_mu;
		deflate_stream_t<mtd>	m_stream;

	public:
		deflate_unit_t() {};

		inline void push( const pk_region_t& lt, const pk_region_t& rt, const pk_region_t& lb, const pk_region_t& rb );

		deflate_stream_t<mtd>& stream() { return m_stream; };
		const pk_region_t* const * regs() const { return m_regs; };
	};
};
