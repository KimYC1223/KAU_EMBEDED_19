
module VGA_Subsystem (
	char_buffer_control_slave_address,
	char_buffer_control_slave_byteenable,
	char_buffer_control_slave_read,
	char_buffer_control_slave_write,
	char_buffer_control_slave_writedata,
	char_buffer_control_slave_readdata,
	char_buffer_slave_address,
	char_buffer_slave_clken,
	char_buffer_slave_chipselect,
	char_buffer_slave_write,
	char_buffer_slave_readdata,
	char_buffer_slave_writedata,
	char_buffer_slave_byteenable,
	pixel_dma_control_slave_address,
	pixel_dma_control_slave_byteenable,
	pixel_dma_control_slave_read,
	pixel_dma_control_slave_write,
	pixel_dma_control_slave_writedata,
	pixel_dma_control_slave_readdata,
	pixel_dma_master_readdatavalid,
	pixel_dma_master_waitrequest,
	pixel_dma_master_address,
	pixel_dma_master_lock,
	pixel_dma_master_read,
	pixel_dma_master_readdata,
	sys_clk_clk,
	sys_reset_reset_n,
	vga_CLK,
	vga_HS,
	vga_VS,
	vga_BLANK,
	vga_SYNC,
	vga_R,
	vga_G,
	vga_B,
	vga_pixel_dma_avalon_pixel_source_ready,
	vga_pixel_dma_avalon_pixel_source_startofpacket,
	vga_pixel_dma_avalon_pixel_source_endofpacket,
	vga_pixel_dma_avalon_pixel_source_valid,
	vga_pixel_dma_avalon_pixel_source_data,
	vga_pll_ref_clk_clk,
	vga_pll_ref_reset_reset);	

	input	[1:0]	char_buffer_control_slave_address;
	input	[3:0]	char_buffer_control_slave_byteenable;
	input		char_buffer_control_slave_read;
	input		char_buffer_control_slave_write;
	input	[31:0]	char_buffer_control_slave_writedata;
	output	[31:0]	char_buffer_control_slave_readdata;
	input	[10:0]	char_buffer_slave_address;
	input		char_buffer_slave_clken;
	input		char_buffer_slave_chipselect;
	input		char_buffer_slave_write;
	output	[31:0]	char_buffer_slave_readdata;
	input	[31:0]	char_buffer_slave_writedata;
	input	[3:0]	char_buffer_slave_byteenable;
	input	[1:0]	pixel_dma_control_slave_address;
	input	[3:0]	pixel_dma_control_slave_byteenable;
	input		pixel_dma_control_slave_read;
	input		pixel_dma_control_slave_write;
	input	[31:0]	pixel_dma_control_slave_writedata;
	output	[31:0]	pixel_dma_control_slave_readdata;
	input		pixel_dma_master_readdatavalid;
	input		pixel_dma_master_waitrequest;
	output	[31:0]	pixel_dma_master_address;
	output		pixel_dma_master_lock;
	output		pixel_dma_master_read;
	input	[7:0]	pixel_dma_master_readdata;
	input		sys_clk_clk;
	input		sys_reset_reset_n;
	output		vga_CLK;
	output		vga_HS;
	output		vga_VS;
	output		vga_BLANK;
	output		vga_SYNC;
	output	[7:0]	vga_R;
	output	[7:0]	vga_G;
	output	[7:0]	vga_B;
	input		vga_pixel_dma_avalon_pixel_source_ready;
	output		vga_pixel_dma_avalon_pixel_source_startofpacket;
	output		vga_pixel_dma_avalon_pixel_source_endofpacket;
	output		vga_pixel_dma_avalon_pixel_source_valid;
	output	[7:0]	vga_pixel_dma_avalon_pixel_source_data;
	input		vga_pll_ref_clk_clk;
	input		vga_pll_ref_reset_reset;
endmodule
