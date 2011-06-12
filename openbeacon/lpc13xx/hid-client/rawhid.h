#ifndef rawhid_included_h__
#define rawhid_included_h__

// Raw HID, Basic API
typedef void rawhid_t;
rawhid_t * rawhid_open_only1(int vid, int pid, int usage_page, int usage);
int rawhid_status(rawhid_t *hid);
int rawhid_read(rawhid_t *h, void *buf, int bufsize, int timeout_ms);
int rawhid_write(rawhid_t *hid, const void *buf, int len, int timeout_ms);
void rawhid_close(rawhid_t *h);

#endif
