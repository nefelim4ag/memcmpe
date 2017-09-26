#include <stdio.h>
#include <inttypes.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define PAGE_SIZE 4097

static int memcmpe(const void *s1, const void *s2, size_t len, size_t *offset) {
        size_t i = 0;
        size_t iter = 1024;
        const size_t max_error = 256;
        uint8_t *ps1 = (uint8_t *) s1;
        uint8_t *ps2 = (uint8_t *) s2;
        int ret = 0;

        if (offset == NULL) {
                ret = memcmp(s1, s2, len);
        } else {
                if (*offset <= len)
                        i = *offset;

                while (i < len) {

                        if (len-i < iter)
                                iter = len-i;

                        ret = memcmp(&ps1[i], &ps2[i], iter);
                        if (ret) {
                                if (iter < max_error)
                                        break;
                                iter /= 2;
                                continue;
                        }
                        i+= iter;
                }

                *offset = i;
        }

        return ret;
}

int main() {
        uint8_t PAGE1[PAGE_SIZE];
        uint8_t PAGE2[PAGE_SIZE];
        uint64_t iter = 1024;
        uint64_t i;
        clock_t start, end;
        size_t offset = 0;
        int ret;
        srand(time(NULL));


        iter *= 1024*1024*4/PAGE_SIZE;

        for (i = 0; i < PAGE_SIZE; i++) {
                PAGE1[i] = i%256;
                PAGE2[i] = i%256;
        }

        PAGE2[PAGE_SIZE-1] = 32;
        PAGE1[PAGE_SIZE-1] = 32;
        PAGE1[PAGE_SIZE-1356] = 1;
        //PAGE2[PAGE_SIZE-1356] = 1;

        printf("PAGE_SIZE: %u, loop count: %lu\n", PAGE_SIZE, iter);

{
        start = clock()*1000000/CLOCKS_PER_SEC;
        for (i = 0; i < iter; i++) {
                ret = memcmpe(&PAGE1, &PAGE2, PAGE_SIZE, NULL);
                PAGE1[PAGE_SIZE-1] = ret ^ 127;
                PAGE2[PAGE_SIZE-1] = ret ^ 127;
        }
        end = clock()*1000000/CLOCKS_PER_SEC;

        printf("memcmp:  %i\t\t\ttime: %lu ms,\tth: %f MiB/s\n", ret, (end - start)/1000, PAGE_SIZE*iter*1.0/(end - start));
}

        for (i = 0; i < PAGE_SIZE; i++) {
                PAGE1[i] = i%256;
                PAGE2[i] = i%256;
        }

        PAGE2[PAGE_SIZE-1] = 32;
        PAGE1[PAGE_SIZE-1] = 32;
        PAGE1[PAGE_SIZE-1356] = 1;
        //PAGE2[PAGE_SIZE-1356] = 1;

{
        start = clock()*1000000/CLOCKS_PER_SEC;
        for (i = 0; i < iter; i++) {
                offset=0;
                ret = memcmpe(&PAGE1, &PAGE2, PAGE_SIZE, &offset);
                PAGE1[PAGE_SIZE-1] = ret ^ 127;
                PAGE2[PAGE_SIZE-1] = ret ^ 127;
        }
        end = clock()*1000000/CLOCKS_PER_SEC;

        printf("memcmpe: %i, offset: %lu\ttime: %lu ms,\tth: %f MiB/s\n", ret, offset, (end - start)/1000, PAGE_SIZE*iter*1.0/(end - start));
}

        for (i = 0; i < PAGE_SIZE; i++) {
                PAGE1[i] = i%256;
                PAGE2[i] = i%256;
        }

        PAGE2[PAGE_SIZE-1] = 32;
        PAGE1[PAGE_SIZE-1] = 32;
        PAGE1[PAGE_SIZE-1356] = 1;
        //PAGE2[PAGE_SIZE-1356] = 1;

{
        offset = 0;
        start = clock()*1000000/CLOCKS_PER_SEC;
        for (i = 0; i < iter; i++) {
                //offset=0;
                ret = memcmpe(&PAGE1, &PAGE2, PAGE_SIZE, &offset);
                PAGE1[PAGE_SIZE-1] = ret ^ 127;
                PAGE2[PAGE_SIZE-1] = ret ^ 127;
        }
        end = clock()*1000000/CLOCKS_PER_SEC;

        printf("memcmpe: %i, offset: %lu\ttime: %lu ms,\tth: %f MiB/s\n", ret, offset, (end - start)/1000, PAGE_SIZE*iter*1.0/(end - start));
}

        return 0;
}
