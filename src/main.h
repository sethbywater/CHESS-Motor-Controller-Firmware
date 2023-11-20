// Prototypes
void main(void);
void system_clock_config(void);

void foc_isr(void);

// Math
void clarke(float u, float v, float w, float *a, float *b);
void inv_clarke(float a, float b, float *u, float *v, float *w);
void park(float a, float b, float angle, float *d, float *q);
void inv_park(float d, float q, float angle, float *a, float *b);
