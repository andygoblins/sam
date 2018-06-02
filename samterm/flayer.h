/* Copyright (c) 1998 Lucent Technologies - All rights reserved. */
#undef None
typedef enum Vis{
    None=0,
    Some,
    All
}Vis;

enum{
    Clicktime=1000      /* one second */
};


typedef struct Flayer Flayer;

/* Flayer is a synchronously updated layer
 * containing a Frame of text and a scroll bar.
 * It may overlap with other Flayers on the screen.
 */
// why does flprepare call flfp0p1 twice?
struct Flayer
{
    /* note that we track background color, but not foreground
     * this is because the foreground color is the same
     * for all frames
     */
    uint64_t    bg;     /* background color */
    Frame       f;      /* text frame */
    int64_t     origin; /* offset of first rune, used to draw scrollbar */
    int64_t     p0, p1; /* selection */
    int64_t     click;  /* time at which selection click occurred, in HZ */
    /* textfn retrieves up to n characters to display.
     * The length of the returned string is stored in np.
     */
    wchar_t     *(*textfn)(Flayer*, int64_t n, uint64_t* np);
    int         user0;  /* custom user data; not used */
    void        *user1; /* custom user data; main.c puts Text buffer here */
    Rectangle   entire; /* of full frame */
    Rectangle   scroll; /* of the scroll bar */
    Vis     visible;    /* visibility status */
};

void    flborder(Flayer*, bool);

/* flclose unmaps the Flayer from the screen
 * and frees its underlying Frame.
 */
void    flclose(Flayer*);

/* fldelete deletes the text between the two points. */
void    fldelete(Flayer*, int64_t, int64_t);

/* flfp0p1 calculates the offset of fl->f.p0 and fl->f.p1
 * and stores it in the provided pointers.
 */
void    flfp0p1(Flayer*, uint64_t* p0, uint64_t* p1);

/* flinit initializes the Frame and moves the Flayer
 * to the foreground.
 */
void    flinit(Flayer*, Rectangle, XftFont*, uint64_t bg);

/* flinsert inserts characters into the Frame */
void    flinsert(Flayer*, wchar_t*, wchar_t*, int64_t);

/* flnew assigns the arguments to the Flayer and adds it 
 * to the global array of Flayers.
 */
void    flnew(Flayer*, wchar_t *(*fn)(Flayer*, int64_t, uint64_t*), int, void*);

int flprepare(Flayer*);

Rectangle flrect(Flayer*, Rectangle);

void    flrefresh(Flayer*, Rectangle, int);

void    flreshape(Rectangle);

/* flselect tracks the mouse to select a string of text in the Frame.
 * returns true if the mouse double-clicks.
 */
bool flselect(Flayer*);

void    flsetselect(Flayer*, int64_t, int64_t);

void    flstart(Rectangle);

/* flupfront moves the Flayer to the foreground.
 * It will be drawn on top of all other Flayers.
 */
void    flupfront(Flayer*);

/* flwhich returns the visible Flayer underneath the given Point */
Flayer  *flwhich(Point);

#define FLMARGIN    4
#define FLSCROLLWID 12
#define FLGAP       4
