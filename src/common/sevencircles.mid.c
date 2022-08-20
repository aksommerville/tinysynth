#include <stdint.h>

#if TS_tiny
  #include <avr/pgmspace.h>
#else
  #define PROGMEM
#endif

const uint8_t song_sevencircles[] PROGMEM={
4,17,0,6,0,6,240,126,166,240,117,65,127,113,240,56,45,240,27,190,127,113,240,119,178,241,244,62,127,
113,240,104,53,127,113,240,252,185,127,127,127,99,240,104,53,240,95,65,127,113,240,61,178,240,121,62,
127,113,240,105,38,240,106,193,127,113,240,79,173,240,25,190,127,113,240,105,50,241,212,190,127,113,
240,113,53,127,113,241,39,185,127,127,127,99,240,131,65,127,113,240,115,62,127,113,240,138,166,240,138,
195,127,113,240,101,45,240,29,64,127,113,240,121,178,241,215,64,127,113,240,130,53,127,113,240,238,57,
127,127,127,99,240,105,53,240,122,67,127,113,240,128,192,127,113,240,126,38,144,69,71,127,113,240,116,
45,127,113,240,41,178,127,113,240,26,181,127,113,241,27,57,127,127,127,127,127,127,127,71,240,123,38,
240,117,65,47,128,69,65,127,66,240,67,45,240,29,62,127,113,240,130,50,241,227,62,127,113,240,126,181,
127,113,240,231,185,127,127,127,99,240,128,181,240,110,193,127,113,240,51,178,240,129,190,127,113,240,
125,38,240,115,65,127,113,240,112,173,240,33,62,127,113,240,125,50,241,220,190,127,113,240,111,181,127,
113,241,30,57,127,127,127,99,240,126,65,127,113,240,131,62,127,113,240,146,166,240,125,64,127,113,240,
86,45,240,25,188,127,113,240,137,178,241,235,188,127,113,240,125,53,127,113,240,241,185,127,127,127,
99,240,102,181,240,111,64,127,113,240,138,188,127,113,240,135,166,241,12,190,127,113,240,76,41,127,113,
240,121,45,127,113,240,115,50,127,113,240,138,181,127,113,240,146,185,127,113,240,207,62,127,113,240,
143,65,127,113,240,159,69,127,113,240,153,74,127,113,240,130,77,127,113,240,147,209,127,113,240,177,
214,127,127,127,127,127,127,127,71,240,164,38,241,86,192,241,85,65,127,113,240,109,173,127,113,240,122,
50,127,113,240,105,45,240,97,62,127,113,240,128,166,127,113,240,117,173,127,113,240,124,50,241,104,64,
241,98,193,127,113,240,122,173,127,113,240,155,166,127,113,240,124,45,240,126,62,127,113,240,104,50,
127,113,240,114,45,127,113,240,129,166,240,138,195,127,113,240,127,173,240,144,69,127,113,240,116,50,
240,110,198,127,113,240,121,45,240,140,197,127,113,240,192,39,241,123,198,127,113,240,130,174,127,113,
240,108,179,127,113,240,116,174,241,121,63,127,113,240,145,39,127,113,240,124,46,127,113,240,97,179,
241,249,195,127,113,240,119,174,127,113,240,146,167,127,113,240,114,46,127,113,240,105,51,127,113,240,
125,174,127,113,240,231,167,240,135,183,240,158,195,127,113,240,121,174,127,113,240,123,51,240,117,185,
240,153,197,127,113,240,125,174,127,113,240,157,166,241,41,192,241,81,65,127,113,240,118,173,127,113,
240,139,178,127,113,240,126,173,241,3,190,127,113,240,158,166,127,113,240,125,173,127,113,240,122,50,
241,110,192,241,102,65,127,113,240,128,173,127,113,240,149,38,127,113,240,131,45,240,156,190,127,113,
240,121,178,127,113,240,110,173,127,113,240,124,38,240,133,67,127,113,240,124,45,240,133,69,127,113,
240,126,178,240,106,70,127,113,240,124,45,240,142,69,127,113,240,154,167,144,70,70,127,113,240,120,46,
127,113,240,131,51,127,113,240,107,174,127,113,240,178,167,127,113,240,130,174,26,128,70,38,127,87,240,
118,179,240,241,198,127,113,240,121,46,127,113,240,144,39,241,246,200,127,113,240,119,174,127,113,240,
107,51,127,113,240,122,46,127,113,240,159,39,240,237,202,127,113,240,122,46,127,113,240,135,179,127,
113,240,117,46,127,113,240,179,43,240,209,186,240,225,70,127,113,240,106,50,127,113,240,135,183,127,
113,240,123,50,240,191,54,240,221,66,127,113,240,171,43,127,113,240,116,178,127,113,240,217,55,241,18,
67,127,113,240,136,178,127,113,240,162,43,127,113,240,117,50,241,28,57,127,113,240,131,183,127,113,240,
115,50,127,113,240,153,171,240,218,58,127,113,240,138,178,127,113,240,115,55,240,100,188,127,113,240,
54,178,127,113,240,135,167,241,9,58,241,6,198,127,113,240,127,174,127,113,240,119,179,127,113,240,137,
174,240,217,182,240,238,66,127,113,240,143,39,127,113,240,132,46,127,113,240,106,51,241,236,55,144,67,
69,127,113,240,124,46,127,113,240,178,39,127,113,240,119,174,127,113,240,133,51,127,55,128,67,79,58,
240,125,46,127,113,240,123,39,241,21,210,127,113,240,133,46,127,113,240,147,179,127,113,240,114,46,127,
113,240,172,43,241,12,186,241,4,198,127,113,240,113,50,127,113,240,125,55,127,113,240,120,50,240,220,
182,240,237,66,127,113,240,146,171,127,113,240,118,178,127,113,240,219,55,241,0,67,127,113,240,129,178,
127,113,240,156,43,127,113,240,116,50,241,50,185,127,113,240,137,183,127,113,240,110,178,127,113,240,
140,171,240,227,58,127,113,240,123,50,127,113,240,121,55,241,13,188,127,113,240,49,50,127,113,240,136,
39,144,58,64,127,113,240,125,46,127,113,240,109,179,127,113,240,115,46,127,113,240,161,39,127,113,240,
117,174,127,113,240,96,51,127,113,240,114,46,127,113,240,163,39,144,55,62,29,128,58,49,127,84,240,107,
46,127,113,240,112,179,127,113,240,131,46,127,113,240,203,167,127,34,128,55,50,127,127,127,127,127,127,
37,240,202,166,241,105,192,241,88,65,127,113,240,116,45,127,113,240,132,50,127,113,240,114,45,240,103,
190,127,113,240,136,166,127,113,240,122,45,127,113,240,123,50,241,115,192,241,104,193,127,113,240,124,
45,127,113,240,158,166,127,113,240,116,45,240,134,62,127,113,240,112,50,127,113,240,115,45,127,113,240,
121,38,240,121,67,127,113,240,125,45,240,139,69,127,113,240,121,50,240,116,70,127,113,240,128,173,240,
138,197,127,113,240,166,167,241,124,198,127,113,240,124,174,127,113,240,116,179,127,113,240,132,46,241,
128,191,127,113,240,153,39,127,113,240,124,46,127,113,240,103,51,144,67,70,127,113,240,121,174,127,113,
240,130,39,127,113,240,130,174,127,113,240,118,179,127,99,128,67,65,14,240,126,174,127,113,240,227,39,
240,123,55,240,155,195,127,113,240,124,46,127,113,240,123,51,240,124,57,240,163,69,127,113,240,120,46,
127,113,240,155,166,241,86,64,241,83,65,127,113,240,109,173,127,113,240,124,50,127,113,240,114,45,240,
128,190,127,113,240,144,38,127,113,240,112,45,127,113,240,124,178,241,138,192,241,128,193,127,113,240,
123,45,127,113,240,131,38,127,113,240,144,173,240,123,62,127,113,240,124,50,127,113,240,115,173,127,
113,240,124,38,240,133,67,127,113,240,121,45,240,115,69,127,113,240,134,50,240,124,70,127,113,240,133,
173,240,155,197,127,113,240,163,39,144,70,68,127,113,240,129,174,127,113,240,103,179,127,113,240,125,
174,127,113,240,162,39,127,113,240,127,174,40,128,70,34,127,73,240,113,51,240,242,198,127,113,240,124,
46,127,113,240,160,39,241,243,72,127,113,240,126,174,127,113,240,116,179,127,113,240,122,46,127,113,
240,215,167,241,3,202,127,113,240,131,46,127,113,240,142,51,127,113,240,121,46,127,113,240,164,171,240,
237,58,240,237,70,127,113,240,114,50,127,113,240,125,55,127,113,240,121,178,240,241,182,241,0,66,127,
113,240,161,43,127,113,240,114,50,127,113,240,210,55,240,251,195,127,113,240,126,178,127,113,240,139,
171,127,113,240,125,50,241,67,57,127,113,240,133,55,127,113,240,128,178,127,113,240,137,171,240,214,
186,127,113,240,117,50,127,113,240,126,183,240,111,188,127,113,240,57,50,127,113,240,152,39,240,252,
186,240,250,198,127,113,240,132,46,127,113,240,130,51,127,113,240,115,46,240,192,182,240,213,194,127,
113,240,152,39,127,113,240,134,174,127,113,240,121,179,241,160,183,241,237,195,127,113,240,118,174,127,
113,240,191,167,127,113,240,135,174,127,113,240,129,179,127,113,240,128,174,127,113,240,157,167,241,
64,82,127,113,240,124,46,127,113,240,146,179,127,113,240,110,174,127,113,240,173,171,241,1,58,241,2,
198,127,113,240,120,50,127,113,240,125,55,127,113,240,118,178,240,245,54,241,5,194,127,113,240,146,171,
127,113,240,110,178,127,113,240,232,183,241,1,67,127,113,240,121,50,127,113,240,173,43,127,113,240,117,
178,241,19,185,127,113,240,133,183,127,113,240,117,178,127,113,240,146,171,240,183,186,127,113,240,118,
178,127,113,240,136,55,240,104,60,127,113,240,75,178,127,113,240,192,167,240,241,186,240,255,70,127,
113,240,127,174,127,113,240,118,179,127,113,240,137,174,240,198,60,240,224,200,127,113,240,184,167,127,
113,240,131,46,127,113,240,139,179,240,210,190,240,227,74,127,113,240,131,174,127,113,240,161,39,127,
113,240,151,46,240,211,191,240,233,75,127,113,240,138,179,127,113,240,117,46,127,113,240,218,39,240,
141,65,240,127,205,127,113,240,137,174,127,113,240,151,51,240,152,67,240,146,207,127,113,240,118,174,
127,113,240,244,50,240,143,192,127,113,240,120,57,240,149,193,127,113,240,141,62,241,82,72,127,113,240,
99,57,127,113,240,145,50,127,113,240,129,185,240,134,64,127,113,240,116,190,240,117,193,127,113,240,
103,185,240,140,72,127,113,240,163,178,241,232,197,127,113,240,121,57,127,113,240,127,190,127,113,240,
107,185,127,113,240,151,48,240,119,193,127,113,240,112,55,240,114,64,127,113,240,87,60,240,126,62,127,
106,240,128,60,7,240,90,183,127,113,240,224,46,240,155,62,127,113,240,119,181,240,128,192,127,113,240,
125,58,241,114,200,127,113,240,132,53,127,113,240,174,174,127,113,240,119,181,240,164,64,127,113,240,
114,58,240,135,193,127,113,240,139,181,240,244,74,127,113,240,223,174,127,113,240,116,181,240,160,65,
127,113,240,121,186,240,127,195,127,113,240,116,53,241,107,205,127,113,240,205,176,127,113,240,110,183,
127,113,240,125,188,240,240,204,127,113,240,117,55,127,113,240,173,178,240,135,192,127,113,240,112,57,
240,131,65,127,113,240,132,62,241,93,72,127,113,240,114,57,127,113,240,171,50,127,113,240,120,57,240,
164,192,127,113,240,115,62,240,120,65,127,113,240,118,185,240,136,72,127,113,240,148,50,241,223,197,
127,113,240,134,57,127,113,240,126,62,127,113,240,108,185,127,113,240,149,176,240,114,65,127,113,240,
118,183,240,130,192,127,112,240,95,60,240,134,190,127,114,240,101,183,240,102,188,127,113,240,156,174,
144,62,77,127,113,240,116,53,127,113,240,107,186,127,113,240,121,53,127,113,240,234,46,127,113,240,136,
181,127,113,240,124,58,127,113,240,127,181,127,113,240,227,48,123,128,62,1,117,240,117,183,127,113,240,
129,188,127,113,240,105,55,127,113,240,190,48,240,109,193,127,113,240,117,183,240,111,192,127,113,240,
113,62,127,93,240,155,60,20,240,121,55,127,113,240,187,178,240,135,64,127,113,240,114,57,240,150,65,
127,113,240,123,62,241,79,200,127,113,240,117,185,127,113,240,189,50,127,113,240,124,57,240,164,192,
127,113,240,120,62,240,118,193,127,113,240,117,57,240,145,72,127,113,240,161,50,241,232,69,127,113,240,
131,185,127,113,240,126,190,127,113,240,106,57,127,113,240,152,48,240,122,65,127,113,240,114,55,240,
116,192,127,113,240,127,62,127,100,240,158,60,13,240,97,55,127,113,240,157,174,240,153,190,127,113,240,
129,181,240,133,64,127,113,240,114,58,241,107,200,127,113,240,134,181,127,113,240,162,174,127,113,240,
122,53,240,167,64,127,113,240,111,186,240,131,65,127,113,240,140,53,240,234,74,127,113,240,168,174,127,
113,240,120,53,240,145,193,127,113,240,123,58,240,118,195,127,113,240,116,53,241,115,205,127,113,240,
155,176,127,113,240,122,55,127,113,240,123,60,241,6,76,127,113,240,135,55,127,113,240,139,178,240,123,
64,127,113,240,117,185,240,125,193,127,113,240,150,62,241,105,200,127,113,240,125,185,127,113,240,194,
178,127,113,240,116,57,240,156,192,127,113,240,127,62,240,142,65,127,113,240,128,185,240,142,72,127,
113,240,182,178,144,74,80,127,113,240,122,57,127,113,240,125,62,127,113,240,110,185,127,113,240,157,
176,241,217,72,67,128,74,66,127,46,240,124,55,127,113,240,117,188,127,113,240,103,55,127,113,240,129,
174,240,145,62,127,113,240,115,181,240,118,192,127,113,240,90,186,240,114,72,127,113,240,131,53,240,
161,192,127,113,240,175,174,240,116,193,127,113,240,115,53,240,116,74,127,113,240,115,186,240,188,65,
127,113,240,138,53,240,138,67,127,113,240,146,174,240,122,76,127,113,240,119,181,240,171,67,127,113,
240,123,58,240,126,69,127,113,240,123,53,240,116,77,127,113,240,152,46,240,212,197,127,113,240,132,53,
240,95,70,127,113,240,140,186,240,112,79,127,113,240,51,53,240,140,74,127,113,240,174,178,240,146,209,
120,240,143,57,120,240,120,74,127,113,240,133,62,240,146,197,127,113,240,115,57,240,125,81,127,113,240,
173,178,240,127,202,127,113,240,135,57,240,108,197,127,113,240,121,62,240,123,81,127,113,240,118,185,
240,122,202,127,113,240,164,178,240,137,69,127,113,240,129,185,240,125,81,127,113,240,127,62,240,108,
202,127,113,240,115,57,240,143,69,127,113,240,158,50,240,130,210,127,113,240,123,57,240,107,74,127,113,
240,121,190,240,112,81,127,113,240,119,185,240,160,74,127,113,240,127,209,6,240,88,176,127,107,240,141,
74,9,240,121,183,127,104,240,150,69,6,240,98,188,127,107,240,116,81,15,240,94,183,127,97,240,180,176,
1,240,89,74,127,108,240,141,69,14,240,101,55,127,96,240,158,209,7,240,95,188,127,114,240,144,74,8,240,
81,183,127,105,240,136,197,5,240,96,176,127,108,240,157,209,8,240,121,183,127,90,240,121,188,15,240,
130,74,127,113,240,144,197,1,240,121,183,127,103,240,121,176,9,240,132,82,127,113,240,118,202,8,240,
121,183,127,96,240,121,188,9,240,115,81,127,113,240,137,202,14,240,121,183,127,99,240,134,174,240,140,
210,127,113,240,123,53,240,144,74,127,113,240,109,186,240,119,198,127,113,240,131,53,240,135,210,127,
113,240,166,174,240,126,74,127,113,240,121,53,240,129,198,127,113,240,123,58,240,126,82,127,113,240,
134,181,240,141,74,127,113,240,160,46,240,145,70,127,113,240,138,181,240,169,82,127,113,240,111,186,
240,133,74,127,113,240,106,181,240,122,70,127,113,240,213,174,240,123,81,127,113,240,129,53,240,126,
74,127,113,240,144,58,240,111,207,127,113,240,111,53,240,124,74,127,113,240,216,48,240,133,79,127,113,
240,111,183,240,120,72,127,113,240,128,188,240,99,195,127,113,240,129,183,240,130,79,127,113,240,223,
176,240,120,72,127,113,240,121,55,240,131,195,127,113,240,125,60,240,127,207,127,113,240,124,55,240,
117,200,127,113,240,192,176,240,131,67,127,113,240,138,183,240,127,207,127,113,240,213,188,240,140,72,
127,113,240,34,183,240,126,67,127,113,240,172,48,240,120,81,127,113,240,133,55,240,131,72,127,113,240,
151,60,240,123,79,127,113,240,117,183,240,122,72,127,113,240,194,178,240,136,209,127,113,240,121,57,
240,118,202,127,113,240,113,62,240,122,69,127,113,240,118,185,240,128,209,127,113,240,205,50,240,125,
74,127,113,240,123,57,240,122,69,127,113,240,136,190,240,124,209,127,113,240,130,185,240,132,74,127,
113,240,193,178,240,118,197,127,113,240,148,185,240,135,81,127,113,240,122,62,240,126,202,127,113,240,
116,57,240,153,69,127,113,240,161,50,240,130,82,127,113,240,134,57,240,110,202,127,113,240,136,190,240,
119,209,127,113,240,115,185,240,126,202,127,113,240,177,176,240,120,81,127,113,240,122,55,240,126,74,
127,113,240,123,60,240,124,69,127,113,240,131,183,240,115,81,127,113,240,203,176,240,116,74,127,113,
240,116,183,240,132,69,127,113,240,126,60,240,123,81,127,113,240,120,55,240,124,74,127,113,240,164,176,
240,137,69,127,113,240,131,55,240,118,209,127,113,240,151,60,240,134,74,127,113,240,120,55,240,146,197,
127,113,240,171,48,240,137,210,127,113,240,133,55,240,111,202,127,113,240,117,188,240,111,209,127,113,
240,92,183,240,135,74,127,113,240,141,46,240,133,82,127,113,240,114,53,240,146,202,127,113,240,112,58,
240,132,70,127,113,240,126,181,240,131,82,127,113,240,157,174,240,145,74,127,113,240,126,181,240,135,
198,127,113,240,112,186,240,123,82,127,113,240,132,53,240,139,74,127,113,240,187,46,240,128,198,127,
113,240,142,53,240,135,82,127,113,240,126,58,240,140,74,127,113,240,106,53,240,123,70,127,113,240,139,
174,240,127,209,127,113,240,133,53,240,121,74,127,113,240,150,186,240,139,207,127,113,240,123,53,240,
128,198,127,113,240,192,176,240,165,207,127,113,240,122,55,240,138,200,127,113,240,137,60,240,124,67,
127,113,240,125,183,240,147,207,127,113,240,173,48,240,125,72,127,113,240,108,183,240,113,67,127,113,
240,138,188,240,138,207,127,113,240,122,55,240,123,72,127,113,240,169,176,240,130,195,127,113,240,137,
183,240,162,79,127,113,240,136,188,240,131,72,127,113,240,117,183,240,236,67,127,113,240,183,48,240,
110,209,127,113,240,138,183,240,140,200,127,113,240,125,60,240,135,207,127,113,240,111,55,240,136,200,
127,113,240,229,50,144,81,78,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,15,144,80,71,
42,128,81,78,127,127,127,127,127,127,127,127,127,127,127,127,127,127,100,144,77,72,16,128,80,44,127,
127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,126,
128,77,69,127,127,127,127,15,
};

const uint16_t song_sevencircles_length=4088;
