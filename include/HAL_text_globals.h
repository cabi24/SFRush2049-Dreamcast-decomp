#ifndef HAL_TEXT_GLOBALS_H
#define HAL_TEXT_GLOBALS_H
/* Target-backed references, not definitions or placement devices.
   133944..133948 remains unidentified and unallocated by this unit.
   Array bounds follow MAT3 object declarations plus DC index/limit evidence;
   no complete original BSS allocation/end is asserted. */
extern unsigned int halTextOldMask; /* target 8C133938 */
#define gOldMask halTextOldMask
extern int halTextFontDatWaveInst; /* target 8C13393C */
#define gFontDatWaveInst halTextFontDatWaveInst
extern int halTextFontTexWaveInst; /* target 8C133940 */
#define gFontTexWaveInst halTextFontTexWaveInst
extern anon_version_type_10 * halTextFontHeader; /* target 8C133948 */
#define gFontHeader halTextFontHeader
extern anon_kerns_value_c * halTextFontLetters; /* target 8C13394C */
#define gFontLetters halTextFontLetters
extern tTexture * halTextFontTextures[16]; /* target 8C133950 */
#define gFontTextures halTextFontTextures
extern signed short halTextCharMap[256]; /* target 8C133990 */
#define gCharMap halTextCharMap
extern anon_fore_4 halTextFontColor; /* target 8C133B90 */
#define gFontColor halTextFontColor
extern signed char halTextLeading; /* target 8C133B94 */
#define gLeading halTextLeading
extern signed char halTextSpacing; /* target 8C133B95 */
#define gSpacing halTextSpacing
extern int halTextFontAttribute; /* target 8C133B98 */
#define gFontAttribute halTextFontAttribute
extern signed short halTextFontXpos; /* target 8C133B9C */
#define gFontXpos halTextFontXpos
extern signed short halTextFontYpos; /* target 8C133B9E */
#define gFontYpos halTextFontYpos
extern signed char halTextFontIndex; /* target 8C133BA0 */
#define gFontIndex halTextFontIndex
extern int halTextBufLen; /* target 8C133BA4 */
#define gBufLen halTextBufLen
extern unsigned char halTextBuffer[800]; /* target 8C133BA8 */
#define gBuffer halTextBuffer
extern anon_fore_4 halTextLastFontColor; /* target 8C133EC8 */
#define lastFontColor halTextLastFontColor
extern float halTextLastTrans; /* target 8C133ECC */
#define lastTrans halTextLastTrans
extern signed short halTextLastFontXpos; /* target 8C133ED0 */
#define lastFontXpos halTextLastFontXpos
extern signed short halTextLastFontYpos; /* target 8C133ED2 */
#define lastFontYpos halTextLastFontYpos
extern signed char halTextLastFontIndex; /* target 8C133ED4 */
#define lastFontIndex halTextLastFontIndex
extern signed char halTextLastLeading; /* target 8C133ED5 */
#define lastLeading halTextLastLeading
extern signed char halTextLastSpacing; /* target 8C133ED6 */
#define lastSpacing halTextLastSpacing
#endif
