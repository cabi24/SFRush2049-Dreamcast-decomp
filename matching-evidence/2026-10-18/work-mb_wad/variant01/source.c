/* mb_wad.c - "wad" resource files: a byte-swapped table of contents of (tag, address, length)
   entries, relocated in place to a load address. */
#include "..\include\mb_wad_types.h"

/* 0x800288ec */
void MBSetupWad(anon_base_TOCentries_c *wad, void *base)
{
    int i;
    anon_tag_address_c *e;
    int fixup;

    if (!wad)
        return;
    fixup = (unsigned int)base;
    wad->base = base;
    wad->TOC = (anon_tag_address_c *)wad->base[0];
    wad->TOCentries = wad->base[1];
    wad->TOC = (anon_tag_address_c *)((unsigned int)wad->TOC + fixup);
    wad->base[0] = (unsigned int)wad->TOC;
    for (i = 0; i < wad->TOCentries; i++) {
        e = &wad->TOC[i];
        e->address += fixup;
    }
}

/* 0x800289b4 */
void MBOffsetWad(anon_base_TOCentries_c *wad, void *base, int offset)
{
    int i;
    anon_tag_address_c *e;

    if (!wad)
        return;
    base = (char *)base + offset;
    wad->base = base;
    *(unsigned int *)base += offset;
    wad->TOC = (anon_tag_address_c *)wad->base[0];
    wad->TOCentries = wad->base[1];
    for (i = 0; i < wad->TOCentries; i++) {
        e = &wad->TOC[i];
        e->address += offset;
    }
}

/* 0x80028a78 */
anon_tag_address_c *MBFindInWad(anon_base_TOCentries_c *wad, unsigned int tag)
{
    int i;

    if (!wad)
        return 0;
    for (i = 0; i < wad->TOCentries; i++) {
        if (wad->TOC[i].tag == tag)
            return &wad->TOC[i];
    }
    return 0;
}

/* 0x80028ad0 */
void *MBGetFromWad(anon_base_TOCentries_c *wad, unsigned int tag, int *length)
{
    anon_tag_address_c *e = 0;

    if (wad)
        e = MBFindInWad(wad, tag);

    if (!e) {
        if (length)
            *length = 0;
        return 0;
    }
    if (length)
        *length = e->length;
    return (void *)e->address;
}
