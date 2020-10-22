#include "RockPCH.h"

#include "jpeglib.h"
#pragma comment( lib, "jpeg.lib" )


GLOBAL(void)
write_JPEG_file( char* filename, int quality, int image_width, int image_height, JSAMPLE* image_buffer )
{
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;

	FILE * outfile;
	JSAMPROW row_pointer[1];
	int row_stride;

	cinfo.err = jpeg_std_error(&jerr);

	jpeg_create_compress(&cinfo);

#ifdef UNICODE_ROCK
	if ((outfile = _wfopen(RWCHAR(filename), _RT("wb"))) == NULL)
#else 
	if ((outfile = fopen(filename, "wb")) == NULL)
#endif 
	{
		//	fprintf(stderr, "can't open %s\n", filename);
		return;
	}

	jpeg_stdio_dest(&cinfo, outfile);
	/* Setting the parameters of the output file here */
	cinfo.image_width = image_width;
	cinfo.image_height = image_height;
	cinfo.input_components = 3;
	cinfo.in_color_space = JCS_RGB;
	/* default compression parameters, we shouldn't be worried about these */
	jpeg_set_defaults(&cinfo);
	/* Now do the compression .. */
	jpeg_start_compress(&cinfo, TRUE);
	/* like reading a file, this time write one row at a time */
	while (cinfo.next_scanline < cinfo.image_height)
	{
		row_pointer[0] = &image_buffer[cinfo.next_scanline * cinfo.image_width *  cinfo.input_components];
		jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}
	/* similar to read file, clean up after we're done compressing */
	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);
	fclose(outfile);

}


