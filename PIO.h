#ifndef __PIO_H__
#define __PIO_H__

extern void PIO_Init(void);
extern void PinInterrupt_ISR (void);

extern void handleButtonDebounce(void);
extern void handleClickProtect(void);
extern void PIO_button_proc(void);

#endif