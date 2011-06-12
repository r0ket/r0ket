/***************************************************************
 *
 * OpenPCD.org - main entry for LPC flashing tool
 *
 * Copyright 2010 Milosch Meriac <meriac@bitmanufaktur.de>
 * Copyright 2010 Henryk Plötz <henryk@ploetzli.ch>
 *
 ***************************************************************

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; version 3.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License along
 with this program; if not, write to the Free Software Foundation, Inc.,
 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

 */

/* Library includes. */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>

/* static buffer for the maximal flash size: 32 kibibytes
 */
static uint8_t data[32 * 1024];

static void print_usage(char *progname)
{
	fprintf(stderr,
			"usage: %s inputfirmware.bin \"/media/CRP DISABLD/firmware.bin\"\n\n"
			"Either or both input and output may be -, which indicates to work\n"
			"on stdin or stdout\n\n", progname);
}

static int parse_arguments(int argc, char **argv, int *input, int *output)
{
	if (argc != 3) {
		print_usage(argv[0]);
		return 0;
	}

	/* open input file */
	if (strcmp(argv[1], "-") != 0) {
		if ((*input = open(argv[1], O_RDONLY)) == -1) {
			perror("Can't open input file for reading");
			return 0;
		}
	} else {
		*input = 0;
	}

	/* open output file */
	if (strcmp(argv[2], "-") != 0) {
		if ((*output = open(argv[2], O_WRONLY | O_CREAT, 00664)) == -1) {
			perror("Can't open output file for writing");
			close(*input);
			return 0;
		}
	} else {
		*output = 1;
	}

	return 1;
}

/**
 * Converts Little-Endian data to host byte order
 * \param ledata is a 32-bit integer stored in little endian order
 * \return \e ledata converted to host byte order
 */
uint32_t letohl(uint32_t ledata)
{
	/* first convert input to big endian, then use ntohl */
	uint32_t bedata =
		(((ledata >> 0) & 0xff) << 24) | (((ledata >> 8) & 0xff) << 16) |
		(((ledata >> 16) & 0xff) << 8) | (((ledata >> 24) & 0xff) << 0);
	return ntohl(bedata);
}

/**
 * Converts host byte order data to Little-Endian
 * \param hdata is a 32-bit integer stored in host byte order
 * \return \e hdata converted to little endian order
 */
uint32_t htolel(uint32_t hdata)
{
	/* use htonl, then convert from big endian */
	uint32_t bedata = htonl(hdata);
	return (((bedata >> 0) & 0xff) << 24) | (((bedata >> 8) & 0xff) << 16)
		| (((bedata >> 16) & 0xff) << 8) | (((bedata >> 24) & 0xff) << 0);
}


int main(int argc, char **argv)
{
	int input = -1, output = -1;

	if (!parse_arguments(argc, argv, &input, &output)) {
		return 1;
	}

	/* Read in the entire input file */
	ssize_t len_read, input_size = 0;
	while ((len_read =
			read(input, data + input_size,
				 sizeof(data) - input_size)) > 0) {
		input_size += len_read;
	}

	if (input_size < 8 * 4) {
		fprintf(stderr,
				"Error: Input file is too small, does not contain \n"
				"at least 8 interrupt vectors\n");
		print_usage(argv[0]);
		return 2;
	}

	/* Now sum over the first 7 ISR values */
	uint32_t accumulator = 0, isr_value;
	for (int i = 0; i < 7; i++) {
		isr_value = ((uint32_t *) data)[i];
		/* The contents in the file are stored in little endian, need to
		 * convert to host byte order.
		 */
		isr_value = letohl(isr_value);

		accumulator += isr_value;
	}

	/* The checksum is the twos complement of the sum */
	uint32_t checksum = ~accumulator + 1;

	/* Needs to be converted to little endian when storing */
	((uint32_t *) data)[7] = htolel(checksum);

	/* Now write out the whole file */
	ssize_t len_written, output_size = 0;
	while ((len_written =
			write(output, data + output_size,
				  input_size - output_size)) > 0) {
		output_size += len_written;
	}

	if (output_size != input_size) {
		perror("couldn't write complete output file, flash size might be"
			   "smaller than input size");
	}

	/* close & flush files */
	close(input);
	fsync(output);
	close(output);

	return 0;
}
