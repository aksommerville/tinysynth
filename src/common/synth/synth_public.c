#include "synth_internal.h"

//XXX very temp, just making some noise to verify for reals.
uint32_t p=0;
uint32_t dp=0;
uint8_t gchid=0,gnoteid=0;

/* Init.
 */
 
void synth_init(uint32_t rate) {
}

/* Update.
 */

void synth_update(int16_t *v,int c) {
  memset(v,0,c<<1);
  if (!dp) return;
  p+=dp;
  *v=(p&0x80000000)?-8000:8000;
}

/* Realtime event.
 */

void synth_event_system(uint8_t e) {
//TODO
}

/* Note Off.
 */
 
void synth_event_note_off(uint8_t chid,uint8_t noteid,uint8_t velocity) {
//TODO
  if ((chid==gchid)&&(noteid==gnoteid)) dp=0;
}

/* Note On.
 */
static uint32_t minisyni_ratev[128]={
  2125742,2252146,2386065,2527948,2678268,2837526,3006254,3185015,3374406,3575058,3787642,4012867,4251485,
  4504291,4772130,5055896,5356535,5675051,6012507,6370030,6748811,7150117,7575285,8025735,8502970,9008582,
  9544261,10111792,10713070,11350103,12025015,12740059,13497623,14300233,15150569,16051469,17005939,18017165,
  19088521,20223584,21426141,22700205,24050030,25480119,26995246,28600467,30301139,32102938,34011878,36034330,
  38177043,40447168,42852281,45400411,48100060,50960238,53990491,57200933,60602276,64205876,68023757,72068660,
  76354085,80894335,85704563,90800821,96200119,101920476,107980983,114401866,121204555,128411753,136047513,
  144137319,152708170,161788671,171409126,181601643,192400238,203840952,215961966,228803732,242409110,256823506,
  272095026,288274639,305416341,323577341,342818251,363203285,384800477,407681904,431923931,457607465,484818220,
  513647012,544190053,576549277,610832681,647154683,685636503,726406571,769600953,815363807,863847862,915214929,
  969636441,1027294024,1088380105,1153098554,1221665363,1294309365,1371273005,1452813141,1539201906,1630727614,
  1727695724,1830429858,1939272882,2054588048,2176760211,2306197109,2443330725,2588618730,2742546010,2905626283,
  3078403812,3261455229,
};
 
void synth_event_note_on(uint8_t chid,uint8_t noteid,uint8_t velocity) {
//TODO
  gchid=chid;
  gnoteid=noteid;
  dp=minisyni_ratev[noteid&0x7f];
}

/* Note Adjust.
 */
 
void synth_event_note_adjust(uint8_t chid,uint8_t noteid,uint8_t velocity) {
//TODO
}

/* Control Change.
 */
 
void synth_event_control(uint8_t chid,uint8_t k,uint8_t v) {
//TODO
}

/* Program Change.
 */
 
void synth_event_program(uint8_t chid,uint8_t pid) {
//TODO
}

/* Channel Pressure.
 */
 
void synth_event_pressure(uint8_t chid,uint8_t v) {
//TODO
}

/* Pitch Wheel.
 */
 
void synth_event_wheel(uint8_t chid,uint16_t v) {
//TODO
}