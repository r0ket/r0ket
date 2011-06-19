/* Raw HID I/O Routines
 * Copyright 2008, PJRC.COM, LLC
 * paul@pjrc.com
 *
 * You may redistribute this program and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/
 */


// This code will someday be turned into "librawhid", an easy-to-use
// and truly cross platform library for accessing HID reports.  But
// there are many complexities not properly handled by this simple
// code that would be expected from a high quality library.  In
// particular, how report IDs are handled is not uniform on the 3
// platforms.  The mac code uses a single buffer which assumes no
// other functions can cause the "run loop" to process HID callbacks.
// The linux version doesn't extract usage and usage page from the
// report descriptor and just hardcodes a signature for the Teensy
// USB debug example.  Lacking from all platforms are functions to
// manage multiple devices and robust detection of device removal
// and attachment.  There are probably lots of other issues... this
// code has really only been used in 2 projects.  If you use it,
// please report bugs to paul@pjrc.com


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "rawhid.h"

#ifdef OPERATING_SYSTEM
#undef OPERATING_SYSTEM
#endif


/*************************************************************************/
/**                                                                     **/
/**                             Linux                                   **/
/**                                                                     **/
/*************************************************************************/

#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/hidraw.h>

struct rawhid_struct {
	int fd;
	int name;
	int isok;
};

rawhid_t * rawhid_open_only1(int vid, int pid, int usage_page, int usage)
{
	struct rawhid_struct *hid;
	struct stat devstat;
	struct hidraw_devinfo info;
	struct hidraw_report_descriptor *desc;
	char buf[512];
	int r, i, fd=-1, len, found=0;

	printf("Searching for device using hidraw....\n");
	for (i=0; i<HIDRAW_MAX_DEVICES; i++) {
		if (fd > 0) close(fd);
		snprintf(buf, sizeof(buf), "/dev/hidraw%d", i);
		r = stat(buf, &devstat);
		if (r < 0) continue;
		printf("device: %s\n", buf);
		fd = open(buf, O_RDWR);
		if (fd < 0) continue;
		printf("  opened\n");
		r = ioctl(fd, HIDIOCGRAWINFO, &info);
		if (r < 0) continue;
		printf("  vid=%04X, pid=%04X\n", info.vendor & 0xFFFF, info.product & 0xFFFF);
		r = ioctl(fd, HIDIOCGRDESCSIZE, &len);
		if (r < 0 || len < 1) continue;
		printf("  len=%u\n", len);
		desc = (struct hidraw_report_descriptor *)buf;
		if (len > sizeof(buf)-sizeof(int)) len = sizeof(buf)-sizeof(int);
		desc->size = len;
		r = ioctl(fd, HIDIOCGRDESC, desc);
		if (r < 0) continue;
		printf("  Match\n");
		found = 1;
		break;
	}
	if (!found) {
		if (fd > 0) close(fd);
		return NULL;
	}
	hid = (struct rawhid_struct *)malloc(sizeof(struct rawhid_struct));
	if (!hid) {
		close(fd);
		return NULL;
	}
	hid->fd = fd;
	return hid;
}

int rawhid_status(rawhid_t *hid)
{
	// TODO: how to check if device is still online?
	return -1;
}

int rawhid_read(rawhid_t *h, void *buf, int bufsize, int timeout_ms)
{
	struct rawhid_struct *hid;
	int num;

	hid = (struct rawhid_struct *)h;
	if (!hid || hid->fd < 0) return -1;

	while (1) {
		num = read(hid->fd, buf, bufsize);
		if (num < 0) {
			if (errno == EINTR || errno == EAGAIN) continue;
			if (errno == EIO) {
				return -1;
				printf("I/O Error\n");
			}
			printf("read error, r=%d, errno=%d\n", num, errno);
			return -1;
		}
		//printf("read %d bytes\n", num);
		return num;
	}
}

void rawhid_close(rawhid_t *h)
{
	struct rawhid_struct *hid;

	hid = (struct rawhid_struct *)h;
	if (!hid || hid->fd < 0) return;
	close(hid->fd);
	hid->fd = -1;
}
